#include "sawtooth-functions.h"

#include <dsp/basic.h>

namespace globus {
    std::pair<size_t, size_t> getSignalRoi(const float *signal, size_t signalSize, float meanScale) {
        float threshold = meanScale * globus::mean(signal, signalSize);

        size_t startIndex = 0;
        size_t endIndex = 0;

        for (size_t i = 0; i < signalSize; ++i) {
            if (signal[i] > threshold) {
                startIndex = i;
                break;
            }
        }

        for (size_t i = signalSize - 1; i > 0; --i) {
            if (signal[i] > threshold) {
                endIndex = i;
                break;
            }
        }

        return {startIndex, endIndex};
    }

    std::pair<size_t, size_t> getSawtoothIndexes(const float *signal, size_t signalSize, float threshold) {
        size_t startIndex = 0;
        size_t endIndex = 0;

        for (size_t i = 0; i < signalSize; ++i) {
            if (signal[i] > threshold) {
                startIndex = i;
                break;
            }
        }

        for (size_t i = signalSize - 1; i > 0; --i) {
            if (signal[i] > threshold) {
                endIndex = i;
                break;
            }
        }

        return {startIndex, endIndex};
    }
}