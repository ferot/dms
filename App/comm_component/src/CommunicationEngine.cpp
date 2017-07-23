/*
 * CommunicationEngine.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "CommunicationEngine.hpp"
#include <iostream>
#include <utility>
#include <map>
/*
 * Initializes logger instance
 */
int CommunicationEngine::initLogger() {
	return mylog_init();
}

/*
 * Gets ip address from OS
 */
CommEngineRetCode CommunicationEngine::obtainServerIP() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	//TODO: obtain iface address
	//m_server_ip = ip;
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
 * Constructor of communication engine.
 * Creates fundamental objects essential for RPC communication
 */
CommunicationEngine::CommunicationEngine(string ip, int port) :
		m_server_ip(ip), m_server_port(port) {

	if (!initLogger())
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	else
		printf("Logger init failed");

	LOGMSG_ARG(LOG_TRACE, "Server starting with port_num %d", m_server_port);
	m_server = make_shared<HttpServer>(HttpServer(m_server_port));
	m_stubserver_handle = shared_ptr<ServerStub>(
			new ServerStub(*(m_server.get())));

	LOGMSG(LOG_DEBUG, "Communication Engine initialized...");
}

/*
 * Cleans up connections handlers as they're allocated manually
 */
CommunicationEngine::~CommunicationEngine() {
	for (auto connect_handler : m_connections) {
		delete connect_handler.second;
	}
}

/*
 * Add connection.
 * Inserts entry to connections map.
 * If id is not provided, generates unique
 */
CommEngineRetCode CommunicationEngine::addConnection(string ip, int port,
		string id = nullptr) {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	if (id.empty())
		id = "not_assigned_id";

	m_connections.insert(make_pair(id, new Connection(ip, port)));
	return ret;
}

/*
 * Prints all connections
 */
void CommunicationEngine::printConnections() {
	for (auto it : m_connections) {
		LOGMSG_ARG(LOG_DEBUG, "Connection id: %s", (it.first).c_str());
		LOGMSG_ARG(LOG_DEBUG, "ip: %s \n", (it.second->m_ip_address).c_str());

	}
}
/*
 * Send event to host via specific connection
 */
void CommunicationEngine::send(Connection c) {
	//TODO: send event to host via connetc
	std::cout << "helloworld";
}

/*
 * Start listening server for RPC commands
 */
CommEngineRetCode CommunicationEngine::startServer() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	LOGMSG(LOG_DEBUG, "Starting http server...");

	if (!m_stubserver_handle->StartListening()) {
		ret = COMM_ENG_ERROR;
	}
	return ret;
}

/*
 * Stops listening server for RPC commands
 */
CommEngineRetCode CommunicationEngine::stopServer() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	if (!m_stubserver_handle->StopListening()) {
		ret = COMM_ENG_ERROR;
		LOGMSG(LOG_ERROR, "Error while stopping http server...");
	}
	LOGMSG(LOG_DEBUG, "Stopping http server...");
	return ret;
}
