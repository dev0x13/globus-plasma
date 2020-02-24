add_executable(baseline-label-sht baseline-label-sht.cpp)
target_link_libraries(baseline-label-sht PRIVATE kaldi-util kaldi-base globus-util)