from scipy import signal
import numpy as np
import os
import matplotlib.pyplot as plt

#####################
# SCRIPT PARAMETERS #
#####################

stage = 0
current_dir = os.path.dirname(os.path.abspath(__file__))
output_dir = os.path.join(current_dir, "output", "sawtooth_frequency_computation")

########################
# ALGORITHM PARAMETERS #
########################

DATA_FILE = "38515_SXR 80 mkm.npy"
HIGH_PASS_CUTOFF = 250
LOW_PASS_CUTOFF = 2000
MOVING_AVERAGE_WINDOW_SIZE = 5
SAWTOOTH_ROI = (162000, 197700)

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


# Applies Butterworth filter
def butter_filter(input_, cutoff, fs, btype, order=5):
    b, a = signal.butter(order, cutoff / (0.5 * fs), btype=btype, analog=False)
    return signal.filtfilt(b, a, input_)


# Applies moving average window
def moving_average(x, w):
    return np.convolve(x, np.ones(w), "valid") / w


if __name__ == "__main__":
    font = {"size": 22}

    plt.rc("font", **font)

    os.makedirs(output_dir, exist_ok=True)

    print("Stage %i: Data loading and preparing" % stage)

    data = np.load(os.path.join(current_dir, "sample_data", DATA_FILE))

    print("Loaded %s" % DATA_FILE)

    x = data[0, SAWTOOTH_ROI[0]:SAWTOOTH_ROI[1]]
    y = data[1, SAWTOOTH_ROI[0]:SAWTOOTH_ROI[1]]

    plot(data[0], data[1], "Время, с", "U, В")

    print("Stage %i: High pass filtering" % stage)

    sample_rate = 1.0 / (x[1] - x[0])

    y = butter_filter(y, HIGH_PASS_CUTOFF, sample_rate, btype="high")

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: Low pass filtering" % stage)

    y = butter_filter(y, LOW_PASS_CUTOFF, sample_rate, btype="low")

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: Finding zero crossings" % stage)

    zero_crossings = np.where(np.diff(np.sign(y)))[0]

    plot(x, y, "Время, с", "U, В", flush=False)
    plot(x[zero_crossings], y[zero_crossings], "Время, с", "U, В", color="rx", new_fig=False)

    print("Stage %i: Computing frequencies" % stage)

    freqs = []

    for i in range(len(zero_crossings) - 2):
        freqs.append(1 / (x[zero_crossings[i + 2]] - x[zero_crossings[i]]))

    x = x[zero_crossings][:-(MOVING_AVERAGE_WINDOW_SIZE + 1)]
    y = moving_average(freqs, MOVING_AVERAGE_WINDOW_SIZE)

    plot(x, y, "Время, с", "Частота, Гц", color="ko-")

    print("Done!")
