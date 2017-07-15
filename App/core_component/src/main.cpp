/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "main.hpp"
#include "CommunicationEngine.hpp"

using namespace jsonrpc;
using namespace std;

int main() {
	CommunicationEngine ce(string("localhost"), 8888);
	ce.send();
	return 0;
}
