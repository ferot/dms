/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#include <iostream>

#include "main.hpp"
#include "CommunicationEngine.hpp"
#include "StorageEngine.hpp"
#include "VisionEngine.hpp"
#include "Config.hpp"
#include "NodeEngine.hpp"
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

	NodeEngine *ne = NodeEngine::getInstance();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");

	CommunicationEngine *ce = CommunicationEngine::getInstance();

	VisionEngine *ve = VisionEngine::getInstance();
	ve->addTracker("KCF", 0);
	ve->displayDebugWindow();
	ve->startAllTrackers();

	StorageEngine* se = StorageEngine::getInstance();
	se->create_table();

	return 0;
}
