/*
 * VisionEngine.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: tf
 */

#include "VisionEngine.hpp"

VisionEngine* VisionEngine::m_instance = nullptr;

/*
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 * This could be improve to get system's information about attached video source.
 */
VisionEngine* VisionEngine::getInstance(string streamSource) {
	if (!m_instance) {
		m_instance = new VisionEngine(streamSource);
	}
	return m_instance;
}

/*
 * Constructor of Vision Engine.
 * Opens stream (by default - video dev with index 0 : /dev/video0).
 */
VisionEngine::VisionEngine(string streamSource) {
	if (streamSource.empty()) {
		streamSource = "/dev/video0";
	}
	config = Config::getInstance();
	if(!video.open(streamSource))
	{
		LOGMSG_ARG(LOG4C_PRIORITY_ERROR, "Couldn't open video source %s", streamSource.c_str());
	}
}

/*
 * Checks if videostream has been connected and ready to use.
 */
TrcEnRc VisionEngine::isVidOpened() {
	if (!video.isOpened()) {
		LOGMSG(LOG4C_PRIORITY_ERROR, "Video source is not available!!!");
		return TRCK_ENG_VIDDEV_ERROR;
	} else {
		LOGMSG(LOG4C_PRIORITY_INFO, "Video source is available");
		return TRCK_ENG_SUCCESS;
	}
}

/*
 * Add tracker.
 * Inserts entry to trackers map.
 * If id is not provided, generates unique
 */
TrcEnRc VisionEngine::addTracker(string trackerType, int id) {
	TrcEnRc ret = TRCK_ENG_SUCCESS;
	static int dupa;
	if (!id) {
		//not user-friendly format
		id = dupa++;
	}

	m_trackers.insert(make_pair(id, new Tracker(trackerType, id)));
//	config->setValue("nodes", "client-" + id, ip + ":" + numToString(port));

	return ret;
}

TrcEnRc VisionEngine::startAllTrackers() {
	TrcEnRc ret = TRCK_ENG_ERROR;

	for (auto it : m_trackers) {
		m_th_trackers.push_back(
				std::thread(&Tracker::startTracking, it.second));
		auto id = m_th_trackers.back().get_id();
		m_mapIdtrckTothr.insert(make_pair(it.first, id));
		LOGMSG_ARG(LOG_TRACE, "Spawning tracker thread with id: %d",
				(it.first));
	}
	return ret;
}

TrcEnRc VisionEngine::stopAllTrackers() {
	TrcEnRc ret = TRCK_ENG_ERROR;

	std::vector<std::thread>::iterator iter = m_th_trackers.begin();
	while (iter != m_th_trackers.end()) {
		//Wait for thread to join and erase it from thread vector.
		auto thrId = iter->get_id();
		iter->join();
		iter = m_th_trackers.erase(iter);

		//Find thread with such id and remove from tracker map.
		auto trckIter = std::find_if(m_mapIdtrckTothr.begin(),
				m_mapIdtrckTothr.end(), [thrId](std::pair<int, thread::id> pair) {
					return ((pair.second == thrId) ? true : false);
				});
		m_trackers.erase(trckIter->first);

		//also update trackerId to threadId map
		m_mapIdtrckTothr.erase(trckIter);
	}
	return ret;
}

TrcEnRc VisionEngine::displayDebugWindow() {
	TrcEnRc ret = TRCK_ENG_ERROR;
	LOGMSG(LOG4C_PRIORITY_CRIT, "Opening debug window...");
	if (isVidOpened() == TRCK_ENG_SUCCESS) {
		cv::VideoCapture video = getVidCapture();
		cv::Mat frame;
		while (video.read(frame)) {
			// Display frame.
			cv::imshow("Tracking", frame);

			// Exit if ESC pressed. This needs to be replaced for some signalling.
			int k = cv::waitKey(1);
			if (k == 27) {
				stopAllTrackers();
				ret = TRCK_ENG_SUCCESS;
				break;
			}
		}
	}
	return ret;
}
