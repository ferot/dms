/*
 * STRGEvent.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: tf
 */

#ifndef STRGEVENT_HPP_
#define STRGEVENT_HPP_

#include <string.h>
#include "Event.hpp"


/*
 * Class for STRG Event representation.
 */
class StrgEvent : public Event{
private:

public:
	StrgEvent(eventType type);

};

#endif /* STRGEVENT_HPP_ */
