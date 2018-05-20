/*
 * ParamSetGenerator.cpp
 *
 *  Created on: May 20, 2018
 *      Author: tf
 */

#include "ParamSetGenerator.hpp"

ParamSetGenerator::ParamSetGenerator(int setCount) :
		m_lastID(0), m_setCount(setCount) {

}

/**
 * Returns subsequent single param vector essential for creation fann wrapper's net
 * @return
 */
const paramSet& ParamSetGenerator::getVector() const {
	return (m_lastID < m_setCount) ? m_setVector[m_lastID++] : nullptr;
}

t_paramVec ParamSetGenerator::generateSet() {

	std::for_each(m_setVector.begin(), m_setVector.end(), [](paramSet set){
		set.
	});

}

float paramSet::getDesiredError() const {
	return desired_error;
}

void paramSet::setDesiredError(float desiredError) {
	desired_error = desiredError;
}

unsigned int paramSet::getEpochsBetweenReports() const {
	return epochs_between_reports;
}

void paramSet::setEpochsBetweenReports(unsigned int epochsBetweenReports) {
	epochs_between_reports = epochsBetweenReports;
}

float paramSet::getLearningRate() const {
	return learning_rate;
}

void paramSet::setLearningRate(float learningRate) {
	learning_rate = learningRate;
}

FANN::activation_function_enum paramSet::getActivationFun() const {
	return m_activationFun;
}

void paramSet::setActivationFun(FANN::activation_function_enum activationFun) {
	m_activationFun = activationFun;
}

FANN::training_algorithm_enum paramSet::getTrainingAlg() const {
	return m_trainingAlg;
}

void paramSet::setTrainingAlg(FANN::training_algorithm_enum trainingAlg) {
	m_trainingAlg = trainingAlg;
}

unsigned int paramSet::getMaxEpochs() const {
	return max_epochs;
}

void paramSet::setMaxEpochs(unsigned int maxEpochs) {
	max_epochs = maxEpochs;
}

unsigned int paramSet::getNumInput() const {
	return num_input;
}

void paramSet::setNumInput(unsigned int numInput) {
	num_input = numInput;
}

unsigned int paramSet::getNumLayers() const {
	return num_layers;
}

void paramSet::setNumLayers(unsigned int numLayers) {
	num_layers = numLayers;
}

unsigned int paramSet::getNumNeuronsHidden() const {
	return num_neurons_hidden;
}

void paramSet::setNumNeuronsHidden(unsigned int numNeuronsHidden) {
	num_neurons_hidden = numNeuronsHidden;
}

unsigned int paramSet::getNumOutput() const {
	return num_output;
}

void paramSet::setNumOutput(unsigned int numOutput) {
	num_output = numOutput;
}

const std::string& paramSet::getOutputFilename() const {
	return outputFilename;
}

void paramSet::setOutputFilename(const std::string& outputFilename) {
	this->outputFilename = outputFilename;
}
