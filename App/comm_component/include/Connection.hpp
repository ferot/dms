/*
 * Connection.hpp
 *
 *  Created on: Jul 22, 2017
 *      Author: tf
 */

#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <string.h>
#include <memory>

#include "logger.h"
#include "stubclient.h"

using namespace std;
using namespace jsonrpc;

/*
 * Class implementing Connection abstraction
 */
class Connection {
private:

	int m_port;
	unique_ptr<HttpClient> m_client;
	unique_ptr<StubClient> m_clientStub;
public:
	Connection(string ip, int port);
	~Connection();
	string m_ip_address;

};

#endif /* CONNECTION_HPP_ */
