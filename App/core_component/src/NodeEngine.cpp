/*
 * NodeManager.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: tf
 */
#include "NodeEngine.hpp"

NodeEngine* NodeEngine::m_instance = nullptr;

/*
 * Default constructor.
 * Initializes fundamental members and
 * obtains instance to config component.
 */
NodeEngine::NodeEngine(){
	config = Config::getInstance();
}

/*
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 */
NodeEngine* NodeEngine::getInstance() {
	if (!m_instance) {
		m_instance = new NodeEngine();
	}
	return m_instance;
}

/*
 * Gets list of nodes from conf file or
 * in case of problems contacting with main node.
 */
NodEnRc NodeEngine::getNodeList() {
	string dupa = config->getSection("nodes");
	LOGMSG_ARG(LOG_DEBUG, " get section %s", dupa.c_str());
}

/*
 * Sets entry in node list
 */
NodEnRc NodeEngine::setEntry(NodeEntry) {

}

/*
 * Refreshes list of nodes by fetching from conf file or
 * in case of problems contacting with main node.
 */
NodEnRc NodeEngine::refreshNodeList() {

}
