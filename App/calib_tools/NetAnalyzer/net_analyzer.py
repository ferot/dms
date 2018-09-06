from Visualizer.Visualizer import Visualizer
from ReportParser.ReportParser import ReportParser
import sys

def main():
    parser = ReportParser("reports/")
    parser.enlist_reports()
    results = parser.process()

    viz = Visualizer(results)
    best_net = parser.get_best_net_candidate ()


    for arg in sys.argv:
        if "nodraw" in arg:
            with open ('best_cand', 'a') as the_file:
                str_to_write = best_net.get_name() + " " + str(best_net.get_mse())
                the_file.write(str_to_write)
        elif len(sys.argv) == 1:
            viz.draw_plot ("MSE", "MSE(neu_nr)")

            # best_net.print_net_info()

if __name__ == "__main__":
    main()
