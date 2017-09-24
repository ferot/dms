/*
 * NodeManager.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: tf
 */
#include "NodeEngine.hpp"

NodeEngine* NodeEngine::m_instance = nullptr;

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
 * Default constructor.
 * Initializes fundamental members and
 * obtains instance to config component.
 */
NodeEngine::NodeEngine() {
	config = Config::getInstance();
}

/*
 * Gets list of nodes from conf file or
 * in case of problems contacting with main node.
 */
NodEnRc NodeEngine::getNodeList() {
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
