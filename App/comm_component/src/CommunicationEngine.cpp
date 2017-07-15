/*
 * CommunicationEngine.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "CommunicationEngine.hpp"
#include <iostream>

string CommunicationEngine::getServerIp() {
	return m_server_ip;
}
int CommunicationEngine::getServerPort() {
	return m_server_port;
}

CommunicationEngine::CommunicationEngine(string ip, int port) :
		m_server_ip(ip), m_server_port(port) {
}

void CommunicationEngine::send() {
	std::cout << "helloworld";
}
