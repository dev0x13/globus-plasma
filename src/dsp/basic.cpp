#include "basic.h"

#include <cmath>
#include <cstdint>

namespace globus {
    template <typename T>
    T mean(const T* signal, size_t signalSize) {
        double mean = 0;

        for (size_t i = 0; i < signalSize; ++i) {
            mean += signal[i];
        }

        mean /= signalSize;

        return (T) mean;
    }

    template <typename T>
    void abs(T* signal, size_t signalSize) {
        for (size_t i = 0; i < signalSize; ++i) {
            signal[i] = std::abs(signal[i]);
        }
    }
}

namespace globus {
    template
    int32_t mean<>(const int32_t* signal, size_t signalSize);

    template
    void abs<>(int32_t* signal, size_t signalSize);

    template
    float mean<>(const float* signal, size_t signalSize);

    template
    void abs<>(float* signal, size_t signalSize);
}