#include "sht-reader.h"

#include <kaldi/base/kaldi-error.h>

using namespace globus;

ShtReader::ShtReader(const std::string& filePath) {
    f.open(filePath, std::ios::binary);

    if (!f.is_open()) {
        KALDI_ERR << "Unable to open SHT file: " << filePath;
    }

    char versionStr[lenVersion];

    f.read(versionStr, lenVersion);

    if (strncmp(versionStr, validVersion, strlen(validVersion)) == 0) {
       if (strncmp(&versionStr[lenVersion - 1], "0", 1) == 0) {
           KALDI_ERR << "Unsupported version of SHT file: 0";
       } else if (strncmp(&versionStr[lenVersion - 1], "1", 1) == 0) {
           KALDI_ERR << "Unsupported version of SHT file: 1";
       } else if (strncmp(&versionStr[lenVersion - 1], "2", 1) == 0) {
           version = 2;
       } else {
           KALDI_ERR << "Unknown version of SHT file: " << versionStr[lenVersion - 1];
       }
    } else {
        KALDI_ERR << "Unknown version header of SHT file: " << versionStr;
    }

    f.seekg(1, std::ios_base::cur);

    f.read(reinterpret_cast<char*>(&numSignals), sizeof(numSignals));

    KALDI_LOG << "Found " << numSignals << " signals";
}

const int32_t& ShtReader::getNumSignals() const {
    return numSignals;
}