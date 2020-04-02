add_subdirectory(base)
add_subdirectory(bin)
add_subdirectory(dsp)
add_subdirectory(util)
add_subdirectory(sawtooth)

if (use_python)
    add_subdirectory(python)
endif()

add_subdirectory(kaldi)
