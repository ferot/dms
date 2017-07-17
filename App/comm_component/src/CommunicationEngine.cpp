/*
 * CommunicationEngine.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "CommunicationEngine.hpp"
#include <iostream>

int CommunicationEngine::initLogger() {
	return mylog_init();
}

string CommunicationEngine::getServerIp() {
	return m_server_ip;
}
int CommunicationEngine::getServerPort() {
	return m_server_port;
}

CommunicationEngine::CommunicationEngine(string ip, int port) :
		m_server_ip(ip), m_server_port(port) {
	if (!initLogger())
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	else
		printf("logger failed");

	HttpServer httpserver(m_server_port);
	ServerStub server_handle = ServerStub(httpserver);
	this->m_server_handle = &server_handle;

	LOGMSG(LOG_DEBUG, "Communication Engine initialized...");
}

void CommunicationEngine::send() {
	std::cout << "helloworld";
}

CommEngineRetCode CommunicationEngine::startServer() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	LOGMSG(LOG_DEBUG, "Starting server...");

	if (!m_server_handle->StartListening()) {
		ret = COMM_ENG_ERROR;
	}
	return ret;
}

CommEngineRetCode CommunicationEngine::stopServer() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	if (!m_server_handle->StopListening()) {
		ret = COMM_ENG_ERROR;
	}
	return ret;
}
