/*
 * Command.hpp
 *
 *  Created on: Nov 30, 2017
 *      Author: tf
 */

#ifndef COORDCOMMAND_HPP_
#define COORDCOMMAND_HPP_

#include "Command.hpp"
#include "Common.hpp"

#include "ModelEngine.h"


/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class CoordsRcvdCmd: public Command {
private:
std::shared_ptr<ModelEngine> m_model;
public:
	CoordsRcvdCmd(ModelEngine* model): m_model(model){}
	CommonRC execute(std::string params);
	~CoordsRcvdCmd(){};
};




#endif /* COORDCOMMAND_HPP_ */
