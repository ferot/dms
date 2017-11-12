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

#include "logger.h"
#include "Config.hpp"
#include "Common.hpp"
#include "Tracker.hpp"

typedef std::pair<int, int> t_imgResPair;
typedef cv::Rect2d t_bBox;
/*
 * Class responsible for Vision operations.
 * Handles video sources and tracking workers management.
 */
class VisionEngine {
private:
	static VisionEngine* m_instance;
	std::map<int, std::shared_ptr<Track::Tracker>> m_trackers;
	std::vector<std::thread> m_th_trackers;
	bool m_vidOpened;

	/*mapping of tracker id into thread id
	 essential to manage properly both objects*/
	std::map<int, std::thread::id> m_mapIdtrckTothr;

	cv::VideoCapture video;
	double m_vidStrWid;
	double m_vidStrHei;
	Config* config;

public:
	VisionEngine(string streamSource = "", int w = 0, int h = 0);
	~VisionEngine();
	static VisionEngine* getInstance(string streamSource = "", int w = 0,
			int h = 0);
	bool getVidOpened();
	t_imgResPair getActualImgRes();

	TrcEnRc addTracker(std::string trackerType, int id);
	TrcEnRc startAllTrackers();
	TrcEnRc stopAllTrackers();
	std::shared_ptr<Track::Tracker> getTracker(int id);

	inline cv::VideoCapture getVidCapture() {
		return this->video;
	}
	TrcEnRc isVidOpened();
	TrcEnRc displayDebugWindow();
	//SEND METHODS
	std::string send(std::string);
};

#endif /* VisionENGINE_HPP_ */
