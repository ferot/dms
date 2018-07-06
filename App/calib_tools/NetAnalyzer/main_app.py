from Visualizer.Visualizer import Visualizer
from ReportParser.ReportParser import ReportParser

def main():
    parser = ReportParser("path")
    viz = Visualizer()

    viz.draw_plot("x", "y", "MSE")



if __name__ == "__main__":
    main()
