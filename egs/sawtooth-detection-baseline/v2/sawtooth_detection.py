import numpy as np
import os
import matplotlib.pyplot as plt
import sys

#####################
# SCRIPT PARAMETERS #
#####################

stage = 0
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(current_dir, "../../../tools"))

from base import get_globus_version
pyglobus_root = os.path.join(current_dir, "../../..", "_stage-%s" % get_globus_version(), "python")
sys.path.append(pyglobus_root)
try:
    import pyglobus
except ImportError as e:
    print("Cannot import pyglobus from %s, exiting" % pyglobus_root)
    sys.exit(1)

output_dir = os.path.join(current_dir, "output", "sawtooth_detection")

########################
# ALGORITHM PARAMETERS #
########################

SHT_FILE = "../../../data/test/sht-reader/sht38515.sht"
SIGNAL_SAMPLING_RATE = int(1e6)
HIGH_PASS_CUTOFF = 400
SMOOTHED_DD1_ORDER = 30
LOW_PASS_CUTOFF = 5000
SAWTOOTH_DETECTION_THRESHOLD = 0.0005
ROI_DETECTOR_MEAN_SCALE = 1
NUM_SIGNAL_FROM_SHT = 26

####################
# HELPER FUNCTIONS #
####################

# Plotting sample_data and saving it to PNG file
def plot(x, y, label_x, label_y, color="k", new_fig=True, flush=True):
    global stage

    if new_fig:
        plt.figure(figsize=(15, 10))

    plt.plot(x, y, color)
    plt.xlabel(label_x, fontsize=25)
    plt.ylabel(label_y, fontsize=25)

    if flush:
        out = os.path.join(output_dir, "#%i.png" % stage)
        plt.savefig(out)

        print("Stage %i result:" % stage, out)

        stage += 1


if __name__ == "__main__":
    font = {"size": 22}

    plt.rc("font", **font)

    os.makedirs(output_dir, exist_ok=True)

    print("Stage %i: Data loading" % stage)

    sht_reader = pyglobus.util.ShtReader(SHT_FILE)
    signal = sht_reader.get_signal(NUM_SIGNAL_FROM_SHT)
    data = np.array((signal.get_data_x(), signal.get_data_y()))

    print("Loaded %s" % SHT_FILE)

    plot(data[0], data[1], "Время, с", "U, В")

    print("Stage %i: ROI extracting" % stage)

    roi = pyglobus.sawtooth.get_signal_roi(data[1], mean_scale=1)
    x = np.copy(data[0, roi[0]:roi[1]])
    y = np.copy(data[1, roi[0]:roi[1]])

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: High pass filtering" % stage)

    pyglobus.dsp.high_pass_filter(y, HIGH_PASS_CUTOFF, SIGNAL_SAMPLING_RATE)

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: Smoothed differentiation" % stage)

    y = pyglobus.dsp.first_order_diff_filter(y, SMOOTHED_DD1_ORDER)

    plot(x, y, "Время, с", "U', В/с")

    print("Stage %i: Taking absolute value" % stage)

    y = np.abs(y)

    plot(x, y, "Время, с", "|U'|, В/с")

    print("Stage %i: Low pass filtering" % stage)

    pyglobus.dsp.low_pass_filter(y, LOW_PASS_CUTOFF, SIGNAL_SAMPLING_RATE)

    plot(x, y, "Время, с", "|U'|, В/с", flush=False)
    plot(x, [SAWTOOTH_DETECTION_THRESHOLD] * len(x), "Время, с", "|U'|, В/с", color="r", new_fig=False)

    print("Stage %i: Sawtooth detection" % stage)

    start_ind, end_ind = pyglobus.sawtooth.get_sawtooth_indexes(y, SAWTOOTH_DETECTION_THRESHOLD)

    plt.figure(figsize=(15, 10))
    plt.axvline(x[start_ind], color="r")
    plt.axvline(x[end_ind], color="r")
    plot(data[0], data[1], "Время, с", "U, В", new_fig=False)

    print("Done!")
