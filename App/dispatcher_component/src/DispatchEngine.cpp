/*
 * DispatchEngine.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */
#include <iostream>
#include <utility>
#include <map>
#include <memory>

#include "DispatchEngine.hpp"
#include "Common.hpp"
#include "ConcreteCommand.hpp"


DispatchEngine* DispatchEngine::m_instance = nullptr;

/**
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 *
 * @return
 */
DispatchEngine* DispatchEngine::getInstance() {
	if (!m_instance) {
		m_instance = new DispatchEngine();
	}
	return m_instance;
}

/**
 * Constructor of dispatcher engine.
 *
 * Params description as in getInstance
 */
DispatchEngine::DispatchEngine() {
	m_evReaderKill = true;
	config = Config::getInstance();

}

/**
 * Cleans up connection handler as they're allocated manually
 */
DispatchEngine::~DispatchEngine() {

}

/**
 * This methos is called from framework to handle the event
 * @param event
 * @return
 */
CommonRC DispatchEngine::handleEvent(t_eventPtr event) {
	/*
	 * Get event from queue
	 */
	CommonRC ret = CMN_RC_ERROR;
	auto it = find_if(m_registeredCommands.begin(), m_registeredCommands.end(),
			[event](std::pair<eventType, t_commandPtr> pair) {
				if(pair.first == event->getEventType()) {
					return true;
				} else {
					return false;
				}
			});

	if (it != m_registeredCommands.end()) {
		t_commandPtr cmdPtr = it->second;
		if (cmdPtr) {
			ret = cmdPtr->execute();
		} else {
			LOGMSG(LOG_ERROR, "[handleEvent] Command is invalid!!");
		}
	}
	return ret;
}

/**
 * This function is called by the appication to register its command for Events.
 *
 * @param event
 * @return
 */
CommonRC DispatchEngine::registerEvent(eventType type) {
	CommonRC ret = CMN_RC_SUCCESS;

	t_commandPtr cmdPtr;

	LOGMSG_ARG(LOG_DEBUG, "[registerEvent] Attempt to create event %d", type);
	switch ((int)type) {
	case COMMUNICATION_EVENT:
		cmdPtr = std::make_shared<PublishMsgCMD>();
		break;
	case TRACKING_EVENT:
		cmdPtr = std::make_shared<ObjectTrackMoveCMD>();
		break;
	default:
		LOGMSG_ARG(LOG_ERROR, "[registerEvent] Event type %d not registered!",
				(int)type);
		ret = CMN_RC_ERROR;
	}

	LOGMSG_ARG(LOG_DEBUG,
			"[registerEvent] Registering command for event type %d", (int)type);

	m_registeredCommands.insert(
			std::pair<eventType, t_commandPtr>(type, cmdPtr));
	return ret;
}

/**
 * Pushes event on back of the event-process queue.
 *
 * @param event - event to be enqueued
 */
CommonRC DispatchEngine::enqueueEvent(t_eventPtr event){
	CommonRC ret = CMN_RC_SUCCESS;
	m_eventQueue.push_back(move(event));
	LOGMSG_ARG(LOG_DEBUG, "[enqueueEvent] Enqueued event type : %d", event->getEventType());

	return ret;
}

/**
 * Spawns thread responsible for reading events from queue and dispatching
 * proper actions.
 */
CommonRC DispatchEngine::startEventReader() {
	CommonRC ret = CMN_RC_SUCCESS;

	LOGMSG(LOG_DEBUG, "[startEventReader] Starting event-reader thread...");

	m_evReaderKill = false;
	m_th_readerThread = std::thread(&DispatchEngine::eventReader, this);

	return ret;
}

/**
 * Stops event reader thread by setting flag.
 */
CommonRC DispatchEngine::stopEventReader(){
	CommonRC ret = CMN_RC_SUCCESS;

	m_evReaderKill = true;
	m_th_readerThread.join();

	LOGMSG(LOG_DEBUG, "[stopEventReader] Joined event-reader thread...");

	return ret;
}

/**
 * Thread's worker responsible for getting events from queue
 * and applying them into handleEvent() to map specific actions.
 */
CommonRC DispatchEngine::eventReader() {
	CommonRC ret = CMN_RC_SUCCESS;
	do {
		auto event = m_eventQueue.pop_back();
		handleEvent(event.second);
		LOGMSG_ARG(LOG_DEBUG,
				"[PublishMsgCMD::execute()] Handled event type : %d",
				event.second->getEventType());
	} while (!m_evReaderKill);

	return ret;
}

