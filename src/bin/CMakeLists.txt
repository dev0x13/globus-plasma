add_executable(get-sht-info get-sht-info.cpp)
target_link_libraries(get-sht-info PRIVATE kaldi-util kaldi-base globus-util)

add_executable(detect-sawtooth-baseline detect-sawtooth-baseline.cpp)
target_link_libraries(detect-sawtooth-baseline PRIVATE kaldi-util kaldi-base globus-util globus-dsp)