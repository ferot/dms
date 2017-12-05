#ifndef FANNWRAPER_HPP
#define FANNWRAPER_HPP

#include <QString>

#include "doublefann.h"
//C++ porting of FANN
#include "fann_cpp.h"
#include "DataSet.hpp"
#include "calibtool.h"

#include <ios>
#include <iostream>
#include <iomanip>

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


    static int print_callback(FANN::neural_net &net, FANN::training_data &train,
          unsigned int max_epochs, unsigned int epochs_between_reports,
          float desired_error, unsigned int epochs, void *user_data)
      {
  //        cout << "Epochs     " << setw(8) << epochs << ". "
  //             << "Current Error: " << left << net.get_MSE() << right << endl;
          return 0;
      }

public:

    FANNWrapper( Ui::CalibTool* ui = nullptr, DataSet* ds = nullptr, std::string outFile = "trainedNet.net") :
        outputFilename(outFile),
        num_input(9),
        num_output(2),
        num_layers(1),
        num_neurons_hidden(3),
        desired_error(0.001f),
        max_epochs(500000),
        epochs_between_reports(1000),
        learning_rate(0.7f),
        UI(ui)
    {
    	if(ds == nullptr){
    		inputFilename = "trainSet.data";
    	}else {
    		inputFilename = (ds->getFilePath()).toStdString();
    	}

        net.create_standard(num_layers, num_input, num_neurons_hidden, num_output);
        net.set_learning_rate(learning_rate);

        net.set_activation_steepness_hidden(1.0);
        net.set_activation_steepness_output(1.0);

        net.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
        net.set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);

        //net.set_training_algorithm(FANN::TRAIN_QUICKPROP);
        net.print_parameters();

    }

    ~FANNWrapper(){
    }

    void trainNet(){
    	using std::cout;
    	using std::cerr;
    	using std::endl;
    	using std::setw;
    	using std::left;
    	using std::right;
    	using std::showpos;
    	using std::noshowpos;

        if (data.read_train_from_file(inputFilename))
        {
            // Initialize and train the network with the data
            net.init_weights(data);

            cout << "Max Epochs " << setw(8) << max_epochs << ". "
                << "Desired Error: " << left << desired_error << right << endl;
            net.set_callback(print_callback, NULL);
            net.train_on_data(data, max_epochs,
            		epochs_between_reports, desired_error);

            cout << endl << "Testing network." << endl;

            for (unsigned int i = 0; i < data.length_train_data(); ++i)
            {
                // Run the network on the test data
                fann_type *calc_out = net.run(data.get_input()[i]);

                cout << "XOR test (" << showpos << data.get_input()[i][0] << ", "
                     << data.get_input()[i][1] << ") -> " << *calc_out
                     << ", should be " << data.get_output()[i][0] << ", "
                     << "difference = " << noshowpos
                     << fann_abs(*calc_out - data.get_output()[i][0]) << endl;
            }

            cout << endl << "Saving network." << endl;

            // Save the network in floating point and fixed point
            net.save(outputFilename);
            unsigned int decimal_point = net.save_to_fixed(std::string("fixed") + outputFilename);
            data.save_train_to_fixed("xor_fixed.data", decimal_point);

            cout << endl << "XOR test completed." << endl;
        }
    }

};


#endif // FANNWRAPER_HPP
