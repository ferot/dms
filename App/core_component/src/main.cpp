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

	DispatchEngine *de = DispatchEngine::getInstance();

	shared_ptr<Event> ev(make_shared<CommEvent>(eventType::COMMUNICATION_EVENT));
	LOGMSG(LOG_DEBUG, "created event!");

	de->registerEvent(ev);
	LOGMSG(LOG_DEBUG, "registered event!");

	for(int i =0; i<10; i++){
	de->enqueueEvent(ev);
	LOGMSG(LOG_DEBUG, "enqueued event!");
	}


	NodeEngine *ne = NodeEngine::getInstance();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");

	CommunicationEngine *ce = CommunicationEngine::getInstance();
	ce->connect();
	ce->subscribe("hellotopic");

	int ch;
    do
    {
    	ce->publish("WIADOMOSC", "hellotopic");
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    VisionEngine *ve = VisionEngine::getInstance();
	ve->addTracker("KCF", 0);
	ve->displayDebugWindow();
	ve->startAllTrackers();

	StorageEngine* se = StorageEngine::getInstance();
	se->create_table();

	return 0;
}
