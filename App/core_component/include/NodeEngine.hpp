/*
 * NodeManager.hpp
 *
 *  Created on: Sep 20, 2017
 *      Author: tf
 */

#ifndef NODEMANAGER_HPP_
#define NODEMANAGER_HPP_

#include <vector>
#include <string>

/*
 * Node Engine Return Code.
 * Represents status of operation
 */
enum NodEnRc {
	NODE_ENG_SUCCESS,
	NODE_ENG_ERROR
};

/*
 * Node State.
 * Represents [in memory] state of node.
 */
enum NodeState {
	ACTIVE,
	DISABLED,
	NOT_RESPONDING
};

/*
 * Node Entry.
 * Represents actual status of node.
 * Consists of few fields which should be filled.
 */
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
	NodEnRc getNodeList();
	NodEnRc setEntry(NodeEntry);
	NodEnRc refreshNodeList();
};

#endif /* NODEMANAGER_HPP_ */
