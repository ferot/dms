/*
 * Tracker.hpp
 *
 *  Created on: Oct 15, 2017
 *      Author: tf
 */

#ifndef TRACKER_HPP_
#define TRACKER_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include "DispatchEngine.hpp"

namespace Track {

typedef cv::Rect2d t_bBox;

/*
 * Class representing single tracker module.
 * It's responsible is to detect object class and return it's coords.
 */
class Tracker {

private:
	int id;
	std::string m_trackerType;
	cv::Ptr<cv::Tracker> m_tracker;
	bool m_trackingEnabled;
	bool m_debugWindowEnabled;

	DispatchEngine* dispatchEngine;

	cv::Ptr<cv::Tracker> createTracker(std::string trackerType);
	TrcEnRc initTracker();

public:

	Tracker(std::string trackerType, int id = 0);

	bool processFrame(cv::Mat& frame, t_bBox&);
	TrcEnRc initializeTracker(cv::Mat&, t_bBox);
	void enableTracking();
	void disableTracking();

	t_eventPtr prepareEvent(t_bBox bbox);
	TrcEnRc enqueueEvent(t_eventPtr trackEvent);

};
}
#endif /* TRACKER_HPP_ */
