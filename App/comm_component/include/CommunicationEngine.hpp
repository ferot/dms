/*
 * CommunicationEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef COMMUNICATIONENGINE_HPP_
#define COMMUNICATIONENGINE_HPP_
#include <string>
#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace jsonrpc;
using namespace std;

/*
 * Class responsible for Communicating with nodes
 */
class CommunicationEngine {
private:
	string m_server_ip;
	int m_server_port;
public:
	string getServerIp();
	int getServerPort();
	CommunicationEngine(string ip, int port);
	//SEND METHODS
	void send();
};

#endif /* COMMUNICATIONENGINE_HPP_ */
