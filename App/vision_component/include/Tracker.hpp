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

#include "VisionEngine.hpp"
#include "DispatchEngine.hpp"

using namespace std;

/*
 * Class representing single tracker module.
 * It's responsible is to detect object class and return it's coords.
 */
class Tracker {

private:
	int id;
	string m_trackerType;
	cv::Ptr<cv::Tracker> m_tracker;
	bool m_trackingEnabled;

	DispatchEngine* dispatchEngine;

	cv::Ptr<cv::Tracker> createTracker(string trackerType);
	TrcEnRc initTracker();

	TrcEnRc enqueueEvent(t_eventPtr trackEvent);

public:
	Tracker(string trackerType, int id = 0);
	TrcEnRc startTracking();
	TrcEnRc stopTracking();
};

#endif /* TRACKER_HPP_ */
