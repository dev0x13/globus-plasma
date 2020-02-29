#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

#include "kaldi/base/kaldi-error.h"

namespace globus {

    // Reflect Windows SYSTEMTIME
    // https://docs.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-systemtime
    struct SystemTime {
        uint16_t year         = 0;
        uint16_t month        = 0;
        uint16_t dayOfWeek    = 0;
        uint16_t day          = 0;
        uint16_t hour         = 0;
        uint16_t minute       = 0;
        uint16_t second       = 0;
        uint16_t milliseconds = 0;

        // TODO: operator<<
    };

    // Stores a single SHT signal (aka histogram)
    struct ShtSignal {
        // 0 - int32_t, 1 - 2xDouble, 2 - 3xDouble
        int32_t type = 0;

        // Be careful: these strings contents are encoded in CP1251
        std::string name;
        std::string comment;
        std::string unit;

        SystemTime time;
        int32_t numChannels = 0;
        double tMin  = 0;
        double tMax  = 0;
        double uMin  = 0;
        double delta = 0;

        // At the moment user is responsible for handling real data type
        // (i.e. casting this pointer to int32_t and double)
        uint8_t *data = nullptr;
        size_t dataSize = 0;

        ShtSignal() = default;

        ShtSignal(const ShtSignal& other) {
            type = other.type;
            name = other.name;
            comment = other.comment;
            time = other.time;
            numChannels = other.numChannels;
            tMin = other.tMin;
            tMax = other.tMax;
            uMin = other.uMin;
            delta = other.delta;
            dataSize = other.dataSize;
            data = new uint8_t[dataSize];
            std::memcpy(data, other.data, dataSize);
        }

        ShtSignal& operator=(const ShtSignal& other) {
            if (&other == this) {
                return *this;
            }

            type = other.type;
            name = other.name;
            comment = other.comment;
            time = other.time;
            numChannels = other.numChannels;
            tMin = other.tMin;
            tMax = other.tMax;
            uMin = other.uMin;
            delta = other.delta;
            dataSize = other.dataSize;
            data = new uint8_t[dataSize];
            std::memcpy(data, other.data, dataSize);

            return *this;
        }

        ~ShtSignal() {
            delete[] data;
        }

        // TODO: operator<<
    };

    class ShtReader {
    public:
        explicit ShtReader(const std::string &filePath);

        // Leave this unimplemented for now
        ShtSignal getSignal(const std::string& signalName) {
            throw std::logic_error("Not implemented");
        }

        // Reads and returns #numSignal signal from SHT
        ShtSignal getSignal(int32_t numSignal);

        // Returns total number of signals presented in SHT
        const int32_t& getNumSignals() const;

        // Reads and returns all signals from SHT
        std::vector<ShtSignal> getAllSignals();

        ShtReader& operator=(ShtReader& other) = delete;
        ShtReader(ShtReader& other) = delete;

    protected:
        std::ifstream f;

        size_t version;
        int32_t numSignals;

        std::vector<size_t> signalsOffsets;
        std::vector<int32_t> signalsSizes;

        // Header stuff
        static const constexpr size_t lenVersion = 11;
        static const constexpr char* validVersion = "ANALIZER";
    };
}