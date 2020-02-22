#include <kaldi/util/parse-options.h>

#include <iostream>

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

        if (po.NumArgs() != 2) {
            po.PrintUsage();
            exit(1);
        }

    } catch(const std::exception &e) {
        std::cerr << e.what();
        return -1;
    }
}
