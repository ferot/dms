/*
 * Event.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: tf
 */
#include "Event.hpp"

Event::Event() {
	m_uuid = boost::uuids::random_generator()();
	setParamUUID();
}

std::string Event::getEventId() {
	return boost::lexical_cast<std::string>(m_uuid);
}

std::string Event::getEventString() {
	Json::StyledWriter writer;

	return writer.write(m_params);
}

void Event::setParamUUID() {
	m_params["uuid"] = getEventId();
}
