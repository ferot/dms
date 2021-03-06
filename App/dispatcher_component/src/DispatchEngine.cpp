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

/** Arbitrary number of events stored for processing.
 * May have strong impact on performance.
 * Set only when config value not present in config.ini.
 */
const int fixedQueueSize = 20;
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

	int queueSize = stoi(config->getValue("Other", "queue_size"), nullptr, 10);
	queueSize = (queueSize != 0) ? queueSize : fixedQueueSize;

    LOGMSG_ARG(LOG_TRACE, "[DispatchEngine] setting queueSize : %d", queueSize);

	m_eventQueue = std::make_shared<t_eventDeque>(queueSize);
}

/**
 * Cleans up connection handler as they're allocated manually
 */
DispatchEngine::~DispatchEngine() {
	stopEventReader();
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

			std::string payload = event->getEventString();
			ret = cmdPtr->execute(payload);
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
CommonRC DispatchEngine::registerEvent(eventType type, std::shared_ptr<Command> usrEvPtr) {
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
		LOGMSG_ARG(LOG_DEBUG, "[registerEvent] Event type %d is not standard. "
				"Registering user-type event", (int)type);

		if (usrEvPtr != nullptr) {
			cmdPtr = usrEvPtr;
		} else {
			ret = CMN_RC_ERROR;
		}
	}

	LOGMSG_ARG(LOG_DEBUG,
			"[registerEvent] Registering command for event type %d", (int)type);

	auto result = m_registeredCommands.insert(
			std::pair<eventType, t_commandPtr>(type, cmdPtr));
	if (!result.second) {
		LOGMSG_ARG(LOG_ERROR, "[registerEvent] Couldn't insert event %d!",
				(int )type);
		ret = CMN_RC_ERROR;
	}
	return ret;
}

/**
 * Pushes event on back of the event-process queue.
 *
 * @param event - event to be enqueued
 */
CommonRC DispatchEngine::enqueueEvent(t_eventPtr event){
	CommonRC ret = CMN_RC_SUCCESS;
    LOGMSG_ARG(LOG_TRACE, "[enqueueEvent] Enqueued event type : %d",
			(int )event->getEventType());

	m_eventQueue->push_back(move(event));

	return ret;
}

/**
 * Spawns thread responsible for reading events from queue and dispatching
 * proper actions.
 */
CommonRC DispatchEngine::startEventReader() {
	CommonRC ret = CMN_RC_SUCCESS;

    LOGMSG(LOG_TRACE, "[startEventReader] Starting event-reader thread...");

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
	m_eventQueue->kill();

    LOGMSG(LOG_TRACE, "[stopEventReader] Joined event-reader thread...");

	return ret;
}

/**
 * Thread's worker responsible for getting events from queue
 * and applying them into handleEvent() to map specific actions.
 */
CommonRC DispatchEngine::eventReader() {
	CommonRC ret = CMN_RC_SUCCESS;
	do {
		auto event = m_eventQueue->pop_front();
		handleEvent(event.second);
        LOGMSG_ARG(LOG_TRACE,
				"[PublishMsgCMD::execute()] Handled event type : %d",
				event.second->getEventType());
	} while (!m_evReaderKill);

	return ret;
}

