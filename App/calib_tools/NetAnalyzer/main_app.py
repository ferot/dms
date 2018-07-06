from Visualizer.Visualizer import Visualizer
from ReportParser.ReportParser import ReportParser


def main():
    parser = ReportParser("path")
    parser.enlist_reports()
    results = parser.process()

    viz = Visualizer(results)

    viz.draw_plot("MSE", "MSE(neu_nr)")
    viz.show_plots()


if __name__ == "__main__":
    main()
