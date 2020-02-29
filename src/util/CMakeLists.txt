add_library(globus-util STATIC sht-reader.cpp compress.cpp)
target_link_libraries(globus-util INTERFACE kaldi-base)

add_executable(sht-reader-test sht-reader-test.cpp)
target_link_libraries(sht-reader-test PRIVATE globus-util)
target_compile_options(sht-reader-test PRIVATE -DGLOBUS_ROOT="${CMAKE_CURRENT_LIST_DIR}/../..")