#include <kaldi/util/parse-options.h>

#include <iostream>
#include <util/sht-reader.h>

int main(int argc, char *argv[]) {
    try {
        using namespace kaldi;
        const char *usage =
                "Create sawtooth label for a X-Ray data.\n"
                "Usage:  baseline-label-sht [options...] <input-sht-file>\n";

        // Construct all the global objects.
        ParseOptions po(usage);

        std::string inputFile;

        // Register the options.
        po.Register("sample-param", &inputFile, "Sample param");

        po.Read(argc, argv);

        if (po.NumArgs() != 1) {
            po.PrintUsage();
            exit(1);
        }

        globus::ShtReader(po.GetArg(1));

    } catch(const std::exception &e) {
        std::cerr << e.what();
        return -1;
    }
}
