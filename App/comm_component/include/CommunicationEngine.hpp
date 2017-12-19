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
#include "Command.hpp"

/**
 * Node Engine Return Code.
 * Represents status of operation
 */
enum ComEnRc {
	COMM_ENG_SUCCESS, COMM_ENG_ERROR
};

typedef std::map<std::string, std::pair<eventType, t_commandPtr>> t_mapTopicEvtCmd;
typedef std::pair<eventType, t_commandPtr> t_p_evtTypeCmd;
/**
 * Class responsible for Communicating with MQTT broker.
 */
class CommunicationEngine {
private:

	static CommunicationEngine* m_instance;
	Config* config;

	MQTTClient m_client;
	MQTTClient_connectOptions m_conn_opts;

	std::string m_brokerAddress;
	std::string m_clientId;
	std::string m_topic;
	short int m_qos;
	int m_timeout;
	int m_port;

	t_commandPtr m_comm;

	static t_mapTopicEvtCmd m_mapTopEventCmd;

	CommunicationEngine(string, string, int, int, int);
	~CommunicationEngine();

	ComEnRc obtainBrokerAddr();
	void mergeAddrPort(int port);

	t_eventPtr prepareEvent(t_eventPtr msgRcvdEvent);
	static void enqueueEvt(string topic, string payload);

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
			std::string client = "examplecl",
			std::string brokerAddress = "",
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
	ComEnRc subscribe(std::string, eventType = eventType::DEFAULT_EVENT, t_commandPtr ptr = nullptr);
	ComEnRc unsubscribe(std::string);
	ComEnRc publish(std::string msg, std::string topic = "");
};

#endif /* COMMUNICATIONENGINE_HPP_ */
