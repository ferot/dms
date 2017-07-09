/*
 * CommunicationEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef COMMUNICATIONENGINE_HPP_
#define COMMUNICATIONENGINE_HPP_
#include <string>
/*
 * Class responsible for Communicating with nodes
 */
class CommunicationEngine {
private:
	std::string server_ip;
//JSONRPC CLIENT + SERVER STUBS handlers
public:
	//SEND METHODS
void send();
};

#endif /* COMMUNICATIONENGINE_HPP_ */
