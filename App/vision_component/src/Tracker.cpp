#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include "Tracker.hpp"

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
	return tracker;
}

/*
 * Method responsible for putting generated event on tracking manager event's queue.
 * Special care for synchronization should be taken here.
 */
TrcEnRc Tracker::enqueueEvent() {

}

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

/*
 * Method responsible for spawning thread with tracking worker.
 * Assures that video stream is avaialable.
 */
TrcEnRc Tracker::startTracking() {

	TrcEnRc vidOpened = VisionEngine::getInstance()->isVidOpened();

	if (vidOpened == TRCK_ENG_SUCCESS) {
		m_trackingEnabled = true;
	} else {
		return TRCK_ENG_VIDDEV_ERROR;
	}
	// Read first frame
	cv::Mat frame;
	cv::VideoCapture video = VisionEngine::getInstance()->getVidCapture();

	bool ok = video.read(frame);

	// Define initial boundibg box
	cv::Rect2d bbox(287, 23, 86, 320);

	// Uncomment the line below to select a different bounding box
	bbox = selectROI(frame, false);

	// Display bounding box.
	cv::rectangle(frame, bbox, cv::Scalar(255, 0, 0), 2, 1);
	imshow("Tracking", frame);

	m_tracker->init(frame, bbox);

	while (video.read(frame) && m_trackingEnabled) { //need to add some kind of flag to additionaly control loop
		// Start timer
		double timer = (double) cv::getTickCount();

		// Update the tracking result
		bool ok = m_tracker->update(frame, bbox);

		// Calculate Frames per second (FPS)
		float fps = cv::getTickFrequency()
				/ ((double) cv::getTickCount() - timer);

		if (ok) {
			// Tracking success : Draw the tracked object
			rectangle(frame, bbox, cv::Scalar(255, 0, 0), 2, 1);
		} else {
			// Tracking failure detected.
			putText(frame, "Tracking failure detected", cv::Point(100, 80),
					cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
		}

		// Display tracker type on frame
		putText(frame, m_trackerType + " Tracker", cv::Point(100, 20),
				cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

		// Display FPS on frame
		putText(frame, "FPS : " + SSTR(int(fps)), cv::Point(100, 50),
				cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

	}
}

/*
 * Method responsible for signalling thread to stop tracking.
 */
TrcEnRc Tracker::stopTracking() {
	m_trackingEnabled = false;
	return TRCK_ENG_SUCCESS;
}