/*
 * NodeManager.hpp
 *
 *  Created on: Sep 20, 2017
 *      Author: tf
 */

#ifndef NODEMANAGER_HPP_
#define NODEMANAGER_HPP_

#include <vector>

typedef NodeVec std::vector<NodeEntry>;

enum NodeEngineRetCode {
	NODE_ENG_SUCCESS,
	NODE_ENG_ERROR
};

enum NodeState {
	ACTIVE,
	DISABLED,
	NOT_RESPONDING
};

struct NodeEntry {
	std::string ip;
	int port;
	NodeState state;
};

typedef std::vector<NodeEntry> NodeVec;


/*
 * Class responsible for node information management.
 * Stores knowledge about nodes in system.
 * Synchronizes state of nodes with each other on demand.
 */
class NodeEngine {
	NodeVec nodes;
public:
	NodeEngineRetCode getNodeList();
	NodeEngineRetCode setEntry(NodeEntry);
	NodeEngineRetCode refreshNodeList();
};

#endif /* NODEMANAGER_HPP_ */
