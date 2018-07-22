from Visualizer.Visualizer import Visualizer
from ReportParser.ReportParser import ReportParser


def main():
    parser = ReportParser("/home/tf/repos/dms/build/bin/reports/")
    parser.enlist_reports()
    results = parser.process()

    viz = Visualizer(results)

    viz.draw_plot("MSE", "MSE(neu_nr)")
    viz.show_plots()

    parser.get_best_net_candidate().print_net_info()

if __name__ == "__main__":
    main()
