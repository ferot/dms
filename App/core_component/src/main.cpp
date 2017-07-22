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
	CommEngineRetCode ret = ce.startServer();
	if(ret != COMM_ENG_SUCCESS)
		LOGMSG(LOG_ERROR, "Server didn't start prperly!");
	getchar();
	ce.stopServer();
	return 0;
}
