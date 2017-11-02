/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#include <iostream>

#include "main.hpp"

#include "Event.hpp"

#include "CommunicationEngine.hpp"
#include "CommEvent.hpp"
#include "StorageEngine.hpp"
#include "VisionEngine.hpp"
#include "Config.hpp"
#include "NodeEngine.hpp"

#include "DispatchEngine.hpp"

#include <iostream>
#include <thread>

#include "Event.hpp"

using namespace std;

int main() {

	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}
	LOGMSG(LOG_DEBUG, "Starting core_app main!");

	StorageEngine* se = StorageEngine::getInstance();
	CommunicationEngine *ce = CommunicationEngine::getInstance();
	DispatchEngine *de = DispatchEngine::getInstance();
	VisionEngine *ve = VisionEngine::getInstance();



	ce->connect();
	ce->subscribe("hellotopic");

	///////////////////DISPATCHER TESTS\\\\\\\\\\\\\\\\\\\\\


	vector<shared_ptr<Event>> ev_vector;
	for (int i = 0; i < 10; i++) {
		ev_vector.push_back(
				(make_shared<CommEvent>(eventType::COMMUNICATION_EVENT)));
		LOGMSG_ARG(LOG_DEBUG, "created %d event!", i);
	}

//	for (int i = 0; i < 10; i++) {
//		de->enqueueEvent(ev_vector.front());
//		LOGMSG_ARG(LOG_DEBUG, "enqueued %d event!", i);
//	}

	CommonRC ret = de->startEventReader();
	if (ret == CMN_RC_SUCCESS) {
		LOGMSG(LOG_DEBUG, "started reader thread");
	}

	////////////////////////DISPATCHER TESTS \\\\\\\\\\\\\\\\\\\\

	int ch;
//	do {
//		ce->publish("WIADOMOSC", "hellotopic");
//		ch = getchar();
//	} while (ch != 'Q' && ch != 'q');

	ve->addTracker("KCF", 0);
	ve->startAllTrackers();

//	ve->displayDebugWindow();

	se->create_table();
	ve->stopAllTrackers();
	return 0;
}
