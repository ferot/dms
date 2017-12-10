/*
 * HaarTracker.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: tf
 */
#include "HaarTracker.hpp"
#include "logger.h"

HaarTracker * HaarTracker::m_instance = nullptr;

HaarTracker::HaarTracker() {
	m_face_cascade_name = "haarcascade_frontalface_alt2.xml";
	m_face_cascade_name = "haarcascade_profileface.xml";
	init();
	LOGMSG(LOG_DEBUG, "LOADED CASCADES");
}

HaarTracker* HaarTracker::getInstance() {
	if (!m_instance) {
		m_instance = new HaarTracker();
	}
	return m_instance;
}

bool HaarTracker::init() {
	if (!m_face_cascade.load(m_face_cascade_name)) {
		LOGMSG(LOG_DEBUG, "[initImpl] Error loading cascade!");
		return false;
	}

	if (!m_profile_cascade.load(m_face_cascade_name)) {
		printf("[initImpl] Error loading cascade!");
		return false;
	}

	return true;
}

/*
* Constructor
*/
cv::Ptr<HaarTracker> HaarTracker::createTracker(){
	cv::Ptr<HaarTracker> trck = HaarTracker::getInstance();
    return cv::Ptr<HaarTracker>(trck);
}

/**
 * Runs Haar detection algorithm on desired image and ROI.
 * Applies next cascade if first fails, to ensure that object is visible.
 * @param image - reference to frame to be checked.
 * @param boundingBox - region of interest (ROI) to be checked.
 * @return true on successfull check, otherwise false.
 */
bool HaarTracker::update(const cv::Mat& image, cv::Rect2d& boundingBox) {
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;
	bool ret = false;

	cv::cvtColor(image, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	m_face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2,
			0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	if (faces.size() == 0) {
		LOGMSG(LOG_TRACE, "[HaarTracker::update] Applying next cascade!");

		m_profile_cascade.detectMultiScale(frame_gray, faces, 1.1, 2,
				0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	}
	//Checking again, cause it may change after second cascade apply.
	if (faces.size() != 0) {
		boundingBox = faces[0];
		ret = true;
	}

	return ret;
}
