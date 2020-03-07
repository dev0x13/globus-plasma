#!/usr/bin/python3.6

import numpy as np
import pyglobus
import csv
import argparse
import matplotlib.pyplot as plt

####################################
# Sawtooth crash labels visualizer #
####################################

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Frozen graph to TFLite graph converter.')

    parser.add_argument("-i", "--sht-file", dest="sht_file", required=True,
                        help="path to a valid `tgraph.cfg` file")
    parser.add_argument("-l", "--labels", dest="labels_file", required=True,
                        help="path to output directory (will be created if not exists)")

    args = parser.parse_args()

    sht_reader = pyglobus.ShtReader(args.sht_file)

    with open(args.labels_file, "r") as f:
        reader = csv.reader(f)

        # Skip header
        next(reader)

        for row in reader:
            num_signal = int(row[0])
            start_ind = int(row[1])
            end_ind = int(row[2])

            signal = sht_reader.get_signal(num_signal)
            x = signal.get_data_x()
            y = signal.get_data_y()

            plt.plot(x, y)
            plt.axvline(x[start_ind], color="r")
            plt.axvline(x[end_ind], color="r")
            plt.show()
