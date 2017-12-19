/*
 * CommunicationEngine.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include <iostream>
#include <utility>
#include <map>

#include "DispatchEngine.hpp"
#include "CommunicationEngine.hpp"
#include "Common.hpp"

CommunicationEngine* CommunicationEngine::m_instance = nullptr;
t_mapTopicEvtCmd CommunicationEngine::m_mapTopEventCmd = {};

volatile MQTTClient_deliveryToken deliveredtoken;

/**
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 *
 * @param address - address of MQTT broker
 * @param client - client's ID
 * @param port - port for session
 * @param timeout - timeout for MQTT communications
 * @param qos - quality of servise QOS
 * @return
 */
CommunicationEngine* CommunicationEngine::getInstance(string client, string address,
		int port, int timeout, int qos) {
	if (!m_instance) {
		m_instance = new CommunicationEngine(address, client, port, timeout, qos);
	}
	return m_instance;
}

/**
 * Constructor of communication engine.
 * Creates fundamental objects essential for MQTT communication
 *
 * Params description as in getInstance
 * @param address
 * @param client
 * @param port
 * @param timeout
 * @param qos
 */
CommunicationEngine::CommunicationEngine(string brokerAddress, string client,
		int port, int timeout, int qos) :
		m_brokerAddress(brokerAddress),
		m_clientId(client),
		m_qos(qos),
		m_timeout(timeout),
		m_port(port)
{
	config = Config::getInstance();

	m_conn_opts = MQTTClient_connectOptions_initializer;
	m_conn_opts.keepAliveInterval = 20;
	m_conn_opts.cleansession = 1;

	if(m_brokerAddress.empty()){
		obtainBrokerAddr();
	} else {
		mergeAddrPort(m_port);
	}
	LOGMSG_ARG(LOG_TRACE, "MQTT full client's address %s", m_brokerAddress.c_str());

	if ((MQTTClient_create(&m_client, m_brokerAddress.c_str(), m_clientId.c_str(),
			MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Client %s creation failed !", m_clientId.c_str());
	} else {
		LOGMSG_ARG(LOG_ERROR, "Client %s creation successfull !", m_clientId.c_str());
	}

	MQTTClient_setCallbacks(m_client, NULL,
			&CommunicationEngine::connlostCallback,
			&CommunicationEngine::msgarrvdCallback,
			&CommunicationEngine::deliveredCallback);

	DispatchEngine::getInstance()->registerEvent(eventType::COMMUNICATION_EVENT);

}

/**
 * Cleans up connection handler as they're allocated manually
 */
CommunicationEngine::~CommunicationEngine() {
	if (disconnect() == COMM_ENG_SUCCESS) {
		MQTTClient_destroy(&m_client);
	} else {
		LOGMSG(LOG_WARN, "Couldn't destroy client!");
	}
}

/**
 * Delivery callback.
 * Assigns delivery token specific for exchange message session.
 * Could be used to session control.
 * @param context
 * @param dt
 */
void CommunicationEngine::deliveredCallback(void *context, MQTTClient_deliveryToken dt)
{
    LOGMSG_ARG(LOG_TRACE, "[deliveredCallback] Message delivery with token value %d confirmed", dt);
    deliveredtoken = dt;
}

/**
 * Message Arrived callback.
 * Retrieves
 * @param context - application specific context.
 * @param topicName
 * @param topicLen
 * @param message - payload message
 * @return - true for success
 */
int CommunicationEngine::msgarrvdCallback(void *context, char *topicName,
		int topicLen, MQTTClient_message *message) {
	LOGMSG_ARG(LOG_TRACE, "[msgarrvdCallback] Message with topic : %s arrived", topicName);

	string topic = string(topicName);
	CommunicationEngine::enqueueEvt(topic, (char*)(message->payload));

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

void CommunicationEngine::enqueueEvt(string topic, string payload) {
	auto it = m_mapTopEventCmd.find(std::string(topic));

	t_eventPtr event = std::make_shared<Event>(it->second.first, payload);

	DispatchEngine::getInstance()->enqueueEvent(event);
}

/**
 * Connection lost callback.
 * @param context
 * @param cause
 */
void CommunicationEngine::connlostCallback(void *context, char *cause)
{
    LOGMSG_ARG(LOG_ERROR, "[connlostCallback] Connection lost. Cause : ", cause);
    //TODO: handle such situation (Notify)
}

/**
 * Gets broker's address from config file.
 *
 * @return COMM_ENG_SUCCESS on success. COMM_ENG_ERROR on error.
 */
ComEnRc CommunicationEngine::obtainBrokerAddr() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	m_brokerAddress = config->getValue("MQTT", "broker_address");
	m_port = stoi(config->getValue("MQTT", "broker_port"));

	mergeAddrPort(m_port);

	if (m_brokerAddress.empty() || m_port == 0) {
		LOGMSG(LOG_ERROR, "Cannot get broker address from config file! Setting to default tcp://localhost...");
		m_brokerAddress = "tcp://localhost";
		ret = COMM_ENG_ERROR;
	}
	return ret;
}

/**
 * Returns address associated with broker.
 *
 * @return
 */
string CommunicationEngine::getBrokerAddr() {
	return m_brokerAddress;
}

/**
 * Returns server port associated with http server
 *
 * @return
 */
int CommunicationEngine::getPort() {
	return m_port;
}

/**
 * Start listening for messages @topic.
 *
 * @param topic
 * @return COMM_ENG_SUCCESS on success. COMM_ENG_ERROR on error.
 */
ComEnRc CommunicationEngine::subscribe(string topic, eventType type, t_commandPtr cmdPtr) {
	ComEnRc ret = COMM_ENG_SUCCESS;
	m_topic = topic;
	if (MQTTClient_subscribe(m_client, m_topic.c_str(), m_qos) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't subscribe to topic %s !", topic.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Subscribing to topic %s...", topic.c_str());
		m_mapTopEventCmd.insert(
				std::pair<std::string, t_p_evtTypeCmd>(topic,
						t_p_evtTypeCmd(type, cmdPtr)));

		DispatchEngine::getInstance()->registerEvent(eventType::USER_EVENT, cmdPtr);

	}
	return ret;
}

/**
 *  * Stops subscription for messages @topic
 *
 * @param topic
 * @return COMM_ENG_SUCCESS on success. COMM_ENG_ERROR on error.
 */
ComEnRc CommunicationEngine::unsubscribe(string topic) {
	ComEnRc ret = COMM_ENG_SUCCESS;

	if (MQTTClient_unsubscribe(m_client, topic.c_str()) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't unsubscribe from topic %s !", topic.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Unsubscribing from topic %s...", topic.c_str());
	}
	return ret;
}

/**
 * Connects to MQTT broker.
 *
 * @return COMM_ENG_SUCCESS on success. COMM_ENG_ERROR on error.
 */
ComEnRc CommunicationEngine::connect() {
	ComEnRc ret = COMM_ENG_SUCCESS;
	if ((MQTTClient_connect(m_client, &m_conn_opts)) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't connect to broker %s !", m_brokerAddress.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Connected to broker %s...", m_brokerAddress.c_str());
	}
	return ret;
}

/**
 * Disconnects client.
 *
 * @return COMM_ENG_SUCCESS on success. COMM_ENG_ERROR on error.
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

/**
 * Method publishing message to MQTT broker.
 * Prepares MQTT communication structure and attempts to send it.
 * Obtains unique token for this concrete publishing session.
 *
 * @param message - payload to be sent
 * @param topic
 * @return ComEnRc ret code.
 */
ComEnRc CommunicationEngine::publish(string message, string topic) {
	ComEnRc ret = COMM_ENG_SUCCESS;
	string top = topic.empty() ? m_topic : topic;

	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	pubmsg.payload = (void*)message.c_str();
	pubmsg.payloadlen = message.length();
	pubmsg.qos = m_qos;
	pubmsg.retained = 0;

	MQTTClient_deliveryToken token;

	if (MQTTClient_publishMessage(m_client, top.c_str(), &pubmsg, &token) != MQTTCLIENT_SUCCESS) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't pulish message to topic %s !", top.c_str());
		ret = COMM_ENG_ERROR;
	} else {
		LOGMSG_ARG(LOG_DEBUG, "Message published successfully with token [%d]", (int)token);
	}
	return ret;
}

/**
 * Merges member address string value with port
 * @param port
 */
void CommunicationEngine::mergeAddrPort(int port){
	m_brokerAddress+=(":" + std::to_string(port));
}


std::string CommunicationEngine::getTopic(){
	return m_topic;
}
