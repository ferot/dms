/*
 * VisionEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef VisionENGINE_HPP_
#define VisionENGINE_HPP_
#include <string>
#include <log4c.h>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

//#include "Tracker.hpp"
#include "logger.h"
#include "Config.hpp"
#include "Common.hpp"
#include "Tracker.hpp"

using namespace std;
class Tracker;

/*
 * Class responsible for Vision operations.
 * Handles video sources and tracking workers management.
 */
class VisionEngine {
private:
	static VisionEngine* m_instance;
	map<int, Tracker *> m_trackers;

	cv::VideoCapture video;
	Config* config;

public:
	VisionEngine(string streamSource = "");
	~VisionEngine();
	static VisionEngine* getInstance(string streamSource = "");

	TrcEnRc addTracker(string trackerType, int id);
	TrcEnRc startAllTrackers();
	TrcEnRc stopAllTrackers();

	inline cv::VideoCapture getVidCapture() {
		return this->video;
	}
	TrcEnRc isVidOpened();
	TrcEnRc displayDebugWindow();
	//SEND METHODS
	string send(string);
};

#endif /* VisionENGINE_HPP_ */
