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

if(FCPP_CORE_SHARED_LIB)
    target_link_libraries(pyfcpp PRIVATE fcpp)
else()
    get_target_property(PYFCPP_CORE_INCLUDE_DIR fcpp INCLUDE_DIRECTORIES)
    target_include_directories(pyfcpp PRIVATE ${PYFCPP_CORE_INCLUDE_DIR})
    get_target_property(PYFCPP_CORE_SOURCES fcpp SOURCES)
    target_sources(pyfcpp PRIVATE ${PYFCPP_CORE_SOURCES})
endif()

if(FCPP_IO_SHARED_LIB)
    target_link_libraries(pyfcpp PRIVATE fcpp_io)
else()
    get_target_property(PYFCPP_IO_SOURCES fcpp_io SOURCES)
    target_sources(pyfcpp PRIVATE ${PYFCPP_IO_SOURCES})
    get_target_property(PYFCPP_IO_INCLUDE_DIRECTORIES fcpp_io INCLUDE_DIRECTORIES)
    target_include_directories(pyfcpp PRIVATE ${PYFCPP_IO_INCLUDE_DIRECTORIES})
    get_target_property(PYFCPP_IO_COMPILE_DEFINITIONS fcpp_io COMPILE_DEFINITIONS)
    target_compile_definitions(pyfcpp PRIVATE ${PYFCPP_IO_COMPILE_DEFINITIONS})
    get_target_property(PYFCPP_IO_LINK_LIBRARIES fcpp_io LINK_LIBRARIES)
    target_link_libraries(pyfcpp PRIVATE ${PYFCPP_IO_LINK_LIBRARIES})
endif()

install(
    TARGETS pyfcpp
    ARCHIVE DESTINATION pyfcpp
    LIBRARY DESTINATION pyfcpp
    RUNTIME DESTINATION pyfcpp
)
