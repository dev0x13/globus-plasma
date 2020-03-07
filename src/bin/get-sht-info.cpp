#include <kaldi/util/parse-options.h>
#include <util/sht-reader.h>

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        using namespace kaldi;

        const char *usage =
            "Output SHT file info.\n"
            "Usage:  get-sht-info <sht-file>\n";

        ParseOptions po(usage);

        po.Read(argc, argv);

        if (po.NumArgs() != 1) {
            po.PrintUsage();
            exit(1);
        }

        globus::ShtReader shtReader(po.GetArg(1));

        const size_t numSignals = shtReader.getNumSignals();

        KALDI_LOG << "Found " << numSignals << " signals";

        for (size_t i = 0; i < numSignals; ++i ) {
            KALDI_LOG << "#" << i << ": " << shtReader.getSignalName(i);
        }
    } catch(const std::exception &e) {
        std::cerr << e.what();
        return -1;
    }
}
