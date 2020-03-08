from scipy import signal
import numpy as np
import os
import matplotlib.pyplot as plt

#####################
# SCRIPT PARAMETERS #
#####################

stage = 0
current_dir = os.path.dirname(os.path.abspath(__file__))
output_dir = os.path.join(current_dir, "output", "sawtooth_detection")

########################
# ALGORITHM PARAMETERS #
########################

DATA_FILE = "38515_SXR 80 mkm.npy"
HIGH_PASS_CUTOFF = 400
SMOOTHED_DD1_ORDER = 30
LOW_PASS_CUTOFF = 5000

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


# Extracting ROI from signal
def get_roi(y, mean_scale=0.96):
    threshold = np.mean(y) * mean_scale

    start_index = 0
    end_index = 0

    data_length = y.shape[0]

    for i in range(data_length):
        if y[i] > threshold:
            start_index = i
            break

    for i in range(1, data_length):
        if y[data_length - i] > threshold:
            end_index = data_length - i
            break

    return start_index, end_index


# Computing smoothed first derivative
def smoothed_dd1(input_, order):
    input_ = input_.astype(np.float32)
    coeff = 1.0 / order / (order + 1)

    data_size = np.shape(input_)[0]

    output = np.zeros(data_size)

    for i in range(data_size):
        for c in range(1, order + 1):
            output[i] += coeff * (input_[min(i + c, data_size - 1)] - input_[max(i - c, 0)])

    return output


if __name__ == "__main__":
    font = {"size": 22}

    plt.rc("font", **font)

    os.makedirs(output_dir, exist_ok=True)

    print("Stage %i: Data loading" % stage)

    data = np.load(os.path.join(current_dir, "sample_data", DATA_FILE))

    print("Loaded %s" % DATA_FILE)

    plot(data[0], data[1], "Время, с", "U, В")

    print("Stage %i: ROI extracting" % stage)

    roi = get_roi(data[1], mean_scale=1)
    x = data[0, roi[0]:roi[1]]
    y = data[1, roi[0]:roi[1]]

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: High pass filtering" % stage)

    sample_rate = 1.0 / (x[1] - x[0])

    y = butter_filter(y, HIGH_PASS_CUTOFF, sample_rate, btype="high")

    plot(x, y, "Время, с", "U, В")

    print("Stage %i: Smoothed differentiation" % stage)

    y = smoothed_dd1(y, SMOOTHED_DD1_ORDER)

    plot(x, y, "Время, с", "U', В/с")

    print("Stage %i: Taking absolute value" % stage)

    y = np.abs(y)

    plot(x, y, "Время, с", "|U'|, В/с")

    print("Stage %i: Low pass filtering" % stage)

    y = butter_filter(y, LOW_PASS_CUTOFF, sample_rate, btype="low")

    plot(x, y, "Время, с", "|U'|, В/с", flush=False)
    plot(x, [0.0005] * len(x), "Время, с", "|U'|, В/с", color="r", new_fig=False)

    print("Done!")
