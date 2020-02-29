#include "sht-reader.h"
#include "compress.h"

#include <kaldi/base/kaldi-error.h>

using namespace globus;

ShtReader::ShtReader(const std::string& filePath) {
    f.open(filePath, std::ios::binary);

    if (!f.is_open()) {
        KALDI_ERR << "Unable to open SHT file: " << filePath;
    }

    // 1. Read header

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

    // 2. Read signals meta

    signalsOffsets.resize(numSignals);
    signalsSizes.resize(numSignals);

    for (size_t i = 0; i < (size_t) numSignals; ++i) {
        f.read(reinterpret_cast<char*>(&signalsSizes[i]), sizeof(signalsSizes[i]));
        signalsOffsets[i] = f.tellg();
        f.seekg(signalsSizes[i], std::ios_base::cur);
    }
}

const int32_t& ShtReader::getNumSignals() const {
    return numSignals;
}

ShtSignal ShtReader::getSignal(const int32_t numSignal) {
    KALDI_ASSERT(numSignal >= 0 && numSignal < numSignals);

    // 1. Read raw signal data

    f.seekg(signalsOffsets[numSignal]);
    char* buf = new char[signalsSizes[numSignal]];
    f.read(buf, signalsSizes[numSignal]);

    // 2. Decompress histogram

    globus_internal::Histogram* h = globus_internal::Histogram::decompress(reinterpret_cast<uint8_t*>(buf), signalsSizes[numSignal]);
    delete[] buf;

    KALDI_ASSERT(h);

    // 2. Construct high-level SHT signal from the histogram

    ShtSignal result;

    result.numChannels = h->numChannels;
    result.time        = h->time;
    result.comment     = h->comment;
    result.delta       = h->delta;
    result.uMin        = h->uMin;
    result.name        = h->name;
    result.dataSize    = h->getDataSize();
    result.unit        = h->unit;
    result.tMax        = h->tMax;
    result.tMin        = h->tMin;
    result.type        = h->type >> 16;
    result.data        = new uint8_t[result.dataSize];
    std::memcpy(result.data, &h->data, result.dataSize);

    delete reinterpret_cast<uint8_t *>(h);

    return result;
}

std::vector<ShtSignal> ShtReader::getAllSignals() {
    std::vector<ShtSignal> result;

    for (size_t i = 0; i < (size_t) numSignals; ++i) {
        result.emplace_back(getSignal(i));
    }

    return result;
}