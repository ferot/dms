/*
 * ParamSetGenerator.hpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */

#ifndef PARAMSETGENERATOR_HPP_
#define PARAMSETGENERATOR_HPP_




#include "Common.hpp"
#include "ParamSet.hpp"

namespace Ui {
class CalibTool;
}

typedef std::shared_ptr<std::vector<paramSet>> t_p_paramVec;

class ParamSetGenerator {
public:

	ParamSetGenerator(Ui::CalibTool* ui);
	paramSet& getVector();
	std::string generateFilename();
    void incrementID();
	void generateSet();
    int getLastID() const;
	bool isLastID();

private:
    Ui::CalibTool* UI;

	t_p_paramVec m_setVector;

	// Id of last managed(created) set
	// this is managed ONLY by ParamGenerator !
	int m_lastID;

	// describes nr of sets of params, also determines step in generating values.
	int m_setCount;

	CommonRC generateSetName();
	CommonRC saveSetToFile(std::string name, int id);
	CommonRC loadSetsFromFile();

};



#endif /* PARAMSETGENERATOR_HPP_ */
