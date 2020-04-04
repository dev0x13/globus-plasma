#include "filter.h"

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>

namespace globus {
    static constexpr float oneDivPI2 = 1.0 / M_PI_2;

    template<typename T>
    void lowPassFilter(T *signal, size_t signalSize, size_t cutoff, size_t sampleRate) {
        const float RC = oneDivPI2 / cutoff;
        const float dt = 1.0f / sampleRate;
        const float alpha = dt / (RC + dt);

        for (size_t i = 1; i < signalSize; ++i) {
            signal[i] = (float) signal[i - 1] + (alpha * (float) (signal[i] - signal[i - 1]));
        }

        // Fix the first value to conform scale
        signal[0] = signal[1];
    }

    template<typename T>
    void highPassFilter(T *signal, size_t signalSize, size_t cutoff, size_t sampleRate) {
        const float RC = oneDivPI2 / cutoff;
        const float dt = 1.0f / sampleRate;
        const float alpha = RC / (RC + dt);

        float prev = signal[0];
        float tmp;

        for (size_t i = 1; i < signalSize; ++i) {
            tmp = signal[i];
            signal[i] = alpha * (float) (signal[i - 1] + signal[i] - prev);
            prev = tmp;
        }

        // Fix the first value to conform scale
        signal[0] = signal[1];
    }

    template<typename T>
    void firstOrderDiffFilter(const T* inputSignal, size_t signalSize, size_t order, float* outputSignal) {
        const float coeff = 1.0f / (float) (order * (order + 1));

        for (size_t i = 0; i < signalSize; ++i) {
            for (size_t j = 1; j < order + 1; ++j) {
                outputSignal[i] += coeff * (inputSignal[std::min(i + j, signalSize - 1)] -
                                            inputSignal[std::max((int64_t) i - (int64_t) j, (int64_t) 0)]);
            }
        }
    }
}

namespace globus {
    template
    void lowPassFilter<>(int32_t *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    template
    void highPassFilter<>(int32_t *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    template
    void firstOrderDiffFilter<>(const int32_t *signal, size_t signalSize, size_t cutoff, float* outputSignal);

    template
    void lowPassFilter<>(float *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    template
    void highPassFilter<>(float *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    template
    void firstOrderDiffFilter<>(const float *signal, size_t signalSize, size_t cutoff, float* outputSignal);
}
