#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

namespace globus {

    class ShtSignal {
    public:
        int32_t type;
        std::string name;
        std::string comment;
        std::string unit;
        //std::string time;
        int32_t year;
        int32_t month;
        int32_t weekDay;
        int32_t monthDay;
        int32_t hour;
        int32_t minute;
        int32_t second;
        int32_t mSecond;
        int32_t numChannels;
        float tMin;
        float tMax;
        float uMin;
        float delta;
        float *data;
    };

    class ShtReader {
    public:
        explicit ShtReader(const std::string &filePath);

        ShtSignal getSignal(const std::string& signalName);

        ShtSignal getSignal(const int32_t numSignal);

        const int32_t& getNumSignals() const;

        std::vector<ShtSignal> getAllSignals();

        ShtReader& operator=(ShtReader& other) = delete;
        ShtReader(ShtReader& other) = delete;

    protected:
        std::ifstream f;

        size_t version;
        int32_t numSignals;

        // Header stuff
        static const constexpr size_t lenVersion = 11;
        static const constexpr char* validVersion = "ANALIZER";
    };

}