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

enum eventType {
	COMMUNICATION_EVENT, TRACKING_EVENT,

};

/*
 * Class defining common interface for wide range representation of events in application
 */
class Event {
private:
	/* Message unique ID */
	boost::uuids::uuid m_uuid;
	Json::Value m_params;
	eventType type;

	void setParamUUID();

public:
	Event();

	int getEventType() const;
	std::string getEventId() const;
	std::string getEventString() const;

	bool operator <(const Event& src) const;
};

#endif /* EVENT_HPP_ */
