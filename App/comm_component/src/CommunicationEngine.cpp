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

/*
 * Initializes logger instance
 */
int CommunicationEngine::initLogger() {
	return mylog_init();
}

/*
 * Gets ip address from config file.
 */
CommEngineRetCode CommunicationEngine::obtainServerIP() {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
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

	LOGMSG_ARG(LOG_TRACE, "Server starting with port_num %d", m_server_port);

	m_server = make_shared<HttpServer>(HttpServer(m_server_port));
	m_stubserver_handle = shared_ptr<ServerStub>(
			new ServerStub(*(m_server.get())));

	if (m_stubserver_handle) {
		config->setValue(string("nodes"), string("host"), ip);
		LOGMSG(LOG_DEBUG, "Communication Engine initialized...");
	} else {
		LOGMSG(LOG_ERROR, "Communication Engine initialization failed!");
	}
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
		string id) {
	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	if (id.empty()){
		//not user-friendly format
		id = boost::lexical_cast<string>(boost::uuids::random_generator()());
	}

	m_connections.insert(make_pair(id, new Connection(ip, port)));
	config->setValue("nodes","client-" + id, ip + ":" + numToString(port));

	return ret;
}

/*
 * Prints all connections to logfile.
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
string CommunicationEngine::send(string connectionId) {
//	CommEngineRetCode ret = COMM_ENG_SUCCESS;
	string ret;
	auto search = m_connections.find(connectionId);
	if (search != m_connections.end()) {
		ret = search->second->m_clientStub->sayHello(
				string("hi ") + connectionId);
		search->second->m_clientStub->notifyServer();
	} else {
		LOGMSG_ARG(LOG_WARN, "Couldn't find connection with %s id!", connectionId.c_str());
	}
	return ret;
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
