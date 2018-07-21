import matplotlib.pyplot as plt
import numpy as np


class Visualizer(object):
    def __init__(self, aggregates):
        self._aggregates = aggregates
        self._plots = []

    """Draws provided plot handle"""
    def draw_plot(self, title, type):
        if "MSE(neu_nr)" in type:
            x_label = "neuron_nr"
            y_label = "MSE"

            x = []
            y = []
            for agg in self._aggregates:
                _y = agg.get_net().get_mse()
                _x = agg.get_net().get_numneur()
                x.append(_x)
                y.append(_y)

            plot = plt.plot(x, y)

            plt.xlabel(x_label)
            plt.ylabel(y_label)
            plt.title(title)
            plt.grid(True)
            self._plots.append(plt)

    """Saves plot under provided path"""
    def save_plots(self):
        for plot in self._plots:
            plot.savefig("plots/" + plot.get_net() + ".png")

    def show_plots(self):
        for plot in self._plots:
            plot.show()