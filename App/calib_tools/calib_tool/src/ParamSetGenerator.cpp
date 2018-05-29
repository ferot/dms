/*
 * ParamSetGenerator.cpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */
#include <time.h>
#include <random>

#include "ParamSetGenerator.hpp"
#include "calibtool.h"

const int initialSetCount = 1000;

ParamSetGenerator::ParamSetGenerator(Ui::CalibTool* ui) :
		UI(ui), m_lastID(0), m_setCount(0) {
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

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator

	m_setCount = getSpinboxInt(UI->spinBox_setCount);
	m_setVector = std::make_shared<std::vector<paramSet>>(
			std::vector<paramSet>(m_setCount, paramSet()));

	std::for_each(m_setVector->begin(), m_setVector->end(),
			[this, &eng](paramSet &set) {

				//Rand
				std::uniform_int_distribution<> distr(FANN::LINEAR, FANN::COS_SYMMETRIC);// define the range

				set.setActivationFun(static_cast<FANN::activation_function_enum>(distr(eng)), true);
				set.setActivationFun(static_cast<FANN::activation_function_enum>(distr(eng)), false);

				distr = std::uniform_int_distribution<>(FANN::TRAIN_INCREMENTAL, FANN::TRAIN_SARPROP);
				set.setTrainingAlg(static_cast<FANN::training_algorithm_enum>(distr(eng)));

				// Arbitrary
				distr = std::uniform_int_distribution<>(100000, 500000);

				set.setMaxEpochs(distr(eng));
				set.setEpochsBetweenReports(1000);
				set.setDesiredError(getSpinboxFloat(UI->doubleSpinBox_desired_error));

				//UI

				distr = std::uniform_int_distribution<>(getSpinboxInt(UI->spinBox_numneur_min), getSpinboxInt(UI->spinBox_numneur_max));
				set.setNumNeuronsHidden(distr(eng));

				std::uniform_real_distribution<> distr_real(getSpinboxFloat(UI->doubleSpinBox_learnrate_min), getSpinboxFloat(UI->doubleSpinBox_learnrate_max));
				set.setLearningRate(distr_real(eng));

				distr_real = std::uniform_real_distribution<>(getSpinboxFloat(UI->doubleSpinBox_funhidsteep_min), getSpinboxFloat(UI->doubleSpinBox_funhidsteep_max));
				set.setActivationSteepnessHidden(distr_real(eng));

				distr_real = std::uniform_real_distribution<>(getSpinboxFloat(UI->doubleSpinBox__funoutpsteep_min), getSpinboxFloat(UI->doubleSpinBox_funoutpsteep_max));
				set.setActivationSteepnessOutput(distr_real(eng));

				distr = std::uniform_int_distribution<>(getSpinboxInt(UI->spinBox_numlay_max), getSpinboxInt(UI->spinBox_numlay_min));
				set.setNumLayers(distr(eng));

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
