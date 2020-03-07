#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <kaldi/base/kaldi-error.h>

namespace globus {
    std::vector<std::string> splitStringToVector(const std::string& str, const std::string& delimiter);

    template<typename T>
    std::vector<T> splitStringToNumbers(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> split = splitStringToVector(str, delimiter);

        std::vector<T> result(split.size());

        for (size_t i = 0; i < split.size(); ++i) {
            std::istringstream ss(split[i]);
            ss >> result[i];

            if (ss.fail()) {
                KALDI_ERR << "Unable to convert string to number: " << split[i];
            }
        }

        return result;
    }
}