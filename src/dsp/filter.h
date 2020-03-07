#pragma once

#include <cstddef>
#include <cstdint>

namespace globus {
    // In-place low pass filter
    template <typename T>
    void lowPassFilter(T *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    // In-place high pass filter
    template <typename T>
    void highPassFilter(T *signal, size_t signalSize, size_t cutoff, size_t sampleRate);

    // First order differentiator
    // Note: outputSignal should be allocated to fit signalSize values
    template <typename T>
    void firstOrderDiffFilter(const T* inputSignal, size_t signalSize, size_t order, float* outputSignal);
}