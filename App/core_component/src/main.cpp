/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "main.hpp"
#include "CommunicationEngine.hpp"
#include "Config.hpp"
#include "NodeEngine.hpp"
#include <iostream>

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();
	NodeEngine *ne = NodeEngine::getInstance();
//	ne.getNodeList();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");

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
	return 0;
}
