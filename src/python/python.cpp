#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

#include "util/sht-reader.h"

using namespace globus;

PYBIND11_MODULE(pyglobus, m) {
    m.doc() = "TBD";

    py::class_<ShtReader>(m, "ShtReader")
            .def(py::init<const std::string &>())
            .def("get_signal", (ShtSignal (ShtReader::*)(const std::string& signalName)) &ShtReader::getSignal)
            .def("get_signal", (ShtSignal (ShtReader::*)(int32_t numSignal)) &ShtReader::getSignal)
            .def("get_signal_name", &ShtReader::getSignalName)
            .def("get_num_signals", &ShtReader::getNumSignals)
            .def("get_all_signals", &ShtReader::getAllSignals);

    py::class_<SystemTime>(m, "SystemTime")
            .def_readwrite("year", &SystemTime::year)
            .def_readwrite("month", &SystemTime::month)
            .def_readwrite("day_of_week", &SystemTime::dayOfWeek)
            .def_readwrite("day", &SystemTime::day)
            .def_readwrite("hour", &SystemTime::hour)
            .def_readwrite("minute", &SystemTime::minute)
            .def_readwrite("second", &SystemTime::second)
            .def_readwrite("milliseconds", &SystemTime::milliseconds);

    py::class_<ShtSignal>(m, "ShtSignal")
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
}
