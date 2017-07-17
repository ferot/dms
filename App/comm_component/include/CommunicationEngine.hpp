/*
 * CommunicationEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef COMMUNICATIONENGINE_HPP_
#define COMMUNICATIONENGINE_HPP_
#include <string>
#include "ServerStub.hpp"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <log4c.h>
#include "../../common/include/logger.h"

using namespace jsonrpc;
using namespace std;

enum CommEngineRetCode{
	COMM_ENG_SUCCESS,
	COMM_ENG_ERROR
};

/*
 * Class responsible for Communicating with nodes
 */
class CommunicationEngine {
private:
	string m_server_ip;
	int m_server_port;
	ServerStub* m_server_handle;
	int initLogger();
public:
	string getServerIp();
	int getServerPort();
	CommunicationEngine(string ip, int port);

	CommEngineRetCode startServer();
	CommEngineRetCode stopServer();

	//SEND METHODS
	void send();
};

#endif /* COMMUNICATIONENGINE_HPP_ */
