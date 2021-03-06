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

typedef std::shared_ptr<Event> t_eventPtr;
typedef std::shared_ptr<Command> t_commandPtr;
typedef MWSRQueue<std::deque<t_eventPtr>> t_eventDeque;

/**
 * Class responsible for mapping events
 *  on concrete commands and dispatching them.
 *
 */
class DispatchEngine {
private:

	static DispatchEngine* m_instance;
	Config* config;

	std::map<eventType, t_commandPtr> m_registeredCommands;

	std::shared_ptr<t_eventDeque> m_eventQueue;

	bool m_evReaderKill;
	std::thread m_th_readerThread;

	CommonRC eventReader();

	DispatchEngine();
	~DispatchEngine();

	CommonRC handleEvent(t_eventPtr event);
public:

	static DispatchEngine* getInstance();

	t_eventPtr createEvent();
	CommonRC registerEvent(eventType type, std::shared_ptr<Command> usrEvPtr = nullptr);
	CommonRC enqueueEvent(t_eventPtr event);

	CommonRC startEventReader();
	CommonRC stopEventReader();
};

#endif /* DispatchEngine_HPP_ */
