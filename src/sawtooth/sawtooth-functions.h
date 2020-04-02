#pragma once

#include <utility>
#include <cstddef>

namespace globus {
    // Returns signal ROI start and end indexes
    std::pair<size_t, size_t> getSignalRoi(const float* signal, size_t signalSize, float meanScale = 0.96);

    // Applies threshold to processed data and return relative (in ROI domain) indexes of sawtooth start and end
    std::pair<size_t, size_t> getSawtoothIndexes(const float* signal, size_t signalSize, float threshold);
}