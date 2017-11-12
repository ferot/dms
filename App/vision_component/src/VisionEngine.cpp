/*
 * VisionEngine.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: tf
 */

#include "VisionEngine.hpp"
#include "Tracker.hpp"

VisionEngine* VisionEngine::m_instance = nullptr;

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

	if (streamSource.empty()) {
		streamSource = "/dev/video0";
	}
	config = Config::getInstance();

	if (m_vidStrHei && m_vidStrWid) {
		m_vidStrWid = streamWidth;
		m_vidStrHei = streamHeight;
	} else {
		m_vidStrWid = stod(config->getValue("Video", "width"));
		m_vidStrHei = stod(config->getValue("Video", "height"));
	}

	if (!video.open(streamSource)) {
		LOGMSG_ARG(LOG4C_PRIORITY_ERROR, "Couldn't open video source %s",
				streamSource.c_str());
	} else {
		video.set(CV_CAP_PROP_FRAME_WIDTH, m_vidStrWid);
		video.set(CV_CAP_PROP_FRAME_HEIGHT, m_vidStrHei);
		LOGMSG_ARG(LOG4C_PRIORITY_ERROR, "Setting resolution to %s",
				(to_string(m_vidStrWid) + "x" + to_string(m_vidStrHei)).c_str());
        m_vidOpened = true;
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
	static int trackerCount;
	if (!id) {
		//not user-friendly format
		id = trackerCount++;
	}

	m_trackers.insert(std::make_pair(id, new Track::Tracker(trackerType, id)));
//	config->setValue("nodes", "client-" + id, ip + ":" + numToString(port));
	LOGMSG_ARG(LOG_DEBUG, "Added tracker with id : %d", id);

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
		m_th_trackers.push_back(std::thread(&Track::Tracker::enableTracking, it.second));

		auto id = m_th_trackers.back().get_id();
		m_mapIdtrckTothr.insert(make_pair(it.first, id));

		LOGMSG_ARG(LOG_TRACE, "Spawning tracker[%d]", (it.first));
		LOGMSG_ARG(LOG_TRACE, "...with threads id[%d]", id);

	}
	return ret;
}

/*
 * Method responsible for stopping all threads mapped by m_trackers member.
 * Erases both tracker and associated thread.
 */
TrcEnRc VisionEngine::stopAllTrackers() {
	TrcEnRc ret = TRCK_ENG_ERROR;

	std::vector<std::thread>::iterator iter = m_th_trackers.begin();
	while (iter != m_th_trackers.end()) {
		auto thrId = iter->get_id();

		//Wait for thread to join and erase it from thread vector.
		iter->join();
		iter = m_th_trackers.erase(iter);
		LOGMSG_ARG(LOG_TRACE, "Erasing tracker thread [%d]", (thrId));

		//Find thread with such id and remove from tracker map.
		auto trckIter = std::find_if(m_mapIdtrckTothr.begin(),
				m_mapIdtrckTothr.end(), [thrId](std::pair<int, thread::id> pair) {
					return ((pair.second == thrId) ? true : false);
				});
		m_trackers.erase(trckIter->first);
		LOGMSG_ARG(LOG_TRACE, "Erasing tracker [%d]", (thrId));

		//also update trackerId to threadId map
		m_mapIdtrckTothr.erase(trckIter);
		LOGMSG_ARG(LOG_TRACE, "Erasing m_mapIdtrckTothr entry with key-Id [%d]", trckIter);

	}
	return ret;
}

/*
 * Method responsible for displaying debug window with stream.
 * On 'ESC' key window is terminated.
 */
TrcEnRc VisionEngine::displayDebugWindow() {
	TrcEnRc ret = TRCK_ENG_ERROR;
	LOGMSG(LOG4C_PRIORITY_CRIT, "Opening debug window...");

	cv::namedWindow( "Tracking", cv::WINDOW_AUTOSIZE );// Create a window for display.

	if (isVidOpened() == TRCK_ENG_SUCCESS) {
		cv::VideoCapture video = getVidCapture();
		cv::Mat frame;
		while (video.read(frame)) {
			// Display frame.
			cv::imshow("Tracking", frame);

			// Exit if ESC pressed. This needs to be replaced for some signalling probably from UI.
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

shared_ptr<Track::Tracker> VisionEngine::getTracker(int id) {
	auto iter = find_if(m_trackers.begin(), m_trackers.end(),
			[id](std::pair<int, shared_ptr<Track::Tracker>> pair) {
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

bool VisionEngine::getVidOpened(){
    return m_vidOpened;
}
