/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "main.hpp"
#include "CommunicationEngine.hpp"
//#include "../../common/include/logger.h"

using namespace jsonrpc;
using namespace std;

int main() {
	mylog_init();
	LOGMSG(LOG_DEBUG, "Starting core_app main!");
	CommunicationEngine ce("dupa", 1);
	return 0;
}
