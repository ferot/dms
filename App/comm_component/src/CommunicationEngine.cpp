/*
 * CommunicationEngine.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include <iostream>
#include <utility>
#include <map>

#include "CommunicationEngine.hpp"
#include "Common.hpp"

CommunicationEngine* CommunicationEngine::m_instance = nullptr;

/*
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 */
CommunicationEngine* CommunicationEngine::getInstance(string ip, int port) {
	if (!m_instance) {
		m_instance = new CommunicationEngine(ip, port);
	}
	return m_instance;
}

/*
 * Constructor of communication engine.
 * Creates fundamental objects essential for RPC communication
 */
CommunicationEngine::CommunicationEngine(string ip, int port) :
		m_server_ip(ip), m_server_port(port) {

	config = Config::getInstance();

	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}

}

/*
 * Cleans up connections handlers as they're allocated manually
 */
CommunicationEngine::~CommunicationEngine() {

}

/*
 * Initializes logger instance
 */
int CommunicationEngine::initLogger() {
	return mylog_init();
}

/*
 * Gets ip address from config file.
 */
ComEnRc CommunicationEngine::obtainServerIP() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	m_server_ip = config->getValue("nodes", "host");

	if (m_server_ip.empty()) {
		LOGMSG(LOG_ERROR, "Cannot get addres from config file!");
		ret = COMM_ENG_ERROR;
	}
	return ret;
}

/*
 * Returns server ip associated http server
 */
string CommunicationEngine::getServerIp() {
	return m_server_ip;
}

/*
 * Returns server port associated with http server
 */
int CommunicationEngine::getServerPort() {
	return m_server_port;
}


/*
 * Start listening server for RPC commands
 */
ComEnRc CommunicationEngine::startServer() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	LOGMSG(LOG_DEBUG, "Starting http server...");
	return ret;
}

/*
 * Stops listening server for RPC commands
 */
ComEnRc CommunicationEngine::stopServer() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	LOGMSG(LOG_DEBUG, "Stopping http server...");
	return ret;
}
