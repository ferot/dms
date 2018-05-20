/*
 * ParamSetGenerator.hpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */

#ifndef PARAMSETGENERATOR_HPP_
#define PARAMSETGENERATOR_HPP_

#include "Common.hpp"
#include "fann_cpp.h"

typedef std::vector<paramSet> t_paramVec;

struct paramSet {
    std::string outputFilename;
    std::string inputFilename;

    FANN::activation_function_enum m_activationFun;
    FANN::training_algorithm_enum m_trainingAlg;
    unsigned int num_input;
    unsigned int num_output;
    unsigned int num_layers;
    unsigned int num_neurons_hidden;
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
	FANN::activation_function_enum getActivationFun() const;
	void setActivationFun(FANN::activation_function_enum activationFun);
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
};

class ParamSetGenerator {
public:

	ParamSetGenerator(int setCount);
	const paramSet& getVector() const;


private:

	t_paramVec m_setVector;

	// describes nr of sets of params, also determines step in generating values.
	int m_setCount;

	// Id of last managed(created) set
	// this is managed ONLY by ParamGenerator !
	int m_lastID;

	t_paramVec generateSet();

	CommonRC generateSetName();
	CommonRC saveSetToFile(std::string name, int id);
	CommonRC loadSetsFromFile();

};



#endif /* PARAMSETGENERATOR_HPP_ */
