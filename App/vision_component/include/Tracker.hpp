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

	TrcEnRc enqueueEvent(t_eventPtr trackEvent);

public:
	Tracker(std::string trackerType, int id = 0);
	TrcEnRc startTracking();
	TrcEnRc stopTracking();

	void switchDebugWindow(bool switched);
};
}
#endif /* TRACKER_HPP_ */
