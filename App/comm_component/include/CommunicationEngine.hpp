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

#include "MQTTClient.h"
#include "MQTTAsync.h"

#include "logger.h"
#include "Config.hpp"

using namespace std;

/*
 * Node Engine Return Code.
 * Represents status of operation
 */
enum ComEnRc {
	COMM_ENG_SUCCESS, COMM_ENG_ERROR
};

/*
 * Class responsible for Communicating with MQTT broker.
 */
class CommunicationEngine {
private:
	MQTTClient m_client;
	MQTTClient_connectOptions m_conn_opts;

	string m_address;
	string m_clientId;
	string m_topic;
	short int m_qos;
	int m_timeout;
	int m_port;

	static CommunicationEngine* m_instance;

	CommunicationEngine(string address = "tcp://localhost", string client = "",
			int port = 1883, int timeout = 10000L, int qos = 1);
	~CommunicationEngine();


	ComEnRc obtainServerIP();
	Config* config;
public:

	static CommunicationEngine* getInstance(string m_address = "", int port = 1883);

	string getBrokerAddr();
	int getPort();

	inline void setTopic(std::string topic) {
		m_topic = topic;
	}

	ComEnRc connect();
	ComEnRc disconnect();
	ComEnRc subscribe(string);
	ComEnRc unsubscribe(string);
	ComEnRc publish(string, string);
};

#endif /* COMMUNICATIONENGINE_HPP_ */
