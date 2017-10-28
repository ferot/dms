/*
 * ConcreteCommand.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#include "ConcreteCommand.hpp"


CommonRC PublishMsgCMD::execute(){
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackMoveCMD::execute(){
	return CMN_RC_SUCCESS;
}

CommonRC ObjectTrackLossCMD::execute(){
	return CMN_RC_SUCCESS;
}
