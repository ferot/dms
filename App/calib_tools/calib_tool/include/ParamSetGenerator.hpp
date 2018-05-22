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

typedef std::vector<paramSet> t_paramVec;

class ParamSetGenerator {
public:

	ParamSetGenerator(int setCount);
	const paramSet& getVector();
	std::string generateFilename();
    void incrementID();


private:

	t_paramVec m_setVector;

	// Id of last managed(created) set
	// this is managed ONLY by ParamGenerator !
	int m_lastID;

	// describes nr of sets of params, also determines step in generating values.
	int m_setCount;

	void generateSet();

	CommonRC generateSetName();
	CommonRC saveSetToFile(std::string name, int id);
	CommonRC loadSetsFromFile();

};



#endif /* PARAMSETGENERATOR_HPP_ */
