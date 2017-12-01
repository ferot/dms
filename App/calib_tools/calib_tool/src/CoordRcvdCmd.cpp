/*
 * CoordRcvdCmd.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: tf
 */
#include "CoordRcvdCmd.hpp"

CommonRC CoordsRcvdCmd::execute(std::string params){
	std::string value;
	Json::Value root;
    Json::Reader reader;
	LOGMSG_ARG(LOG_DEBUG, "value = %s", params.c_str());

	reader.parse(params.c_str(), root);
	value = root["payload"]["x"].asString();
	LOGMSG_ARG(LOG_DEBUG, "value = %s", value.c_str());

	m_ctInstance->setValue(value);

	return CMN_RC_SUCCESS;
}




