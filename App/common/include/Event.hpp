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
	DEFAULT_EVENT, COMMUNICATION_EVENT, TRACKING_EVENT, USER_EVENT = 123, JOB_FINISHED_EVENT

};

/*
 * Class defining common interface for wide range representation of events in application
 */
class Event {
private:
	/* Message unique ID */
	boost::uuids::uuid m_uuid;

	Json::Value m_params;
	eventType m_type;

	void setParamUUID();

public:

	Event(eventType type, std::string params = "");

	eventType getEventType() const;
	std::string getParams() const;
	std::string getEventId() const;
	std::string getEventString() const;

	void setType(eventType type);
	void setParam(std::string param);
	bool operator <(const Event& src) const;
};

#endif /* EVENT_HPP_ */
