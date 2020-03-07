#pragma once

#include "sht-reader.h"

namespace globus_internal {
    struct Histogram {
        static const constexpr size_t stringFieldsLength = 128;
        static const constexpr size_t headerLength       = 1024;

        int32_t type;
        char name[stringFieldsLength];
        char comment[stringFieldsLength];
        char unit[stringFieldsLength];
        globus::SystemTime time;
        int32_t numChannels;
        double tMin;
        double tMax;
        double uMin;
        double delta;
        int64_t data;

        static Histogram* decompress(uint8_t* H, int32_t Size);
        static std::string decompressName(uint8_t* H);
        int32_t getDataSize() const;
    };
}
