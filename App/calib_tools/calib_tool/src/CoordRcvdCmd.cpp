/*
 * CoordRcvdCmd.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: tf
 */
#include "CoordRcvdCmd.hpp"

/**
 * Updates values of gui labels according to cam_id.
 *
 * @param params
 * @return
 */
CommonRC CoordsRcvdCmd::execute(std::string params) {
	std::string value;
	Json::Value root;
	Json::CharReaderBuilder reader;

	reader.parse(params.c_str(), root);

	auto &vals = root["payload"];
	int id = vals["cam_id"].asInt();
	t_tup_thrstrs tuple = {
			vals["x"].asString(),
			vals["y"].asString(),
			vals["width"].asString()};

	m_ctInstance->setLabelValues(tuple, id);

	return CMN_RC_SUCCESS;
}

