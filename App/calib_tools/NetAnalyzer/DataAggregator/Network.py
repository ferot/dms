"""Class responsible for representing neural network params, used for further processing (comparision, visualization, etc)"""


class Network:
    def __init__(self, num_input, num_output, num_neurons, output_func, hidden_func, name):
        self._num_neurons = num_neurons
        self._num_input = num_input
        self._num_output = num_output
        self._output_activation_func = output_func
        self._hidden_activation_func = hidden_func
        self._name = name

    """Reads report's file content from provided path"""

    def read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read ()
        return content

    """Finds MSE error line and returns value"""

    def get_mse(self, content):
        for item in content.split ("\n"):
            if "MSE" in item:
                print item

    def get_name(self):
        return self._name

    def map_train_alg(self, num):
        dict = {0: "INCREMENTAL",
                1: "BATCH",
                2: "RPROP",
                3: "QUICKPROP",
                4: "SARPROP"}
        return dict[num]
