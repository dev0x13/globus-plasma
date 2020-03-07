#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <kaldi/base/kaldi-error.h>

namespace globus {

    // Simple CSV writer class
    class CsvWriter {
    public:
        CsvWriter(const std::string& filePath, size_t numColumns, const std::string& separator = ",")
                : f(filePath), numColumns(numColumns), separator(separator) {
            if (!f.is_open()) {
                KALDI_ERR << "Unable to open CSV file: " << filePath;
            }
        }

        template<typename T>
        CsvWriter &operator<<(const T &t) {
            if (numValuesInRow == numColumns) {
                f << std::endl;
                numValuesInRow = 0;
            }

            if (numValuesInRow++ != 0) {
                f << separator;
            }

            f << t;

            return *this;
        }

    private:
        std::ofstream f;

        const size_t numColumns;
        size_t numValuesInRow = 0;
        const std::string separator;
    };

}