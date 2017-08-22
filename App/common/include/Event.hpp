/*
 * Event.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: tf
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_
#include "Common.hpp"

using std::string;

/*
 * Class defining common interface for wide range representation of events in application
 */
class Event {
private:
	/* Message unique ID */
	boost::uuids::uuid m_uuid;
	Json::Value m_params;

	void setParamUUID();

public:
	Event();
	std::string getEventId();
	std::string getEventString();
};

#endif /* EVENT_HPP_ */
