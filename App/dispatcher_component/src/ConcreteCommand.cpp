/*
 * ConcreteCommand.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#include "ConcreteCommand.hpp"
#include "CommunicationEngine.hpp"

CommonRC PublishMsgCMD::execute(string params){
	LOGMSG(LOG_DEBUG, "PublishMsgCMD::execute()");
	CommunicationEngine::getInstance()->publish(params.c_str(), "hellotopic");
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackMoveCMD::execute(string params){
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackLossCMD::execute(string params){
	return CMN_RC_SUCCESS;
}
