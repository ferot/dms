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
#include <queue>
#include <thread>

#include "logger.h"
#include "Config.hpp"

//#include "QueueWrapper.hpp"
#include "MwsrQueue.hpp"

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

	map<eventType, t_commandPtr> m_registeredCommands;

	MWSRQueue<std::deque<t_eventPtr>> m_eventQueue;

	bool m_evReaderKill;
	std::thread m_th_readerThread;

	CommonRC eventReader();

	DispatchEngine();
	~DispatchEngine();

	CommonRC handleEvent(t_eventPtr event);
public:

	static DispatchEngine* getInstance();

	t_eventPtr createEvent();
	CommonRC registerEvent(eventType type);
	CommonRC enqueueEvent(t_eventPtr event);

	CommonRC startEventReader();
	CommonRC stopEventReader();
};

#endif /* DispatchEngine_HPP_ */
