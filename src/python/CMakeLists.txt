pybind11_add_module(pyglobus python.cpp)
target_link_libraries(pyglobus PRIVATE globus-util)

install(TARGETS pyglobus LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/python)
