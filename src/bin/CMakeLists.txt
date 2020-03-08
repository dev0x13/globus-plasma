add_executable(get-sht-info get-sht-info.cpp)
target_link_libraries(get-sht-info PRIVATE kaldi-util kaldi-base globus-util)
install(TARGETS get-sht-info RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

add_executable(detect-sawtooth-baseline detect-sawtooth-baseline.cpp)
target_link_libraries(detect-sawtooth-baseline PRIVATE kaldi-util kaldi-base globus-util globus-dsp)
install(TARGETS detect-sawtooth-baseline DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
