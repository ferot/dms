/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include <iostream>
#include "logger.h"
//#include "CommunicationEngine.hpp"

#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();
	LOGMSG(LOG_DEBUG, "Starting rpc test tool");
	HttpClient client("http://localhost:8383");
    StubClient c(client);
    try
    {
        cout << c.sayHello("Peter Knafl") << endl;
        c.notifyServer();
    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }
	return 0;
}
