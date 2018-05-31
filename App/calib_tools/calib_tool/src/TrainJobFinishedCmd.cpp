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
	static int progress;

	std::lock_guard<std::mutex> lockGuard(m_ctInstance->m_jobMutex);

	m_ctInstance->setProgressBar(progress);
	m_ctInstance->notifyProcessedJob();

	m_ctInstance->removeJob(id);
	//TODO : if last item -> set flags, zero everything
	return CMN_RC_SUCCESS;
}

