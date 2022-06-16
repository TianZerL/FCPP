project(cfcpp VERSION 0.0.1.0 LANGUAGES CXX)

add_library(cfcpp SHARED ${TOP_DIR}/bind/c/src/CFCPP.cpp)

if (FCPP_CORE_SHARED_LIB)
    target_link_libraries(cfcpp PRIVATE fcpp)
else()
    get_target_property(CFCPP_CORE_INCLUDE_DIRECTORIES fcpp INCLUDE_DIRECTORIES)
    target_include_directories(cfcpp PRIVATE ${CFCPP_CORE_INCLUDE_DIRECTORIES})
    get_target_property(CFCPP_CORE_SOURCES fcpp SOURCES)
    target_sources(cfcpp PRIVATE ${CFCPP_CORE_SOURCES})
endif()

target_include_directories(cfcpp PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/bind/c/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:cfcpp/include>
)

include(GenerateExportHeader)
generate_export_header(cfcpp
    BASE_NAME "CFCPP"
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/CFCPPExport.h
)

install(
    TARGETS cfcpp EXPORT CFCPP
    ARCHIVE DESTINATION cfcpp/lib
    LIBRARY DESTINATION cfcpp/lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION cfcpp/include
)

install(DIRECTORY ${TOP_DIR}/bind/c/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION cfcpp)

install(
    EXPORT CFCPP
    FILE CFCPPConfig.cmake
    NAMESPACE FCPP::
    DESTINATION cfcpp/cmake
)
