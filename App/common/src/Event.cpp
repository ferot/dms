/*
 * Event.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: tf
 */
#include "Event.hpp"

Event::Event(eventType type, std::string params) : m_type(type) {
	m_uuid = boost::uuids::random_generator()();
	setParamUUID();
	Json::CharReaderBuilder reader;
	reader.parse(params, m_params);
}

std::string Event::getEventId() const {
	return boost::lexical_cast<std::string>(m_uuid);
}

std::string Event::getEventString() const {
	Json::StreamWriterBuilder writer;
	std::string value = writer.write(m_params);

	return value;
}

eventType Event::getEventType() const {
	return m_type;
}

void Event::setParamUUID() {
	m_params["uuid"] = getEventId();
}

void Event::setType(eventType type) {
	m_type = type;
}

void Event::setParam(std::string param) {
	Json::CharReaderBuilder reader;
	reader.parse(param, m_params);
}

bool Event::operator <(const Event& src) const {
	return (this->m_uuid < src.m_uuid);
}
