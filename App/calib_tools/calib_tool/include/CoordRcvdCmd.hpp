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
#include "calibtool.h"

/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class CoordsRcvdCmd: public Command {
private:
	CalibTool* m_ctInstance;

public:
	CoordsRcvdCmd(){}
	CoordsRcvdCmd(CalibTool* ct){
		m_ctInstance = ct;
	}
	CommonRC execute(std::string params);
	~CoordsRcvdCmd(){};
};




#endif /* COORDCOMMAND_HPP_ */
