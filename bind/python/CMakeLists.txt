project(pyfcpp VERSION 0.0.1.0 LANGUAGES CXX)

find_package(pybind11 QUIET)
if(NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
    )
    FetchContent_GetProperties(pybind11)
    if(NOT pybind11_POPULATED)
        FetchContent_Populate(pybind11)
        add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
    endif()
endif()

pybind11_add_module(pyfcpp
    ${TOP_DIR}/bind/python/src/PyFCPP.cpp
    ${TOP_DIR}/bind/python/src/Core.cpp
    ${TOP_DIR}/bind/python/src/IO.cpp
)

target_link_libraries(pyfcpp PRIVATE fcpp fcpp_io)

install(
    TARGETS pyfcpp
    ARCHIVE DESTINATION pyfcpp
    LIBRARY DESTINATION pyfcpp
    RUNTIME DESTINATION pyfcpp
)
