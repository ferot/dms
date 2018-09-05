from DataAggregator.Network import Network

"""Class aggregating network params and other info extracted from reports"""
class Aggregate:
    def __init__(self, type):
        self._net_list = []
        self._type = type

    def set_type(self, plot):
        self._type = self._net

    def get_type(self):
        return self._type

    def insert_net(self, net):
        self._net_list.append(net)

    def get_net_list(self):
        return self._net_list
