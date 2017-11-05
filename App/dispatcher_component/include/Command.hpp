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
	virtual CommonRC execute(std::string params) = 0;
	virtual ~Command(){};
};

#endif /* COMMAND_HPP_ */
