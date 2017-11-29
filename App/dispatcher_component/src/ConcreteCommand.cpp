/*
 * ConcreteCommand.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#include "ConcreteCommand.hpp"
#include "CommunicationEngine.hpp"

CommonRC PublishMsgCMD::execute(string params){
	CommonRC ret = CMN_RC_ERROR;

	LOGMSG(LOG_TRACE, "PublishMsgCMD::execute()");
	ret = static_cast<CommonRC>(CommunicationEngine::getInstance()->publish(params.c_str()));
	return ret;
}

CommonRC ObjectTrackMoveCMD::execute(string params){
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackLossCMD::execute(string params){
	return CMN_RC_SUCCESS;
}
