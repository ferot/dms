#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include "Tracker.hpp"
#include "logger.h"

using namespace Track;
/*
 * Tracker constructor.
 * @param trackerType -type of tracking method we want to use.
 * Available trackers for OPCV 3.2: "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW"
 * @param id - id of tracker. This value if not provided, should be obtained from Tracking Manager component.
 */
Tracker::Tracker(string trackerType, int id) {
	this->id = id;
	this->m_tracker = createTracker(trackerType);
	m_trackingEnabled = false;

	dispatchEngine = DispatchEngine::getInstance();
}

/*
 * Simple factory creating tracker according to type provided.
 * By default KCF tracker is created.
 */
cv::Ptr<cv::Tracker> Tracker::createTracker(string trackerType) {
	cv::Ptr<cv::Tracker> tracker;
	this->m_trackerType = trackerType;

#if (CV_MINOR_VERSION < 3)
	{
		tracker = cv::Tracker::create(trackerType);
	}
#else
	{
		switch(trackerType) {

			case "BOOSTING":
			tracker = TrackerBoosting::create();
			break;

			case "MIL":
			tracker = TrackerMIL::create();
			break;

			case "KCF":
			tracker = TrackerKCF::create();
			break;

			case "TLD":
			tracker = TrackerTLD::create();
			break;

			case "MEDIANFLOW":
			tracker = TrackerMedianFlow::create();
			break;

			default :
			tracker = TrackerKCF::create();
			this->trackerType = "KCF";
		}
	}
#endif
	LOGMSG_ARG(LOG_DEBUG, "Creating %s tracker.", trackerType.c_str());

	return tracker;
}

t_eventPtr Tracker::prepareEvent(t_bBox bbox) {

	t_eventPtr trackEvent(new Event(COMMUNICATION_EVENT));
	Json::Value eventParam;
	Json::FastWriter fastWriter;

	eventParam["width"] = to_string(bbox.width);
	eventParam["height"] = to_string(bbox.height);
	eventParam["x"] = to_string(bbox.x);
	eventParam["y"] = to_string(bbox.y);

	trackEvent->setParam(fastWriter.write(eventParam));

	return trackEvent;
}

/*
 * Method responsible for putting generated event on tracking manager event's queue.
 * To avoid blocking tracker worker enqeuing is being made with detached thread.
 */
TrcEnRc Tracker::enqueueEvent(t_eventPtr trackEvent) {

	std::thread t([trackEvent]() {
		DispatchEngine::getInstance()->enqueueEvent(trackEvent);
	});
	t.detach();

	return TRCK_ENG_SUCCESS;
}

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

/*
 * Method responsible for spawning thread with tracking worker.
 * Assures that video stream is avaialable.
 */
void Tracker::enableTracking(){
	m_trackingEnabled = true;
}

/*
 * Method responsible for signalling thread to stop tracking.
 */
void Tracker::disableTracking() {
	m_trackingEnabled = false;
}

bool Tracker::processFrame(cv::Mat frame, t_bBox & bbox) {
	bool ok = false;
	if (m_trackingEnabled) {
		ok = m_tracker->update(frame, bbox);
	}
	return ok;
}

TrcEnRc Tracker::initializeTracker(cv::Mat frame, t_bBox box) {
	TrcEnRc ret = TRCK_ENG_ERROR;
	bool inited = m_tracker->init(frame, box);

	if (inited == true) {
		enableTracking();
		ret = TRCK_ENG_SUCCESS;
	}

	return ret;
}

