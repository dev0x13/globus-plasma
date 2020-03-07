#pragma once

#include <cstddef>

namespace globus {
    // Computes mean of the signal
    template <typename T>
    T mean(const T* signal, size_t signalSize);

    // Applies in-place abs
    template <typename T>
    void abs(T* signal, size_t signalSize);
}