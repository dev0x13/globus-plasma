#include <kaldi/util/parse-options.h>
#include <util/sht-reader.h>
#include <util/text-utils.h>
#include <util/csv-writer.h>
#include <dsp/filter.h>
#include <dsp/basic.h>
#include <sawtooth/sawtooth-functions.h>

#include <iostream>

struct BaselineLabelingOptions {
    size_t signalSamplingRateHz       = 1e6;
    size_t highPassCutoffHz           = 400;
    size_t lowPassCutoffHz            = 5000;
    size_t differentiatorFilterOrder  = 30;
    float  sawtoothDetectionThreshold = 0.0005;
    float  roiDetectorMeanScale       = 1;

    void registerOptions(kaldi::OptionsItf& opts) {
        opts.Register("signal_sampling_rate_hz", &signalSamplingRateHz, "Signal sampling rate, Hz");
        opts.Register("high_pass_cutoff_hz", &highPassCutoffHz, "High pass filter cutoff frequency, Hz");
        opts.Register("low_pass_cutoff_hz", &lowPassCutoffHz, "Low pass filter cutoff frequency, Hz");
        opts.Register("differentiator_filter_order", &differentiatorFilterOrder, "Differentiator filter order");
        opts.Register("sawtooth_detection_threshold", &sawtoothDetectionThreshold, "Threshold applied at final step to make a detection");
        opts.Register("roi_detector_mean_scale", &roiDetectorMeanScale, "Mean scale used for ROI detection");
    }
};

std::vector<int32_t> prepareSignalsNumbers(const std::string& signalsStr) {
    if (signalsStr.empty()) {
        KALDI_ERR << "Signals to process should be specified";
    } else {
        return globus::splitStringToNumbers<int32_t>(signalsStr, ",");
    }
}

int main(int argc, char *argv[]) {
    try {
        // Options stuff
        using namespace kaldi;

        const char *usage =
            "Create sawtooth label for a X-Ray data.\n"
            "Usage: detect-sawtooth-baseline [options...] <input-sht-file> <output-csv-file>\n";

        ParseOptions po(usage);

        std::string signalsStr;
        BaselineLabelingOptions options;

        po.Register("signals", &signalsStr, "Comma-separated list that specifies "
            "which signals from SHT files should be processed. E.g. --signals=1,2,3");
        options.registerOptions(po);

        po.Read(argc, argv);

        if (po.NumArgs() != 2) {
            po.PrintUsage();
            exit(1);
        }

        std::vector<int32_t> signalsToProcess = prepareSignalsNumbers(signalsStr);

        // Create SHT reader and CSV writer
        globus::ShtReader shtReader(po.GetArg(1));
        globus::CsvWriter csvWriter(po.GetArg(2), 3);

        // CSV header
        csvWriter << "signal" << "sawtooth_start" << "sawtooth_end";

        for (const int32_t& numSignal : signalsToProcess) {
            KALDI_LOG << "Processing signal #" << numSignal << ": " << shtReader.getSignalName(numSignal);

            // 0. Obtain signal data

            const globus::ShtSignal signal = shtReader.getSignal(numSignal);

            std::vector<float> y = signal.getDataY();

            // 1. Select ROI

            const std::pair<int32_t, int32_t> signalRoiBounds =
                globus::getSignalRoi(y.data(), y.size(), options.roiDetectorMeanScale);

            float* signalRoi     = y.data() + signalRoiBounds.first;
            size_t signalRoiSize = signalRoiBounds.second - signalRoiBounds.first;

            // 2. Apply high pass filter

            globus::highPassFilter(signalRoi, signalRoiSize, options.highPassCutoffHz, options.signalSamplingRateHz);

            // 3. Compute first derivative

            std::vector<float> signalRoiFirstDerivative(signalRoiSize);

            globus::firstOrderDiffFilter(
                signalRoi,
                signalRoiSize,
                options.differentiatorFilterOrder,
                signalRoiFirstDerivative.data()
            );

            // 4. Apply abs

            globus::abs(signalRoiFirstDerivative.data(), signalRoiSize);

            // 5. Apply low pass filter

            globus::lowPassFilter(
                signalRoiFirstDerivative.data(),
                signalRoiSize,
                options.lowPassCutoffHz,
                options.signalSamplingRateHz
            );

            // 6. Construct sawtooth indexes

            const std::pair<size_t, size_t> detection =
                globus::getSawtoothIndexes(signalRoiFirstDerivative.data(), signalRoiSize, options.sawtoothDetectionThreshold);

            if (detection.first == detection.second) {
                // No detection
                KALDI_LOG << "Sawtooth crash was not detected";
                continue;
            } else {
                // Detected - transform relative sawtooth indexes to absolute ones
                std::vector<float> time = signal.getDataX();

                const size_t sawtoothAbsStartIndex = signalRoiBounds.first + detection.first;
                const size_t sawtoothAbsEndIndex   = signalRoiBounds.first + detection.second;

                KALDI_LOG << "Detected sawtooth crash at ("
                          << time[sawtoothAbsStartIndex] << ", "
                          << time[sawtoothAbsEndIndex] << ")";

                csvWriter << numSignal
                          << sawtoothAbsStartIndex
                          << sawtoothAbsEndIndex;
            }
        }
    } catch(const std::exception &e) {
        std::cerr << e.what();
        return -1;
    }
}
