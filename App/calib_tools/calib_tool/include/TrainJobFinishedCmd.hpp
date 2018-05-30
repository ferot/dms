/*
 * TrainJobFinishedCmd.hpp
 *
 *  Created on: Nov 30, 2017
 *      Author: tf
 */

#ifndef TRNJOBCOMMAND_HPP_
#define TRNJOBCOMMAND_HPP_

#include "Command.hpp"
#include "Common.hpp"
#include "calibtool.h"

/**
 * Abstract representation of command object.
 * Describes interface for inheriting commands.
 */
class TrainJobFinishedCmd: public Command {
private:
	CalibTool* m_ctInstance;

public:
	TrainJobFinishedCmd(){}
	TrainJobFinishedCmd(CalibTool* ct){
		m_ctInstance = ct;
	}
	CommonRC execute(std::string params);
	~TrainJobFinishedCmd(){};
};




#endif /* TRNJOBCOMMAND_HPP_ */
