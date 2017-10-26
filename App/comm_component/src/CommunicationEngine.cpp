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
		m_instance = new CommunicationEngine();
	}
	return m_instance;
}

/*
 * Constructor of communication engine.
 * Creates fundamental objects essential for MQTT communication
 */
CommunicationEngine::CommunicationEngine(string address, string client,
		int port, int timeout, int qos) :
		m_address(address), m_clientId(client), m_port(port), m_timeout(
				timeout), m_qos(qos) {

	config = Config::getInstance();

	m_conn_opts = MQTTClient_connectOptions_initializer;
	m_conn_opts.keepAliveInterval = 20;
	m_conn_opts.cleansession = 1;

	MQTTClient_create(&m_client, m_address.c_str(), m_clientId.c_str(),
	MQTTCLIENT_PERSISTENCE_NONE, NULL);

	//MQTTClient_setCallbacks(m_client, NULL, connlost, msgarrvd, delivered);

}

/*
 * Cleans up connection handler as they're allocated manually
 */
CommunicationEngine::~CommunicationEngine() {
	if (disconnect() == COMM_ENG_SUCCESS) {
		MQTTClient_destroy(&m_client);
	} else {
		LOGMSG(LOG_WARN, "Couldn't destroy client!");
	}
}

/*
 * Gets broker's address from config file.
 */
ComEnRc CommunicationEngine::obtainServerIP() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	m_address = config->getValue("nodes", "broker_address");

	if (m_address.empty()) {
		LOGMSG(LOG_ERROR, "Cannot get broker address from config file!");
		ret = COMM_ENG_ERROR;
	}
	return ret;
}

/*
 * Returns address associated with broker.
 */
string CommunicationEngine::getBrokerAddr() {
	return m_address;
}

/*
 * Returns server port associated with http server
 */
int CommunicationEngine::getPort() {
	return m_port;
}

/*
 * Start listening for messages @topic.
 */
ComEnRc CommunicationEngine::subscribe(string topic) {
	ComEnRc ret = COMM_ENG_SUCCESS;
	if (MQTTClient_subscribe(m_client, m_topic.c_str(),
			m_qos) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't subscribe to topic %s !",
				topic.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Subscribing to topic %s...", topic.c_str());
	}
	return ret;
}

/*
 * Stops listening for messages @topic
 */
ComEnRc CommunicationEngine::unsubscribe(string topic) {
	ComEnRc ret = COMM_ENG_SUCCESS;
	LOGMSG(LOG_DEBUG, "Stopping http server...");
	return ret;
}

/*
 * Connects to MQTT broker.
 */
ComEnRc CommunicationEngine::publish(string msg, string topic) {
	ComEnRc ret = COMM_ENG_SUCCESS;
	if ((MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't connect to broker %s !",
				m_address.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Connecting to broker %s...", m_address.c_str());
	}
	return ret;
}
/*
 * Disconnects client.
 */
ComEnRc CommunicationEngine::disconnect() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	if (MQTTClient_disconnect(m_client, 10000) != MQTTCLIENT_SUCCESS) {
		LOGMSG(LOG_ERROR, "Client disconnect failure!");
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG(LOG_DEBUG, "client disconnected successfully...");
	}
	return ret;
}

