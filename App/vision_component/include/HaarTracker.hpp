/*
 * HaarTracker.hpp
 *
 *  Created on: Nov 15, 2017
 *      Author: tf
 */

#ifndef HAARTRACKER_HPP_
#define HAARTRACKER_HPP_
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/tracking.hpp>


/**
 * Haar features-combination tracker.
 */
class HaarTracker {
private:
	static HaarTracker* m_instance;
	std::string m_face_cascade_name;
	std::string m_profile_cascade_name;

	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_profile_cascade;

    cv::Rect2d findMaxObj(std::vector<cv::Rect> objects, const cv::Mat & image);
public:

	static cv::Ptr<HaarTracker> createTracker();
	static HaarTracker* getInstance();
	bool init();
	bool update(const cv::Mat& image, cv::Rect& boundingBox);

	HaarTracker();

};

#endif /* HAARTRACKER_HPP_ */
