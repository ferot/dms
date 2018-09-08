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
        if not self.refreshMSE(net):
            self._net_list.append(net)

    def get_net_list(self):
        return self._net_list

    def refreshMSE(self, netToinsert):
        needRefresh = False
        for net in self._net_list:
            if (Network.get_type(net) == Network.get_type(netToinsert)) and (Network.get_numneur(net) == Network.get_numneur(netToinsert)):
                mse = (Network.get_mse(net) + Network.get_mse(netToinsert))/2
                Network.setMSE(net, mse)
                needRefresh = True
                break

        return needRefresh