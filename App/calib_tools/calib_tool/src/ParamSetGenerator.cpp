/*
 * ParamSetGenerator.cpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */
#include <time.h>

#include "ParamSetGenerator.hpp"
#include "calibtool.h"

const int initialSetCount = 1000;

ParamSetGenerator::ParamSetGenerator(Ui::CalibTool* ui) :
		UI(ui), m_lastID(0), m_setCount(0) {
	srand (time(NULL));
}

/**
 * Returns subsequent single param vector essential for creation fann wrapper's net.
 * Note : Modifies m_lastID counter
 * @return
 */
paramSet& ParamSetGenerator::getVector() {
	if (m_lastID > m_setCount) {
		m_lastID = 0;
	}

	return m_setVector->at(m_lastID++);

}

/**
 * Creates single set according to params or range got from UI
 * Note : Modifies m_lastID counter
 */
void ParamSetGenerator::generateSet() {

	m_setCount = getSpinboxInt(UI->spinBox_setCount);
	m_setVector = std::make_shared<std::vector<paramSet>>(
			std::vector<paramSet>(m_setCount, paramSet()));

	std::for_each(m_setVector->begin(), m_setVector->end(),
			[this](paramSet &set) {

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
                m_lastID++;
			});
    // Finished generation let's now allow to get it from the beginning
	m_lastID = 0;
}

/**
 * Generates unique (based on timestamp) filename for param set
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

int ParamSetGenerator::getLastID() {
	return m_lastID;
}

bool ParamSetGenerator::isLastID() {
	return ((m_lastID + 1) == m_setCount);
}
