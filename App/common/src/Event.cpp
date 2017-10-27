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

std::string Event::getEventId() const {
	return boost::lexical_cast<std::string>(m_uuid);
}

std::string Event::getEventString() const {
	Json::StyledWriter writer;

	return writer.write(m_params);
}

void Event::setParamUUID() {
	m_params["uuid"] = getEventId();
}

bool Event::operator <(const Event& src) const {
	return (this->m_uuid < src.m_uuid);
}
