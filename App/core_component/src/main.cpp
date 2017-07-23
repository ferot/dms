/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "main.hpp"
#include "CommunicationEngine.hpp"

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");
	CommunicationEngine ce("dupa", 8383);
	ce.addConnection(string("192.168.0.0"),8989,"lol");
	ce.addConnection(string("192.168.0.1"),8989,"lol1");
	ce.printConnections();
	CommEngineRetCode ret = ce.startServer();
	if(ret != COMM_ENG_SUCCESS)
		LOGMSG(LOG_ERROR, "Server didn't start prperly!");
	getchar();
	ce.stopServer();
	return 0;
}
