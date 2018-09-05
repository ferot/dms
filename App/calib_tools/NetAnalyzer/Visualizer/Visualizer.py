import matplotlib.pyplot as plt
from numpy import pi, linspace, sin
from DataAggregator.Aggregate import Aggregate


class Visualizer(object):
    def __init__(self, aggregates):
        self._aggregates = aggregates
        self._plot = plt
        self._col_dict = {
            0: "ro",
            1: "bo",
            2: "go",
            3: "yo",
            4: "co",
            5: "mo",
            6: "ko"
        }

    """Draws provided plot handle"""
    def draw_plot(self, title, type):
        if "MSE(neu_nr)" in type:
            x_label = "neuron_nr"
            y_label = "MSE"

            x = []
            y = []
            col_idx = 0
            for agg in self._aggregates:
                net_list = Aggregate.get_net_list(agg)
                dot_type = self._col_dict[col_idx]
                x = []
                y = []
                for net in net_list:
                    _y = net.get_mse()
                    _x = net.get_numneur()
                    x.append(_x)
                    y.append(_y)
                plt.plot(x, y, dot_type, label=Aggregate.get_type(agg))
                col_idx += 1
                if col_idx > 6:
                    col_idx = 0
                    print "WARNING : too many plots. Overwriting graphs!"

            # x2 = linspace(0,4*pi, 100)
            # self._plot = plt.plot(x2, sin(x2), self._col_dict[3], label=Aggregate.get_type(agg))

            plt.xlabel(x_label)
            plt.ylabel(y_label)
            plt.title(title)
            plt.grid(True)
            plt.legend()
            plt.plot()

            plt.show()


    """Saves plot under provided path"""
    def save_plots(self):
        for plot in self._plots:
            plot.savefig("plots/" + plot.get_net() + ".png")

    def show_plots(self):
        pass