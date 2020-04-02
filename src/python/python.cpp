#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

#include <util/sht-reader.h>
#include <dsp/filter.h>
#include <sawtooth/sawtooth-functions.h>

using namespace globus;

PYBIND11_MODULE(pyglobus, m) {
    m.doc() = "pyglobus is a Python wrapper for Globus toolkit.";

    /***************
     * GLOBUS-UTIL *
     ***************/

    py::module util = m.def_submodule("util", "Wraps globus-util library.");

    py::class_<ShtReader>(util, "ShtReader")
            .def(py::init<const std::string &>())
            .def("get_signal", (ShtSignal (ShtReader::*)(const std::string& signalName)) &ShtReader::getSignal)
            .def("get_signal", (ShtSignal (ShtReader::*)(int32_t numSignal)) &ShtReader::getSignal)
            .def("get_signal_name", &ShtReader::getSignalName)
            .def("get_num_signals", &ShtReader::getNumSignals)
            .def("get_all_signals", &ShtReader::getAllSignals);

    py::class_<SystemTime>(util, "SystemTime")
            .def_readwrite("year", &SystemTime::year)
            .def_readwrite("month", &SystemTime::month)
            .def_readwrite("day_of_week", &SystemTime::dayOfWeek)
            .def_readwrite("day", &SystemTime::day)
            .def_readwrite("hour", &SystemTime::hour)
            .def_readwrite("minute", &SystemTime::minute)
            .def_readwrite("second", &SystemTime::second)
            .def_readwrite("milliseconds", &SystemTime::milliseconds);

    py::class_<ShtSignal>(util, "ShtSignal")
            .def_readwrite("type", &ShtSignal::type)
            .def_readwrite("name", &ShtSignal::name)
            .def_readwrite("comment", &ShtSignal::comment)
            .def_readwrite("unit", &ShtSignal::unit)
            .def_readwrite("time", &ShtSignal::time)
            .def_readwrite("num_channels", &ShtSignal::numChannels)
            .def_readwrite("tMin", &ShtSignal::tMin)
            .def_readwrite("tMax", &ShtSignal::tMax)
            .def_readwrite("uMin", &ShtSignal::uMin)
            .def_readwrite("delta", &ShtSignal::delta)
            .def_readwrite("data_size", &ShtSignal::dataSize)
            .def_property_readonly("data", [](ShtSignal& m) -> py::array_t<uint8_t> {
                return py::array_t<uint8_t>(
                    { m.dataSize },
                    { sizeof(uint8_t) },
                    m.data
                 );
            })
            .def("get_data_x", [](ShtSignal& m) -> py::array_t<float> {
                auto v = new std::vector<float>(m.getDataX());
                auto capsule = py::capsule(v, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
                return py::array(v->size(), v->data(), capsule);
            })
            .def("get_data_y", [](ShtSignal& m) -> py::array_t<float> {
                auto v = new std::vector<float>(m.getDataY());
                auto capsule = py::capsule(v, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
                return py::array(v->size(), v->data(), capsule);
            });

    /**************
     * GLOBUS-DSP *
     **************/

    py::module dsp = m.def_submodule("dsp", "Wraps globus-dsp library.");

    dsp.def("low_pass_filter", [](py::array_t<float>& m, size_t cutoff, size_t sample_rate) {
        lowPassFilter<float>(m.mutable_data(), m.size(), cutoff, sample_rate);
    });

    dsp.def("high_pass_filter", [](py::array_t<float>& m, size_t cutoff, size_t sample_rate) {
        highPassFilter<float>(m.mutable_data(), m.size(), cutoff, sample_rate);
    });

    dsp.def("first_order_diff_filter", [](py::array_t<float>& m, size_t order) -> py::array_t<float> {
        auto output = new std::vector<float>(m.size());

        firstOrderDiffFilter<float>(m.data(), m.size(), order, output->data());

        auto capsule = py::capsule(output, [](void *v) { delete reinterpret_cast<std::vector<float>*>(v); });
        return py::array(output->size(), output->data(), capsule);
    });

    /*******************
     * GLOBUS-SAWTOOTH *
     *******************/

    py::module sawtooth = m.def_submodule("sawtooth", "Wraps globus-sawtooth library.");

    sawtooth.def("get_signal_roi", [](py::array_t<float>& m, float mean_scale) -> std::pair<size_t, size_t> {
        return getSignalRoi(m.data(), m.size(), mean_scale);
    }, py::arg("signal") , py::arg("mean_scale") = 0.96);

    sawtooth.def("get_sawtooth_indexes", [](py::array_t<float>& m, float threshold) -> std::pair<size_t, size_t> {
        return getSawtoothIndexes(m.data(), m.size(), threshold);
    });
}
