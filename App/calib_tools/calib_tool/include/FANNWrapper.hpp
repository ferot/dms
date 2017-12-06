#ifndef FANNWRAPER_HPP
#define FANNWRAPER_HPP

#include <QString>

#include "floatfann.h"
//C++ porting of FANN
#include "fann_cpp.h"
#include "DataSet.hpp"
#include "logger.h"
#include <QMessageBox>

#include <ios>
#include <iostream>
#include <iomanip>
#include "calibtool.h"

namespace Ui {
class CalibTool;
}


class FANNWrapper{


    std::string outputFilename;
    std::string inputFilename;

    FANN::neural_net net;
    FANN::training_data data;

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

        Ui::CalibTool* UI = static_cast<Ui::CalibTool*>(user_data);
        printTextEdit(QString("Current EPOCH | MSE:\t") + QString::number(epochs) + " | " + QString::number(net.get_MSE()), UI);

         return 0;
     }

    /**
     * @brief printTextEdit - Helper function to show FANN output in calib_tool console.
     * @param str - QString to be added
     * @param UI - handle to ui. used to refer textedit component
     */
    static void printTextEdit(QString str, Ui::CalibTool* UI){
        QString buffer = UI->text_edit_output->toPlainText() + str;
        UI->text_edit_output->setText(buffer+ QString("\n"));
    }

public:

    FANNWrapper( Ui::CalibTool* ui = nullptr, std::string outFile = "trainedNet.net") :
        outputFilename(outFile),
        num_input(2),
        num_output(1),
        num_layers(3), //this includes in and out layers. Thus cannot be less than 2!
        num_neurons_hidden(3),
        desired_error(0.001f),
        max_epochs(500000),
        epochs_between_reports(1000),
        learning_rate(0.7f),
        UI(ui)
    {

        net.create_standard(num_layers, num_input, num_neurons_hidden, num_output);
        LOGMSG(LOG_DEBUG, "[FANNWRAPPER]  net.create_standard");

        net.set_learning_rate(learning_rate);

        net.set_activation_steepness_hidden(1.0);
        net.set_activation_steepness_output(1.0);

        net.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        net.set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);

//        net.set_training_algorithm(FANN::TRAIN_QUICKPROP);
        net.print_parameters();
    }

    ~FANNWrapper(){
    }

    void trainNet(){
         if (data.read_train_from_file(inputFilename))
            {
                // Initialize and train the network with the data
                net.init_weights(data);

//                cout << "Max Epochs " << setw(8) << max_epochs << ". "
//                    << "Desired Error: " << left << desired_error << right << endl;
                net.set_callback(printMSE_callback, reinterpret_cast<void*>(UI));

                LOGMSG_ARG(LOG_DEBUG, "[FANNWRAPPER] Starting trainNet()...", inputFilename.c_str());
                net.train_on_data(data, max_epochs,
                    epochs_between_reports, desired_error);

                UI->progressBar->setValue(50);
                printTextEdit(QString( "Finished training. Now Testing network..."), UI);

                for (unsigned int i = 0; i < data.length_train_data(); ++i)
                {
                    // Run the network on the test data
                    fann_type *calc_out = net.run(data.get_input()[i]);

                    QString strBffer =
                            QString("\nTest(" + QString::number(data.get_input()[i][0]) +
                            		" , " + QString::number(data.get_input()[i][1]) +
                                    ") --> " + QString::number(*calc_out) +
                            		"\nshould be : " + QString::number(data.get_output()[i][0])+
                            		"\ndiff = " + QString::number(fann_abs(*calc_out - data.get_output()[i][0])));
                    printTextEdit(strBffer, UI);

                }

                UI->progressBar->setValue(75);
                LOGMSG_ARG(LOG_DEBUG, "[FANNWRAPPER] Saving network to file : %s", outputFilename.c_str());

                net.save(outputFilename);
                unsigned int decimal_point = net.save_to_fixed(std::string("fixed") + outputFilename);
                data.save_train_to_fixed(std::string("fixed") + inputFilename, decimal_point);

                UI->progressBar->setValue(100);

                printTextEdit(QString("-----------------------------FINISHED----------------------------------"), UI);

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

};


#endif // FANNWRAPER_HPP
