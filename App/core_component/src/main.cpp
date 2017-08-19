/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "main.hpp"
#include "CommunicationEngine.hpp"
#include "Config.hpp"
#include <iostream>

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();

    Config conf("conf.json");
    conf.setValue(std::string("other2"), std::string("conf"), std::string("dupaaa"));
    std::cout<<"\nconfig file: "<< conf.getValue(std::string("other2"), std::string("conf"))<<std::endl;

    conf.setValue(std::string("other3"), std::string("co11nf"), std::string("du23232paaa"));

    std::cout<<"\nconfig file: "<< conf.getValue(std::string("other3"), std::string("co11nf"))<<std::endl;

	LOGMSG(LOG_DEBUG, "Starting core_app main!");
	CommunicationEngine ce("dupa", 8383);
	ce.addConnection(string("localhost"),8383,"lol");
	ce.addConnection(string("10.0.2.15"),8383,"lol1");
	ce.printConnections();
	CommEngineRetCode ret = ce.startServer();
	if(ret != COMM_ENG_SUCCESS)
		LOGMSG(LOG_ERROR, "Server didn't start prperly!");
	LOGMSG_ARG(LOG_DEBUG, "server sent : %s", ce.send(string("lol")).c_str());
	LOGMSG_ARG(LOG_DEBUG, "server sent : %s", ce.send(string("lol1")).c_str());

	ce.stopServer();
	return 0;
}