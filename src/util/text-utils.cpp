#include "text-utils.h"

namespace globus {
    std::vector<std::string> splitStringToVector(const std::string& str, const std::string& delimiter) {
        size_t start = 0, found = 0, end = str.size();

        std::vector<std::string> out;

        while (found != std::string::npos) {
            found = str.find_first_of(delimiter, start);

            if (found != start && start != end) {
                out.emplace_back(str.substr(start, found - start));
            }

            start = found + 1;
        }

        return out;
    }
}