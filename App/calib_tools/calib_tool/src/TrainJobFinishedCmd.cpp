/*
 * CoordRcvdCmd.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: tf
 */
#include "TrainJobFinishedCmd.hpp"

/**
 * Updates values of gui labels according to cam_id.
 *
 * @param params
 * @return
 */
CommonRC TrainJobFinishedCmd::execute(std::string params) {

	int id = std::stoi(params);


	std::lock_guard<std::mutex> lockGuard(m_ctInstance->m_jobMutex);

	m_ctInstance->removeJob(id);
	progress+=10;
	m_ctInstance->setProgressBar(progress);

	//TODO : increase progress bar
	//TODO : check if job vector is empty. ->if so remove it  and set flags to allow new jobs
//	m_ctInstance->setLabelValues(tuple, id);


	return CMN_RC_SUCCESS;
}

