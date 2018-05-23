/*
 * ParamSetGenerator.cpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */
#include <time.h>

#include "ParamSetGenerator.hpp"
#include "calibtool.h"

const int initialSetCount = 100;

ParamSetGenerator::ParamSetGenerator(int setCount) :
		m_lastID(0), m_setCount(setCount) {
	srand (time(NULL));

	m_setVector.reserve(initialSetCount);

}

/**
 * Returns subsequent single param vector essential for creation fann wrapper's net.
 * Note : Increments counter
 * @return
 */
const paramSet& ParamSetGenerator::getVector() {
	if (m_lastID > m_setCount) {
		m_lastID = 0;
	}

	return m_setVector[m_lastID];

}

/**
 * Creates single set according to params or range got from UI
 */
void ParamSetGenerator::generateSet() {

	std::for_each(m_setVector.begin(), m_setVector.end(), [this](paramSet set){

		//Rand
		set.setActivationFun(static_cast<FANN::activation_function_enum>(rand() % 16), true);
		set.setActivationFun(static_cast<FANN::activation_function_enum>(rand() % 16), false);
		set.setTrainingAlg(static_cast<FANN::training_algorithm_enum>(rand() % 5));

		// Arbitrary
		set.setMaxEpochs(rand() % 500000 + 100000);
		set.setEpochsBetweenReports(1000);

		//UI
		set.setNumNeuronsHidden(getSpinboxInt(UI->spinBox_numneur_max));
		set.setDesiredError(getSpinboxFloat(UI->doubleSpinBox_desired_error));
		set.setLearningRate(getSpinboxFloat(UI->doubleSpinBox_learnrate_max));
		set.setActivationSteepnessHidden(getSpinboxFloat(UI->doubleSpinBox_funhidsteep_max));
		set.setActivationSteepnessOutput(getSpinboxFloat(UI->doubleSpinBox_funoutpsteep_max));
		set.setNumLayers(getSpinboxInt(UI->spinBox_numlay_max));

		set.setOutputFilename(generateFilename());
	});

}

/**
 * Generates unique filename for param set to be stored
 * @return
 */
std::string ParamSetGenerator::generateFilename() {
	char date[9];
	time_t t = time(0);
	struct tm *tm;

	tm = gmtime(&t);
	strftime(date, sizeof(date), "%H%M%S_", tm);
	return std::string(date) + std::to_string(m_lastID);
}

void ParamSetGenerator::incrementID(){
	m_lastID++;
}

