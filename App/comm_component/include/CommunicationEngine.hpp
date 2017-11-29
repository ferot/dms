/*
 * CommunicationEngine.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */

#ifndef COMMUNICATIONENGINE_HPP_
#define COMMUNICATIONENGINE_HPP_
#include <string>
#include <log4c.h>
#include <memory>
#include <map>

extern "C" {
#include "MQTTClient.h"
#include "MQTTAsync.h"
}

#include "DispatchEngine.hpp"
#include "Event.hpp"
#include "logger.h"
#include "Config.hpp"


/**
 * Node Engine Return Code.
 * Represents status of operation
 */
enum ComEnRc {
	COMM_ENG_SUCCESS, COMM_ENG_ERROR
};

/**
 * Class responsible for Communicating with MQTT broker.
 */
class CommunicationEngine {
private:

	static CommunicationEngine* m_instance;
	Config* config;

	MQTTClient m_client;
	MQTTClient_connectOptions m_conn_opts;

	std::string m_address;
	std::string m_clientId;
	std::string m_topic;
	short int m_qos;
	int m_timeout;
	int m_port;

	std::map<std::string, eventType> m_mapEvent;

	CommunicationEngine(string, string, int, int, int);
	~CommunicationEngine();

	ComEnRc obtainBrokerAddr();
	void mergeAddrPort(int port);

	t_eventPtr prepareEvent(t_eventPtr msgRcvdEvent);

	/*
	 * MQTT callbacks for Async communication.
	 */
	static void deliveredCallback(void *context, MQTTClient_deliveryToken dt);
	static void connlostCallback(void *context, char *cause);
	static int msgarrvdCallback(void *context,
			char *topicName,
			int topicLen,
			MQTTClient_message *message);

public:

	static CommunicationEngine* getInstance(
			std::string address = "tcp://localhost",
			std::string client = "examplecl",
			int port = 1883,
			int timeout = 10000L,
			int qos = 1);

	std::string getBrokerAddr();
	int getPort();
	std::string getTopic();

	inline void setTopic(std::string topic) {
		m_topic = topic;
	}

	ComEnRc connect();
	ComEnRc disconnect();
	ComEnRc subscribe(std::string, t_eventPtr = nullptr);
	ComEnRc unsubscribe(std::string);
	ComEnRc publish(std::string msg, std::string topic = "");
};

#endif /* COMMUNICATIONENGINE_HPP_ */
