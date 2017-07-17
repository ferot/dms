/*
 * ServerStub.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tf
 */
#include "ServerStub.hpp"
#include <iostream>

ServerStub::ServerStub(AbstractServerConnector &connector) :
    AbstractStubServer(connector)
{
}
void ServerStub::notifyServer()
{
    cout << "Server got notified" << endl;
}
string ServerStub::sayHello(const string &name)
{
    return "Hello " + name;
}


