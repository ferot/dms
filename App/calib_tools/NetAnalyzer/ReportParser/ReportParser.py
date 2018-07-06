import Queue
from setuptools import glob
from DataAggregator.Aggregate import Aggregate
from DataAggregator.Network import Network


class ReportParser:
    def __init__(self, report_dir):
        self._dir_pattern = report_dir
        self._item_queue = Queue.Queue()
        self._networks = []

    """Main processing method"""
    def process(self):
        aggregate_list = []
        while not self._item_queue.empty():
            content = self.read_content(self._item_queue.get())
            mse = self.get_mse(content)
            params_tuple = self.extract_net_params(content)

            net = Network(params_tuple)
            aggregate = Aggregate(net, mse)
            aggregate_list.append(aggregate)

        return aggregate_list

    """Enlists reports to parse and stores in internal list"""
    def enlist_reports(self):
        for name in glob.glob(self._dir_pattern):
            self._item_queue.put(name)

    """Reads report's file content from provided path"""
    def _read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read()

        return content

    """Finds MSE error line and returns value"""
    def _get_mse(self, content):
        for item in content.split("\n"):
            if "MSE" in item:
                #TODO add more parsing
                return item

    """Extracts net params from content and returns tuple"""
    def _extract_net_params(self, content):
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

            return num_input, num_output, num_neu, mse, net_name
