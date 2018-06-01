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
	LOGMSG_ARG(LOG_DEBUG, "RECEIVED EVT with ID = %d", id);

	std::lock_guard<std::mutex> lockGuard(m_ctInstance->m_jobMutex);
	m_ctInstance->notifyProcessedJob(id);

	return CMN_RC_SUCCESS;
}

