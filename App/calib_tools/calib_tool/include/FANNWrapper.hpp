#ifndef FANNWRAPER_HPP
#define FANNWRAPER_HPP

#include <QString>

#include "floatfann.h"
//C++ porting of FANN
#include "fann_cpp.h"

#include "DataSet.hpp"
#include "logger.h"
#include <QMessageBox>
#include "Config.hpp"
#include "ParamSetGenerator.hpp"

#include <ios>
#include <iostream>
#include <iomanip>
#include "calibtool.h"
#include "Common.hpp"

namespace Ui {
class CalibTool;
}

class FANNWrapper{


    std::string outputFilename;
    std::string inputFilename;
    std::string outputDir;
    std::string inputDir;

    FANN::neural_net net;
    FANN::training_data data;

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

    Ui::CalibTool* UI;

    static int printMSE_callback(FANN::neural_net &net, FANN::training_data &train,
                                 unsigned int max_epochs, unsigned int epochs_between_reports,
                                 float desired_error, unsigned int epochs, void *user_data)
    {
        std::string output = std::string(("Current EPOCH | MSE:\t") + std::to_string(epochs) + " | " + std::to_string(net.get_MSE()));

        LOGMSG_F_ARG(LOG_NOTICE, "%s\n", output.c_str());
        return 0;
    }

    /**
     * @brief printTextEdit - Helper function to show FANN output in calib_tool console.
     * @param str - QString to be added
     * @param UI - handle to ui. used to refer textedit component
     */
	static void printTextEdit(QString str, Ui::CalibTool* UI) {
		UI->text_edit_output->insertPlainText(str + QString("\n"));
	}

public:

    FANNWrapper( Ui::CalibTool* ui = nullptr, std::string outFile = "trainedNet.net") :
        outputFilename(outFile),
        num_layers(3), //this includes in and out layers. Thus cannot be less than 2!
        num_neurons_hidden(3),
        desired_error(0.001f),
        max_epochs(500000),
        epochs_between_reports(1000),
        learning_rate(0.7f),
        UI(ui)
    {

    	inputFilename = "train_data.dat";
    	num_input = std::stoi(Config::getInstance()->getValue("ANN", "input_num"));
    	num_output = std::stoi(Config::getInstance()->getValue("ANN", "output_num"));


        net.create_standard(num_layers, num_input, num_neurons_hidden, num_output);
        LOGMSG(LOG_DEBUG, "[FANNWRAPPER]  net.create_standard");

        net.set_learning_rate(learning_rate);

        net.set_activation_steepness_hidden(1.0);
        net.set_activation_steepness_output(1.0);

        m_activationFun = FANN::SIGMOID_SYMMETRIC_STEPWISE;

        net.set_activation_function_hidden(m_activationFun);
        net.set_activation_function_output(m_activationFun);

//        net.set_training_algorithm(FANN::TRAIN_QUICKPROP);
//        net.print_parameters();
    }

    FANNWrapper(paramSet& paramSet, Ui::CalibTool* ui = nullptr) {
    	UI = ui;

    	inputFilename = "train_data.dat";
        inputDir = "datasets/" + generateDateString() + "/";

    	outputFilename = paramSet.getOutputFilename() + ".net";
        outputDir = "nets/" + generateDateString() + "/";
        createDirectory(outputDir);

        num_input = std::stoi(Config::getInstance()->getValue("ANN", "input_num"));
    	num_output = std::stoi(Config::getInstance()->getValue("ANN", "output_num"));


        net.create_standard(4, 9, 5, 5, 2);
        LOGMSG(LOG_DEBUG, "[FANNWRAPPER]  net.create_standard");

//        net.set_learning_rate(0.7);

        net.set_activation_function_hidden(FANN::activation_function_enum::SIGMOID_SYMMETRIC);
        net.set_activation_function_output(FANN::activation_function_enum::LINEAR);

//        int act = paramSet.getActivationFun(true);
//        LOGMSG_ARG(LOG_TRACE, "[FANNWRAPPER] Activation func HIDDEN : %d...", act);

//        net.set_activation_function_hidden(static_cast<FANN::activation_function_enum>(act));

//        act = paramSet.getActivationFun(false);
//        LOGMSG_ARG(LOG_TRACE, "[FANNWRAPPER] Activation func OUTPUT : %d...", act);
//        net.set_activation_function_output(static_cast<FANN::activation_function_enum>(act));

        net.set_training_algorithm(FANN::training_algorithm_enum::TRAIN_RPROP);
        net.print_parameters();

    }


    ~FANNWrapper(){
    }

    void trainNet(){
         if (data.read_train_from_file(inputDir + inputFilename))
            {
                // Initialize and train the network with the data
                net.set_callback(printMSE_callback, reinterpret_cast<void*>(UI));

                LOGMSG_ARG(LOG_DEBUG, "[FANNWRAPPER] Starting trainNet() on file : %s...", (inputDir + inputFilename).c_str());
                LOGMSG_F_ARG(LOG_NOTICE, "[ REPORT FOR DATASET : %s ]\n\n", (inputDir + inputFilename).c_str());
                net.set_scaling_params(data, -1, 1, -1, 1);
                net.scale_train(data);
                net.train_on_data(data, 5000,
                    1000, 0.001);

//                printTextEdit(QString( "Finished training. Now Testing network..."), UI);

                for (unsigned int i = 0; i < data.length_train_data(); i++)
                {
                    // Run the network on the test data
                    net.reset_MSE();
                    fann_type *calc_out = net.run(data.get_input()[i]);
                    net.descale_output(calc_out);

                    std::string report_step = generateReport(data, calc_out, i);

                    LOGMSG_F_ARG(LOG_NOTICE, "%s\n", report_step.c_str());
                    LOGMSG_ARG(LOG_DEBUG, "%s", report_step.c_str());
                }

                LOGMSG_ARG(LOG_DEBUG, "[FANNWRAPPER] Saving network to file : %s", (outputDir + outputFilename).c_str());

                net.save(outputDir + outputFilename);

//                printTextEdit(QString("-----------------------------FINISHED----------------------------------"), UI);

		} else {
			LOGMSG(LOG_ERROR,"[FANNWRAPPER] Couldn't start train procedure...");
		}
    }

    /**
     * Sets filename for ANN input for training. It's assumed to have .data extension.
     * @param inpFile
     */
    void setInputFile(std::string inpFile){
        LOGMSG_ARG(LOG_TRACE, "[FANNWRAPPER] Setting input file to %s", inpFile.c_str());
        inputFilename = inpFile;
    }

    /**
     * Sets filename for trained ANN. It's assumed to have .net extension.
     * @param outFile
     */
    void setOutputFile(std::string outFile){
        LOGMSG_ARG(LOG_TRACE, "[FANNWRAPPER] Setting output file to %s", outFile.c_str());
        inputFilename = outFile;
    }

	unsigned int getNumInput() const {
		return num_input;
	}

	void setNumInput(unsigned int numInput) {
		num_input = numInput;
	}

	unsigned int getNumOutput() const {
		return num_output;
	}

	void setNumOutput(unsigned int numOutput) {
		num_output = numOutput;
	}

	void setProgressBar(int val){
        UI->progressBar->setValue(val);
	}

    std::string generateReport(FANN::training_data data, fann_type* calc_out, int i) {

        QString strBffer =
                QString(
                    "\nTest("
                    + QString::number(
                        data.get_input()[i][0]) + " , "
                + QString::number(
                    data.get_input()[i][1]) + " , "
                + QString::number(
                    data.get_input()[i][2]) + " , "
                + QString::number(
                    data.get_input()[i][3]) + " , "
                + QString::number(
                    data.get_input()[i][4]) + " , "
                + QString::number(
                    data.get_input()[i][5]) + " , "
                + QString::number(
                    data.get_input()[i][6]) + " , "
                + QString::number(
                    data.get_input()[i][7]) + " , "
                + QString::number(
                    data.get_input()[i][8])
                + ") --> " + QString::number(calc_out[0]) + ", " + QString::number(calc_out[1])
                + "\nshould be : "
                + QString::number(
                    data.get_output()[i][0]) + ","
                + QString::number(
                    data.get_output()[i][1])
                + "\ndiff = ("
                + QString::number(
                    fann_abs( calc_out[0] - data.get_output()[i][0]))
                + " , "
                + QString::number(
                    fann_abs( calc_out[1] - data.get_output()[i][1])) + ")"
                );
        return strBffer.toStdString();
    }


};


#endif // FANNWRAPER_HPP
