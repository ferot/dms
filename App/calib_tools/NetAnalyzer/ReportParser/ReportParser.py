import Queue
from setuptools import glob
import DataAggregator.Network
import DataAggregator.Aggregate


class ReportParser:
    def __init__(self, report_dir):
        self.dir = report_dir
        self.item_queue = Queue.Queue()
        self.networks = []

    """Main processing method"""
    def process(self, content):
        aggregate_list = []
        while not self.item_queue.empty():
            content = self.read_content(self.item_queue.get())
            mse = self.get_mse(content)
            params_tuple = self.extract_net_params(content)

            net = DataAggregator.Network (params_tuple)
            aggregate = DataAggregator(net, mse)
            aggregate_list.append(aggregate)

        return aggregate_list

    """Enlists reports to parse and stores in internal list"""
    def enlist_reports(self, path):
        for name in glob.glob(self.dir_pattern):
            self.item_queue.put(name)

    """Reads report's file content from provided path"""
    def read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read()
        return content

    """Finds MSE error line and returns value"""
    def get_mse(self, content):
        for item in content.split("\n"):
            if "MSE" in item:
                #TODO add more parsing
                return item

    """Extracts net params from content and returns tuple"""
    def extract_net_params(self, content):
        num_input = 0
        num_output = 0
        num_neu = 0
        mse = 0

        for item in content.split ("\n"):
            if "NETWORK NAME" in item:
                net_name = item.split(":").second
                print net_name
            if "MSE" in item:
                mse = item.split(":").second
                print mse
            elif "num_input" in item:
                num_input = item.split(":").second
                print num_input
            elif "num_neu_hid" in item:
                num_neu = item.split(":").second
                print num_neu
            elif "activ_fun_steep_hid" in item:
                num_neu = item.split(":").second
                print num_neu
            elif "activ_fun_steep_hid" in item:
                num_neu = item.split(":").second
                print num_neu
            return num_input, num_output, num_neu, mse
