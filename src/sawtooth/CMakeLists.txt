add_library(globus-sawtooth STATIC sawtooth-functions.cpp)
target_link_libraries(globus-sawtooth INTERFACE kaldi-base globus-dsp)