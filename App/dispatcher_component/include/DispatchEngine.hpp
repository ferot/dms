/*
 * DispatchEngine.hpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#ifndef DispatchEngine_HPP_
#define DispatchEngine_HPP_
#include <string>
#include <log4c.h>

#include "logger.h"
#include "Config.hpp"

#include "Event.hpp"
#include "ConcreteCommand.hpp"

using namespace std;

typedef shared_ptr<Event> t_eventPtr;
typedef shared_ptr<Command> t_commandPtr;

/**
 * Class responsible for mapping events
 *  on concrete commands and dispatching them.
 *
 */
class DispatchEngine {
private:

	static DispatchEngine* m_instance;
	Config* config;

	map<t_eventPtr, t_commandPtr> m_registeredCommands;

	DispatchEngine();
	~DispatchEngine();

	CommonRC handleEvent(t_eventPtr event);
public:

	static DispatchEngine* getInstance();
	CommonRC registerEvent(t_eventPtr event);

};

#endif /* DispatchEngine_HPP_ */
