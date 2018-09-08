"""Class responsible for representing neural network params, used for further processing (comparision, visualization, etc)"""


class Network:
    def __init__(self, num_input, num_output, num_neurons, mse, name, act_fun_out, act_fun_hid, train_alg):
        self._num_neurons = num_neurons
        self._num_input = num_input
        self._num_output = num_output
        # self._output_activation_func = output_func
        self._mse = mse
        # self._hidden_activation_func = hidden_func
        self._name = name
        self._act_fun_hid = act_fun_hid
        self._act_fun_out = act_fun_out
        self._train_alg = train_alg
        self._type = self._prepare_type()

    """ Prepares type of network based on train alg and activation functions of hidden and output layers"""
    def _prepare_type(self):
        type = self.map_train_alg(self._train_alg) + "_" + self.map_activation_fun(self._act_fun_hid) + "_" + self.map_activation_fun(self._act_fun_out)
        return type

    def get_type(self):
        return self._type

    """Reads report's file content from provided path"""
    def read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read ()
        return content

    def print_net_info(self):
        print "NET INFO :"
        print "PATH + NAME : " + self._name
        print "NUM_INPUT : " + str(self._num_input)
        print "NUM_OUTPUT : " + str(self._num_output)
        print "NUM_NEU_HIDD : " + str(self._num_neurons)
        print "MSE : " + str(self._mse)


    """Finds MSE error line and returns value"""
    def get_mse(self):
        return self._mse

    def get_name(self):
        return self._name

    def get_numneur(self):
        return self._num_neurons

    """ Maps training algorithm into string representation"""
    def map_train_alg(self, num):
        dict = {0: "INCREMENTAL",
                1: "BATCH",
                2: "RPROP",
                3: "QUICKPROP",
                4: "SARPROP"}
        return dict[num]

    """ Maps activation function into string representation"""
    def map_activation_fun(self, num):
        dict = {0: "LINEAR",
                4: "SIGMOID_STEPWISE",
                5: "SIGMOID_SYMMETRIC",
                6: "SIGMOID_SYMMETRIC_STEPWISE",
                3: "SIGMOID"}
        return dict[num]

    def setMSE(self, mse):
        self._mse = mse