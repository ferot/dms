#include "visionEngineWrapper.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

cv::Mat g_frame;
unsigned long long interval;

VisionEngineWrapper::~VisionEngineWrapper(){
	rythm.stop();
}

VisionEngineWrapper::VisionEngineWrapper() :
		rythm(this) {
	m_visionEngine = VisionEngine::getInstance();

	m_visionEngine->addTracker("KCF", 0);
	m_tracker = m_visionEngine->getTracker(0);

	m_video = m_visionEngine->getVidCapture();

	m_debugWinEnabled = false;
	m_trackingEnabled = false;
	m_trackerInited = false;

	QObject::connect(&rythm, &QTimer::timeout, this,
			&VisionEngineWrapper::worker);

	t_imgResPair resolution = m_visionEngine->getActualImgRes();
	bbox = new cv::Rect2d(resolution.first/2, resolution.second/2, 80, 150);

	rythm.start(0);
}

void VisionEngineWrapper::stopTracker() {
	m_visionEngine->stopAllTrackers();
}

void VisionEngineWrapper::slot_debugWindowClicked(bool switched) {
	m_debugWinEnabled = (m_debugWinEnabled == false) ? true : false;
}

void VisionEngineWrapper::slot_trackWindowClicked(bool){
	m_trackingEnabled = (m_trackingEnabled == false) ? true : false;
}


t_bBox VisionEngineWrapper::track() {
	t_bBox bounding;
	LOGMSG(LOG_DEBUG, "in track");

		std::packaged_task<bool()> trackTask(
				[&]() {
			LOGMSG(LOG_DEBUG, "in labmda");
					bool ret = this->m_visionEngine->getTracker(0)->processFrame(g_frame, bounding);
					return ret;
				});

		std::future<bool> futureBoundings = trackTask.get_future();

		std::thread th(std::move(trackTask));

		th.join();

	return bounding;
}

void VisionEngineWrapper::worker() {
	t_bBox trackResult;
	float fps;

	// Start timer
	double timer = (double) cv::getTickCount();
	if (m_visionEngine->getVidOpened()) {
		m_video.read(g_frame);
	}

	if (m_trackingEnabled) {
		LOGMSG(LOG_DEBUG, "trackingenabled");

		trackResult = track();
		if (interval % 5 == 0) {
			t_eventPtr trackEvent = m_tracker->prepareEvent(trackResult);
			m_tracker->enqueueEvent(trackEvent);
		}

	}
	//Calculate FPS
	fps = cv::getTickFrequency() / ((double) cv::getTickCount() - timer);

	if (m_debugWinEnabled) {
		//update debug window

		cv::rectangle(g_frame, trackResult, cv::Scalar(255, 255, 0), 2, 1);
		if (m_trackerInited == false) {
			cv::rectangle(g_frame, *bbox, cv::Scalar(255, 0, 0), 2, 1);
		}
		//		// Display FPS on frame
		cv::putText(g_frame, "FPS : " + std::to_string(int(fps)),
				cv::Point(100, 50), cv::FONT_HERSHEY_SIMPLEX, 0.75,
				cv::Scalar(50, 170, 50), 2);
		emit sig_notifyDebugWindow(g_frame);
	}
}


/**
 * Slot responsible for setting bounding box, to start tracker.
 * @param keyCode - Qt::Key code.
 */
void VisionEngineWrapper::slot_keyHandler(int keyCode){

	switch(keyCode){
	case 68:	// "D"
		bbox->x+=5;
		break;
	case 65:	// "A"
		bbox->x-=5;
		break;
	case 87:	// "S"
		bbox->y-=5;
		break;
	case 83:	// "W"
		bbox->y+=5;
		break;
	case 82:	// "R" --> reset tracking region
		m_trackerInited = false;
		m_trackingEnabled = false;
		break;
	case 84:	// "T" --> track
		TrcEnRc ret = this->m_visionEngine->getTracker(0)->initializeTracker(
				g_frame, *bbox);

		if (ret == TRCK_ENG_SUCCESS) {
			LOGMSG(LOG_DEBUG, "initialized tracker");
			m_trackingEnabled = true;
			m_trackerInited = true;
		}
		break;

	}
}
