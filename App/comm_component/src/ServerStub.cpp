/*
 * ServerStub.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tf
 */
#include "ServerStub.hpp"
#include <iostream>

StubServer::StubServer(AbstractServerConnector &connector) :
    AbstractStubServer(connector)
{
}
void StubServer::notifyServer()
{
    cout << "Server got notified" << endl;
}
string StubServer::sayHello(const string &name)
{
    return "Hello " + name;
}


