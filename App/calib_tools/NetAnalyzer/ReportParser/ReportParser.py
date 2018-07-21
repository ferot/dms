import Queue
import re
import os
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
            content = self._read_content(self._item_queue.get())
            #mse = self._get_mse(content)
            params_tuple = self._extract_net_params(content)
            net = Network(*params_tuple)
            aggregate = Aggregate(net)
            aggregate_list.append(aggregate)

        return aggregate_list

    """Enlists reports to parse and stores in internal list"""
    def enlist_reports(self):
        ext = ['rep']
        files = [f for f in os.listdir (self._dir_pattern) if ".rep" in f]
        for name in files:
            self._item_queue.put(name)
    """Reads report's file content from provided path"""
    def _read_content(self, path):
        file_handler = open(self._dir_pattern + path, 'r')
        content = file_handler.readlines()

        return content

    """Finds MSE error line and returns value"""
    def _get_mse(self, content):
        for item in content.readline():
            if "MSE" in item:
                #TODO add more parsing
                return item

    """Extracts net params from content and returns tuple"""
    def _extract_net_params(self, content):
        net_name = ""
        num_input = 0
        num_output = 0
        num_neu = 0
        mse = 0


        for item in content:
            if "net_path" in item:
                net_name = item.split(":")[1].rstrip()
                print net_name
            elif "MSE" in item:
                mse = float(item.split(":")[1].rstrip().replace(',','.'))
                print mse
            elif "num_input" in item:
                num_input = int(item.split(":")[1].rstrip())
                print num_input
            elif "num_neu_hid" in item:
                num_neu = int(item.split(":")[1].rstrip())
                print num_neu

        return (num_input, num_output, num_neu, mse, net_name)
