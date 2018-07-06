from DataAggregator.Network import Network

"""Class aggregating network params and other info extracted from reports"""
class Aggregate:
    def __init__(self, net, mse):
        self._net = net
        self._mse = mse

    def get_mse(self):
        return self._mse

    def get_net(self):
        return self._net
