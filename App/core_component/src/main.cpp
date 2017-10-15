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
#include "Event.hpp"

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();
	NodeEngine *ne = NodeEngine::getInstance();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");
	VisionEngine *ve = VisionEngine::getInstance();
	ve->addTracker("KCF", 0);
	ve->displayDebugWindow();
	CommunicationEngine *ce = CommunicationEngine::getInstance("127.0.0.3", 8383);
	ce->addConnection("localhost",8383,"lol");
	ce->addConnection("10.0.2.15",8383,"lol1");
	ce->printConnections();

	ComEnRc ret = ce->startServer();
	if(ret != COMM_ENG_SUCCESS)
		LOGMSG(LOG_ERROR, "Server didn't start prperly!");
	LOGMSG_ARG(LOG_DEBUG, "server sent : %s", ce->send(string("lol")).c_str());
	LOGMSG_ARG(LOG_DEBUG, "server sent : %s", ce->send(string("lol1")).c_str());

	ce->stopServer();
	
	StorageEngine* se = StorageEngine::getInstance();
	se->create_table();

	return 0;
}
