/*
 * ServerStub.hpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tf
 */

#ifndef SERVERSTUB_HPP_
#define SERVERSTUB_HPP_
#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <string.h>

using namespace std;
using namespace jsonrpc;

class StubServer : public AbstractStubServer
{
    public:
        StubServer(AbstractServerConnector &connector);

        virtual void notifyServer();
        virtual std::string sayHello(const std::string& name);
};




#endif /* SERVERSTUB_HPP_ */
