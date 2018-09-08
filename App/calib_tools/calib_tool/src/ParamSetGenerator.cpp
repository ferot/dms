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

    int neuNum = getSpinboxInt(UI->spinBox_numneur_min);

    std::for_each(m_setVector->begin(), m_setVector->end(),
                  [this, &eng, &neuNum](paramSet &set) {
        std::uniform_int_distribution<> distr(FANN::activation_function_enum::LINEAR, FANN::activation_function_enum::COS_SYMMETRIC);// define the range
        // Arbitrary
        distr = std::uniform_int_distribution<>(100000, 500000);

        set.setMaxEpochs(500000);
        set.setEpochsBetweenReports(1000);
        set.setDesiredError(getSpinboxFloat(UI->doubleSpinBox_desired_error));

        //UI-BASED
        set.setActivationFun(static_cast<FANN::activation_function_enum>(getSpinboxInt(UI->spinBox_fun_hidden_type)), true);
        set.setActivationFun(static_cast<FANN::activation_function_enum>(getSpinboxInt(UI->spinBox_fun_output_type)), false);

        set.setTrainingAlg(static_cast<FANN::training_algorithm_enum>(getSpinboxInt(UI->spinBox_train_alg)));

        // Step mode in neuron number chosen
        if(UI->step_numneu_checkbox->isChecked()) {
            neuNum++;
        } else {
            distr = std::uniform_int_distribution<>(getSpinboxInt(UI->spinBox_numneur_min), getSpinboxInt(UI->spinBox_numneur_max));
            neuNum = distr(eng);
        }
        set.setNumNeuronsHidden(neuNum);

        std::uniform_real_distribution<> distr_real(getSpinboxFloat(UI->doubleSpinBox_learnrate_min), getSpinboxFloat(UI->doubleSpinBox_learnrate_max));
        set.setLearningRate(distr_real(eng));

        distr_real = std::uniform_real_distribution<>(getSpinboxFloat(UI->doubleSpinBox_funhidsteep_min), getSpinboxFloat(UI->doubleSpinBox_funhidsteep_max));
        set.setActivationSteepnessHidden(distr_real(eng));

        distr_real = std::uniform_real_distribution<>(getSpinboxFloat(UI->doubleSpinBox__funoutpsteep_min), getSpinboxFloat(UI->doubleSpinBox_funoutpsteep_max));
        set.setActivationSteepnessOutput(distr_real(eng));

        distr = std::uniform_int_distribution<>(getSpinboxInt(UI->spinBox_numlay_min), getSpinboxInt(UI->spinBox_numlay_max));
        set.setNumLayers(distr(eng));

        set.setOutputFilename(generateFilename());
        incrementID();
    });
	// Finished generation let's now allow to get it from the beginning
	m_lastID = 0;
}

/**
 * Generates unique (based on timestamp) filename for param set
 * @return
 */
std::string ParamSetGenerator::generateFilename() {
    const size_t size = 9;
    char date[size];

    generateTimestamp("%H%M%S_", date, size);
	return std::string(date) + std::to_string(m_lastID);
}

void ParamSetGenerator::incrementID(){
	m_lastID++;
}

int ParamSetGenerator::getLastID() const{
	return m_lastID;
}

bool ParamSetGenerator::isLastID() {
    return ((getLastID() + 1) > m_setCount);
}
