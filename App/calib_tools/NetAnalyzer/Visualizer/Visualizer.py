import matplotlib.pyplot as plt
import numpy as np


class Visualizer(object):
    def __init__(self):
        pass

    """Draws provided plot handle"""
    def draw_plot(self, x_label, y_label, title):
        t = np.arange(0.0, 2.0, 0.01)
        s = 1 + np.sin(2 * np.pi * t)
        plt.plot(t, s)

        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.title(title)
        plt.grid(True)
        plt.show()

    """Saves plot under provided path"""
    def save_plot(self, plt, path):
        plt.savefig (path + ".png")
