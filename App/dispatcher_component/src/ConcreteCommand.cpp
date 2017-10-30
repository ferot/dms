/*
 * ConcreteCommand.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#include "ConcreteCommand.hpp"
#include "CommunicationEngine.hpp"

CommonRC PublishMsgCMD::execute(){
	LOGMSG(LOG_DEBUG, "PublishMsgCMD::execute()");
	CommunicationEngine::getInstance()->publish("EXECUTE CALLBACK", "hellotopic");
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackMoveCMD::execute(){
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackLossCMD::execute(){
	return CMN_RC_SUCCESS;
}
