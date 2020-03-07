add_library(globus-dsp STATIC filter.cpp basic.cpp)
target_link_libraries(globus-dsp INTERFACE kaldi-base)