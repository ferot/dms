/*
 * ConcreteCommand.hpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#ifndef CONCRETECOMMAND_HPP_
#define CONCRETECOMMAND_HPP_

#include "Command.hpp"
#include "Common.hpp"

/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class PublishMsgCMD: public Command {
public:
	CommonRC execute(/*TODO:params (json?)*/);
	~PublishMsgCMD(){};
};

/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class ObjectTrackLossCMD: public Command {
public:
	CommonRC execute(/*TODO:params (json?)*/);
	~ObjectTrackLossCMD(){};
};

/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class ObjectTrackMoveCMD: public Command {
public:
	CommonRC execute(/*TODO:params (json?)*/);
	~ObjectTrackMoveCMD(){};
};

#endif /* CONCRETECOMMAND_HPP_ */
