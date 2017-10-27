/*
 * ConcreteCommand.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#include "ConcreteCommand.hpp"


CommonRc PublishMsgCMD::execute(){
	return CMN_RC_SUCCESS;
}

CommonRc ObjectTrackMoveCMD::execute(){
	return CMN_RC_SUCCESS;
}

CommonRc ObjectTrackLossCMD::execute(){
	return CMN_RC_SUCCESS;
}
