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
			[event](std::pair<t_eventPtr, t_commandPtr> pair) {
				if(pair.first->getEventType() == event->getEventType()) {
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
CommonRC DispatchEngine::registerEvent(t_eventPtr event) {
	CommonRC ret = CMN_RC_SUCCESS;

	t_commandPtr cmdPtr;
	int eventType = event->getEventType();
	switch (eventType) {
	case COMMUNICATION_EVENT:
		cmdPtr = std::make_shared<PublishMsgCMD>();
		break;
	case TRACKING_EVENT:
		cmdPtr = std::make_shared<ObjectTrackMoveCMD>();
		break;
	default:
		LOGMSG_ARG(LOG_ERROR, "[registerEvent] Event type %d not registered!",
				eventType);
		ret = CMN_RC_ERROR;
	}
	LOGMSG_ARG(LOG_DEBUG,
			"[registerEvent] Registering command for event type %s", eventType);
	m_registeredCommands.insert(
			std::pair<t_eventPtr, t_commandPtr>(event,
					cmdPtr));
	return ret;
}

