#include "kaldi/base/kaldi-error.h"
#include "sht-reader.h"

using namespace globus;

// Basic test for SHT reader. Reads a single SHT from /data/test/sht-reader folder
// and checks its equality to ground truth/
int main() {
    ShtReader shtReader(std::string(GLOBUS_ROOT) + "/data/test/sht-reader/sht38515.sht");

    KALDI_ASSERT(shtReader.getNumSignals() == 86);

    ShtSignal signal75 = shtReader.getSignal(75);

    std::ifstream input( std::string(GLOBUS_ROOT) + "/data/test/sht-reader/sht38515_signal75_data.bin", std::ios::binary);
    KALDI_ASSERT(input.is_open());

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

    KALDI_ASSERT(signal75.dataSize          == buffer.size());
    KALDI_ASSERT(signal75.type              == 0);
    KALDI_ASSERT(signal75.tMax              == 0.36212999999999995);
    KALDI_ASSERT(signal75.tMin              == 0.1);
    KALDI_ASSERT(signal75.numChannels       == 262130);
    KALDI_ASSERT(signal75.delta             == 335248562.9034082);
    KALDI_ASSERT(signal75.uMin              == -2917719939412529.5);

    KALDI_ASSERT(signal75.time.year         == 2019);
    KALDI_ASSERT(signal75.time.month        == 11);
    KALDI_ASSERT(signal75.time.day          == 19);
    KALDI_ASSERT(signal75.time.dayOfWeek    == 2);
    KALDI_ASSERT(signal75.time.hour         == 17);
    KALDI_ASSERT(signal75.time.minute       == 37);
    KALDI_ASSERT(signal75.time.second       == 27);
    KALDI_ASSERT(signal75.time.milliseconds == 531);

    for (size_t i = 0; i < signal75.dataSize; ++i) {
        KALDI_ASSERT(signal75.data[i] == buffer[i]);
    }

    // We don't check string fields of ShtSignal, because they are encoded in CP1251

    shtReader.getAllSignals();

    return 0;
}