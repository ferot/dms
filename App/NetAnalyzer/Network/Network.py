"""Class responsible for representing neural network params, used for further processing (comparision, visualization, etc)"""
class Network:
    def __init__(self):
        num_neurons = 0
        num_input = 0
        num_output = 0
        output_activation_func = ""
        hidden_activation_func = ""

    """Reads report's file content from provided path"""
    def read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read()
        return content

    """Finds MSE error line and returns value"""
    def get_mse(self, content):
        for item in content.split("\n"):
            if "MSE" in item:
                print item