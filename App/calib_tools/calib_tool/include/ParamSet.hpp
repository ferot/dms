/*
 * ParamSet.hpp
 *
 *  Created on: May 22, 2018
 *      Author: tf
 */

#ifndef PARAMSET_HPP_
#define PARAMSET_HPP_

#include "floatfann.h"
#include "fann_data_cpp.h"
#include "fann_training_data_cpp.h"

struct paramSet {

	// describes general name for output paramSet but also may be used
	// for naming generated network, therefore no extension is assigned
    std::string outputFilename;

    FANN::activation_function_enum m_activationFunOutput;
    FANN::activation_function_enum m_activationFunHidden;
    FANN::training_algorithm_enum m_trainingAlg;

    unsigned int num_input;
    unsigned int num_output;
    unsigned int num_layers;
    unsigned int num_neurons_hidden;

    float activation_steepness_hidden;
    float activation_steepness_output;

    float desired_error;
    unsigned int max_epochs;
    unsigned int epochs_between_reports;
    float learning_rate;

	float getDesiredError() const;
	void setDesiredError(float desiredError);
	unsigned int getEpochsBetweenReports() const;
	void setEpochsBetweenReports(unsigned int epochsBetweenReports);
	float getLearningRate() const;
	void setLearningRate(float learningRate);
	FANN::activation_function_enum getActivationFun(bool hidden) const;
	void setActivationFun(FANN::activation_function_enum activationFun, bool hidden);
	FANN::training_algorithm_enum getTrainingAlg() const;
	void setTrainingAlg(FANN::training_algorithm_enum trainingAlg);
	unsigned int getMaxEpochs() const;
	void setMaxEpochs(unsigned int maxEpochs);
	unsigned int getNumInput() const;
	void setNumInput(unsigned int numInput);
	unsigned int getNumLayers() const;
	void setNumLayers(unsigned int numLayers);
	unsigned int getNumNeuronsHidden() const;
	void setNumNeuronsHidden(unsigned int numNeuronsHidden);
	unsigned int getNumOutput() const;
	void setNumOutput(unsigned int numOutput);
	const std::string& getOutputFilename() const;
	void setOutputFilename(const std::string& outputFilename);
	float getActivationSteepnessHidden() const;
	void setActivationSteepnessHidden(float activationSteepnessHidden);
	float getActivationSteepnessOutput() const;
	void setActivationSteepnessOutput(float activationSteepnessOutput);
};


#endif /* PARAMSET_HPP_ */
