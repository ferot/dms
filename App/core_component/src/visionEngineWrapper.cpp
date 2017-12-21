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

//**
// * Helper func printing bounding values essential for debugging.
// * @param rect
// */
//static void printRectInfo(cv::Rect rect) {
//	LOGMSG_ARG(LOG_TRACE, "Rect X = %d", rect.x);
//	LOGMSG_ARG(LOG_TRACE, "Rect Y = %d", rect.y);
//	LOGMSG_ARG(LOG_TRACE, "Rect WIDTH = %d", rect.width);
//	LOGMSG_ARG(LOG_TRACE, "Rect HEIGHT = %d", rect.height);
//
//}


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
	m_switchTracker = false;

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

void VisionEngineWrapper::slot_switchTrackerClicked(bool)
{
	m_switchTracker = (m_switchTracker == false) ? true : false;
}

/**
 * Checks if object exists in desired ROI. By default cropps image to check only ROI region.
 * If desired (for finding target when lost focus) checks whole image for object existence.
 * Returns true if object is found. Also returns (by modyfing initial bounding obj) bounding,
 * for object found in image.
 *
 * @param frame
 * @param bounding
 * @param whole_im - if true send whole image, otherwise crop to bounding
 * @return true if object is detected, otherwise false.
 */
bool VisionEngineWrapper::checkObjAtBnd(cv::Mat& frame, t_bBox &bounding,
		bool whole_im = false) {
	//conversion needed as haar detect needs Rect<int>
	cv::Rect bnd = static_cast<cv::Rect>(bounding);
	cv::Mat roi;

	if (!whole_im) { //avoid copying when we need to check whole frame anyway
		frame(bnd).copyTo(roi);
	} else {
		roi = frame;
	}

	bool ret = this->m_htracker->update(roi, bnd);
	bounding = bnd;

	return ret;
}

/**
 * Checks in whole image for existence of faces.
 *
 * @param tr - track result - modified if any face had been detected.
 */
void VisionEngineWrapper::findTargetStateFn(t_bBox& tr) {

	if (checkObjAtBnd(g_frame, tr, true)) {
		m_state = INIT_TRCK_S;
	}
}

void VisionEngineWrapper::initTrackStateFn(t_bBox& tr) {
	this->m_visionEngine->addTracker("KCF", 0);

#ifdef TRACKER_INC_ROI_OPT
	cv::Size deltaSize(tr.width * 0.3f, tr.height * 0.3f); // 0.1f = 10/100
	cv::Point offset(deltaSize.width / 2, deltaSize.height / 2);
	cv::Rect rect = tr;

	if ((rect.x + offset.x > 0) && (rect.y + offset.y > 0)
			&& (rect.x + offset.x + rect.width < m_visionEngine->getActualImgRes().first)
			&& (rect.y + offset.y + rect.height < m_visionEngine->getActualImgRes().second))
	{
		rect += deltaSize;
		rect -= offset;
	}
	tr = rect;
#endif

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
	}

	t_eventPtr trackEvent = m_tracker->prepareEvent(trackResult);
	m_tracker->enqueueEvent(trackEvent);
}

void VisionEngineWrapper::verifyTargetStateFn(t_bBox& tr) {

#ifdef TRACKER_INC_ROI_OPT
	if (tr.x < 0) {
		tr.x = 0;
	}
	if (tr.y < 0) {
		tr.y = 0;
	}
	if (tr.y + tr.height > m_visionEngine->getActualImgRes().second) {
		tr.y = m_visionEngine->getActualImgRes().second - tr.height;
	}
	if (tr.x + tr.width > m_visionEngine->getActualImgRes().first) {
		m_visionEngine->getActualImgRes().first);
		tr.x = m_visionEngine->getActualImgRes().first - tr.width;
	}
#endif

	t_bBox box(tr);
	if (checkObjAtBnd(g_frame, box)) {
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
		if (m_trackerInited) {
			cv::rectangle(g_frame, trackResult, cv::Scalar(255, 255, 0), 2, 1);
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

