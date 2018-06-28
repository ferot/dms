/*
 * ParamSet.cpp
 *
 *  Created on: May 22, 2018
 *      Author: tf
 */

#include "ParamSet.hpp"


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

/**
 * @brief paramSet::getActivationFun
 * @param hidden boolean determining if should return hidden activation func, otherwise output's
 * @return activation func enum
 */
FANN::activation_function_enum paramSet::getActivationFun(bool hidden) const {
	return hidden ? m_activationFunHidden : m_activationFunOutput;
}

/**
 * @brief paramSet::setActivationFun - sets activation func members
 * @param activationFun - value to be set
 * @param hidden - flag determining which type of activation func should be set (hidden when true, output otherwise).
 */
void paramSet::setActivationFun(FANN::activation_function_enum activationFun, bool hidden) {
	hidden ? m_activationFunHidden = activationFun : m_activationFunOutput = activationFun;
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

float paramSet::getActivationSteepnessHidden() const {
	return activation_steepness_hidden;
}

void paramSet::setActivationSteepnessHidden(float activationSteepnessHidden) {
	activation_steepness_hidden = activationSteepnessHidden;
}

float paramSet::getActivationSteepnessOutput() const {
	return activation_steepness_output;
}

void paramSet::setActivationSteepnessOutput(float activationSteepnessOutput) {
	activation_steepness_output = activationSteepnessOutput;
}

void paramSet::setOutputFilename(const std::string& outputFilename) {
	this->outputFilename = outputFilename;
}

