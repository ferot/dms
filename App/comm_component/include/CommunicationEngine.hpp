/*
 * CommunicationEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef COMMUNICATIONENGINE_HPP_
#define COMMUNICATIONENGINE_HPP_
#include <string>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <log4c.h>
#include <memory>
#include <map>

#include "logger.h"
#include "ServerStub.hpp"
#include "Connection.hpp"

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
	shared_ptr<ServerStub> m_stubserver_handle;
	shared_ptr<HttpServer> m_server;
	map<string, Connection *> m_connections;

	int initLogger();
	CommEngineRetCode obtainServerIP();
public:
	string getServerIp();
	int getServerPort();
	CommunicationEngine(string ip, int port);
	~CommunicationEngine();

	CommEngineRetCode addConnection(string ip, int port, string id);
	CommEngineRetCode startServer();
	CommEngineRetCode stopServer();
	void printConnections();
	//SEND METHODS
	void send(Connection);
};

#endif /* COMMUNICATIONENGINE_HPP_ */
