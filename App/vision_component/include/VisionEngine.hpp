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

#include <thread>


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
	map<int, shared_ptr<Tracker>> m_trackers;
	std::vector<std::thread> m_th_trackers;

	/*mapping of tracker id into thread id
	essential to manage properly both objects*/
	map<int,thread::id> m_mapIdtrckTothr;

	cv::VideoCapture video;
	double m_vidStrWid;
	double m_vidStrHei;
	Config* config;

public:
	VisionEngine(string streamSource = "", int w = 0, int h = 0);
	~VisionEngine();
	static VisionEngine* getInstance(string streamSource = "",
			int w = 0,
			int h = 0);

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
