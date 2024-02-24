project(cfcpp VERSION 0.0.1.0 LANGUAGES CXX)

add_library(cfcpp SHARED ${TOP_DIR}/bind/c/src/CFCPP.cpp)

target_link_libraries(cfcpp PRIVATE fcpp)

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
    RUNTIME DESTINATION cfcpp/lib
)

install(DIRECTORY ${TOP_DIR}/bind/c/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION cfcpp)

install(
    EXPORT CFCPP
    FILE CFCPPConfig.cmake
    NAMESPACE FCPP::
    DESTINATION cfcpp/cmake
)
