/*
 * CommEvent.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: tf
 */

#ifndef COMMEVENT_HPP_
#define COMMEVENT_HPP_

#include <string.h>
#include "Event.hpp"


/*
 * Class for Communication Event representation.
 */
class CommEvent : public Event{
private:

public:
	CommEvent(eventType type);
	void setParam(string param);

};

#endif /* COMMEVENT_HPP_ */
