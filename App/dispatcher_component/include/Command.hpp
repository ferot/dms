/*
 * Command.hpp
 *
 *  Created on: Oct 27, 2017
 *      Author: tf
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "Common.hpp"
/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class Command
{
public:
	virtual CommonRC execute(/*TODO:params (json?)*/) = 0;
	virtual ~Command() = 0;
};

#endif /* COMMAND_HPP_ */