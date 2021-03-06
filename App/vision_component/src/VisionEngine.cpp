/*
 * VisionEngine.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: tf
 */

#include <string>
#include "VisionEngine.hpp"
#include "Tracker.hpp"

VisionEngine* VisionEngine::m_instance = nullptr;

/* Maximum nr of devices expected to be connected */
const int maxDevCount = 3;

std::string buildVidSource(std::string camID);

/*
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 * This could be improve to get system's information about attached video source.
 */
VisionEngine* VisionEngine::getInstance(string streamSource, int streamWidth,
		int streamHeight) {
	if (!m_instance) {
		m_instance = new VisionEngine(streamSource, streamWidth, streamHeight);
	}
	return m_instance;
}

/**
 * Constructor of Vision Engine.
 * Opens stream (by default - video dev with index 0 : /dev/video0).
 * And sets resolution. If specified uses settings from user.
 * If not fetches values from config file.
 *
 * @param streamSource
 * @param streamWidth
 * @param streamHeight
 */
VisionEngine::VisionEngine(std::string streamSource, int streamWidth, int streamHeight) {
    m_vidOpened = false;

	config = Config::getInstance();

	if (m_vidStrHei && m_vidStrWid) {
		m_vidStrWid = streamWidth;
		m_vidStrHei = streamHeight;
	} else {
		m_vidStrWid = stod(config->getValue("Video", "width"));
		m_vidStrHei = stod(config->getValue("Video", "height"));
	}

	std::string camID = config->getValue("Video", "cam_id");

	if (streamSource.empty()) {
		streamSource = buildVidSource(camID);
	}

	m_camId = std::stoi(camID);

	if (openVideo(streamSource)) {
		video.set(CV_CAP_PROP_FRAME_WIDTH, m_vidStrWid);
		video.set(CV_CAP_PROP_FRAME_HEIGHT, m_vidStrHei);
		LOGMSG_ARG(LOG4C_PRIORITY_ERROR, "Setting resolution to %s",
				(std::to_string(m_vidStrWid) + "x" + std::to_string(m_vidStrHei)).c_str());
		m_vidOpened = true;
	} else {
		LOGMSG(LOG_ERROR, "Couldn't open any video device! Aborting...");
		abort();
	}

	LOGMSG_ARG(LOG_TRACE, "CAM_ID set to %d", m_camId);

}

/**
 * Tries to open specified dev node as opencv video handler.
 * If fails attempts to get next devnode as fallback.
 * In case none of the nodes is working false is returned, otherwise true.
 * NOTE : modifies m_camId - used in communication mode
 *
 * @param streamSource
 * @return boolean true if successfull, false on failure
 */
bool VisionEngine::openVideo(std::string streamSource){
	int attemptCount = 0;
	bool openedFlag = video.open(streamSource);

    while(openedFlag != true && attemptCount < maxDevCount){
		std::string fallback_dev = buildVidSource(std::to_string(++m_camId));
		openedFlag = video.open(fallback_dev);
		attemptCount++;
	}

	if (!openedFlag) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't open video source %s",
				streamSource.c_str());
	}

	return openedFlag;
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

/**
 * Method returning pair of actual set video stream properties.
 * @return integer pair with corresponding first:width, second:height
 */
t_imgResPair VisionEngine::getActualImgRes(){
	t_imgResPair pair;
	pair.first = video.get(CV_CAP_PROP_FRAME_WIDTH);
	pair.second = video.get(CV_CAP_PROP_FRAME_HEIGHT);

	return pair;
}
/*
 * Add tracker.
 * Inserts entry to trackers map.
 * If id is not provided, generates unique
 */
TrcEnRc VisionEngine::addTracker(string trackerType, int id) {
	TrcEnRc ret = TRCK_ENG_SUCCESS;
	id = 0;

	auto it = m_trackers.insert(m_trackers.begin(), //hint for maximum performance in inserting.
			std::make_pair(id, new Track::Tracker(trackerType, id)));
	if (it->second) {
		LOGMSG_ARG(LOG_DEBUG, "Added tracker with id : %d", id);
	} else {
		LOGMSG_ARG(LOG_ERROR, "Couldn't add tracker with id : %d", id);
		ret = TRCK_ENG_ERROR;
	}
	return ret;
}

/*
 * Method responsible for spawning tracking worker threads,
 * mapped by tracker id's from m_trackers.
 * TODO: check if this shouldn't be tried for exception.
 */
TrcEnRc VisionEngine::startAllTrackers() {
	TrcEnRc ret = TRCK_ENG_ERROR;

	for (auto it : m_trackers) {
		it.second->enableTracking();
	}
	return ret;
}

/*
 * Method responsible for stopping all trackers mapped by m_trackers id.
 * And also erases map's entry.
 */
TrcEnRc VisionEngine::stopTracker(int id) {
	TrcEnRc ret = TRCK_ENG_ERROR;

	auto& tracker = m_trackers[id];
	//Find thread with such id and remove from tracker map.
	if (tracker) {
		tracker->disableTracking();

		LOGMSG_ARG(LOG_TRACE, "Erasing tracker [%d]", id);
		m_trackers.erase(id);

		ret = TRCK_ENG_SUCCESS;
	}
	return ret;
}

std::shared_ptr<Track::Tracker> VisionEngine::getTracker(int id) {
	auto iter = find_if(m_trackers.begin(), m_trackers.end(),
			[id](std::pair<int, std::shared_ptr<Track::Tracker>> pair) {
				if(pair.first == id) {
					return true;
				} else {
					return false;
				}
			});
	if (iter != m_trackers.end()) {
		return iter->second;
	}
	return nullptr;
}

const bool& VisionEngine::getVidOpened() const{
    return m_vidOpened;
}

const int& VisionEngine::getCamId(){
	return m_camId;
}

/**
 * Creates video dev node path based on provided camID string
 * @param camID
 * @return string with path to video devnode
 */
std::string buildVidSource(std::string camID){
	return std::string("/dev/video") + camID;

}
