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
        self._aggregate_list = []

    """Main processing method"""
    def process(self):
        while not self._item_queue.empty():
            content = self._read_content(self._item_queue.get())
            params_tuple = self._extract_net_params(content)
            net = Network(*params_tuple)

            self._aggregate_net(net)
        return self._aggregate_list

    """Enlists reports to parse and stores in internal list"""
    def enlist_reports(self):
        ext = ['rep']
        files = [f for f in os.listdir (self._dir_pattern) if ".rep" in f]
        for name in files:
            self._item_queue.put(name)

    """ Returns best net from all available net architectures"""
    def get_best_net_candidate(self):
        best_net = Network(0, 0, 0, 1, " ", 0, 0, 0)

        for agg in self._aggregate_list:
            temp_list = sorted (Aggregate.get_net_list(agg), key=lambda x: Network.get_mse(x), reverse=True)
            best_aggregate_net = temp_list[0]
            if Network.get_mse(best_aggregate_net) < Network.get_mse(best_net):
                best_net = best_aggregate_net

        return best_net

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
        net_name = " "
        num_input = 0
        num_output = 0
        num_neu = 0
        mse = 0
        act_fun_hid = -1
        act_fun_out = -1
        train_alg = -1


        for item in content:
            if "net_path" in item:
                net_name = item.split(":")[1].strip()
            elif "MSE" in item:
                mse = float(item.split(":")[1].rstrip().replace(',','.'))
            elif "num_input" in item:
                num_input = int(item.split(":")[1].rstrip())
            elif "num_output" in item:
                num_output = int(item.split(":")[1].rstrip())
            elif "num_neu_hid" in item:
                num_neu = int(item.split(":")[1].rstrip())
            elif "activ_fun_hid" in item:
                act_fun_hid = int(item.split(":")[1].rstrip())
            elif "activ_fun_out" in item:
                act_fun_out = int(item.split(":")[1].rstrip())
            elif "train_alg" in item:
                train_alg = int(item.split(":")[1].rstrip())

            # assume that MSE bigger than one should be treated as 100% (to not overscale diagrams)
            if mse > 1:
                mse = 1

        return num_input, num_output, num_neu, mse, net_name, act_fun_out, act_fun_hid, train_alg

    def _aggregate_net(self, net):
        found_type = False
        for agg in self._aggregate_list:
            if Aggregate.get_type(agg) == Network.get_type(net):
                agg.insert_net(net)
                found_type = True

        if found_type is False:
            new_aggregate = Aggregate(Network.get_type(net))
            new_aggregate.insert_net(net)
            self._aggregate_list.append(new_aggregate)