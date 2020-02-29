#pragma once

#include "sht-reader.h"

namespace globus_internal {
    struct Histogram {
        int32_t type;
        char name[128];
        char comment[128];
        char unit[128];
        globus::SystemTime time;
        int32_t numChannels;
        double tMin;
        double tMax;
        double uMin;
        double delta;
        int64_t data;

        static Histogram* decompress(uint8_t* H, int32_t Size);
        int32_t getDataSize() const;
    };
}
