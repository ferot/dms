#include "visionEngineWrapper.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

cv::Mat g_frame;
t_bBox trackResult;

unsigned long long interval;

VisionEngineWrapper::~VisionEngineWrapper(){
	rythm.stop();
}

VisionEngineWrapper::VisionEngineWrapper() :
		rythm(this) {
	m_visionEngine = VisionEngine::getInstance();

	m_state = INIT_S;

	m_visionEngine->addTracker("KCF", 0);
	m_tracker = m_visionEngine->getTracker(0);
	m_htracker = HaarTracker::createTracker();

	m_video = m_visionEngine->getVidCapture();

	m_debugWinEnabled = false;
	m_modelDebugWinEnabled = false;
	m_trackingEnabled = false;
	m_trackerInited = false;

	scaler = 1;

	QObject::connect(&rythm, &QTimer::timeout, this,
			&VisionEngineWrapper::worker);

//	t_imgResPair resolution = m_visionEngine->getActualImgRes();
	bbox = new cv::Rect2d(50, 50, 150, 150);

	rythm.start(0);
}

void VisionEngineWrapper::stopTracker() {
	if (m_visionEngine->stopTracker(0) != TRCK_ENG_SUCCESS) {
		LOGMSG(LOG_ERROR,
				"[VisionEngineWrapper::stopTracker] Couldn't stop tracker !");
	}
}

void VisionEngineWrapper::slot_debugWindowClicked(bool switched) {
	m_debugWinEnabled = (m_debugWinEnabled == false) ? true : false;
}

void VisionEngineWrapper::slot_modelDebugWindowClicked(bool){
	m_modelDebugWinEnabled = (m_modelDebugWinEnabled == false) ? true : false;
}

/**
 * Checks if object exists in desired ROI.
 *
 * @param frame
 * @param bounding
 * @return true if object is detected, otherwise false.
 */
bool VisionEngineWrapper::checkObjAtBnd(cv::Mat& frame, t_bBox &bounding){
    //conversion needed as haar detect needs Rect<int>
	cv::Rect bnd = static_cast<cv::Rect>(bounding);

    bool ret = this->m_htracker->update(frame, bnd);
    bounding = bnd;

    return ret;
}

/**
 * Checks in whole image for existence of faces.
 *
 * @param tr - track result - modified if any face had been detected.
 */
void VisionEngineWrapper::findTargetStateFn(t_bBox& tr) {
	if (checkObjAtBnd(g_frame, *bbox)) {
		tr = *bbox;
		// state transition should be done only if any target was found!!!
		m_state = INIT_TRCK_S;
	}
}

void VisionEngineWrapper::initTrackStateFn(t_bBox& tr) {
	this->m_visionEngine->addTracker("KCF", 0);
	m_tracker->initializeTracker(g_frame, tr);

	m_trackingEnabled = true;
	m_trackerInited = true;

	m_state = RUN_TRCK_S;
}

void VisionEngineWrapper::runTrackStateFn(t_bBox& tr) {

	bool ret = false;
	std::packaged_task<bool()> trackTask([&]() {
		ret = m_tracker->processFrame(g_frame, tr);
		return ret;
	});

	std::future<bool> futureBoundings = trackTask.get_future();

	std::thread th(std::move(trackTask));
	th.join();

	if (interval % scaler == 0) {
		m_state = VERIF_TRGT_S;
		t_eventPtr trackEvent = m_tracker->prepareEvent(trackResult);
		m_tracker->enqueueEvent(trackEvent);
	}

}

void VisionEngineWrapper::verifyTargetStateFn(t_bBox& tr) {
	if (checkObjAtBnd(g_frame, tr)) {
		m_state = RUN_TRCK_S;
	} else {
		disableTracking();
		m_state = FIND_TRGT_S;
	}

}

void VisionEngineWrapper::worker() {
	float fps;
	// Start timer
	double timer = (double) cv::getTickCount();
	if (m_visionEngine->getVidOpened()) {
		m_video.read(g_frame);
	}

	interval++;

	switch (m_state) {
	case INIT_S:
		LOGMSG(LOG_ERROR, "[VisionEngineWrapper::worker]INIT_S");

	case FIND_TRGT_S:
		LOGMSG(LOG_ERROR, "[VisionEngineWrapper::worker]FIND_TRGT_S");

		findTargetStateFn(trackResult);
		break;
	case INIT_TRCK_S:
		LOGMSG(LOG_ERROR, "[VisionEngineWrapper::worker]INIT_TRCK_S");

		initTrackStateFn(trackResult);
		break;
	case RUN_TRCK_S:
		LOGMSG(LOG_ERROR, "[VisionEngineWrapper::worker]RUN_TRCK_S");

		runTrackStateFn(trackResult);
		break;
	case VERIF_TRGT_S:
		LOGMSG(LOG_ERROR, "[VisionEngineWrapper::worker]VERIF_TRGT_S");

		verifyTargetStateFn(trackResult);
		break;
	case STOP_S:
		break;
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

	if (m_modelDebugWinEnabled) {
		emit sig_notifyModelDebugWindow(
				cv::Point(trackResult.x - 50, trackResult.y - 50));

	}
}

void VisionEngineWrapper::disableTracking(){
	m_trackingEnabled = false;
	m_trackerInited = false;

	stopTracker();
}

static bool reinitFlag = false;

/**
 * Slot responsible for setting bounding box, to start tracker.
 * Allows to reset bound box and reinitialize tracker for new region.
 * @param keyCode - Qt::Key code.
 */
void VisionEngineWrapper::slot_keyHandler(int keyCode) {

	switch (keyCode) {
	case 68:	// "D"
		bbox->x += 5;
		break;
	case 65:	// "A"
		bbox->x -= 5;
		break;
	case 87:	// "S"
		bbox->y -= 5;
	case 73:	// "I"
	{
		if (interval > 0) {
			scaler -= 1;
		}
		LOGMSG_ARG(LOG_ERROR, "[slot_keyHandler] scaler = %d", scaler);
		break;
	}
		case 79:
		// "O"
		scaler += 1;
		LOGMSG_ARG(LOG_ERROR, "[slot_keyHandler] scaler = %d", scaler);
		break;

	case 83:	// "W"
		bbox->y += 5;
		break;
	case 82:	// "R" --> reset tracking region
	{
		if (reinitFlag) {
			disableTracking();
			reinitFlag = false;
		}
		break;
	}
	case 84: // "T" --> track (reinitialize)
	{
		this->m_visionEngine->addTracker("KCF", 0);
		m_tracker->initializeTracker(g_frame, *bbox);

		m_trackingEnabled = true;
		m_trackerInited = true;
		reinitFlag = true;
		break;
	}

	}
}
