/*
 * Connection.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: tf
 */

#include "Connection.hpp"

/*
 * Constructs connection according to
 */
Connection::Connection(string ip, int port) :
		m_port(port) {
	//TODO: implement https!
	m_ip_address = string("http://" + ip + ":" + to_string(m_port));
	m_client = unique_ptr<HttpClient>(new HttpClient(m_ip_address.c_str()));
	m_clientStub = unique_ptr<StubClient> (new StubClient(*m_client.get()));

	LOGMSG_ARG(LOG_TRACE, "Connection to host with address : %s created.",
			m_ip_address.c_str());
}

Connection::~Connection(){

}
