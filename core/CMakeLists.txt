project(fcpp_core VERSION 1.0.0.0 LANGUAGES CXX)

set(FCPP_CORE_SRCS
    ${TOP_DIR}/core/src/APU.cpp
    ${TOP_DIR}/core/src/Bus.cpp
    ${TOP_DIR}/core/src/Cartridge.cpp
    ${TOP_DIR}/core/src/Clock.cpp
    ${TOP_DIR}/core/src/CPU.cpp
    ${TOP_DIR}/core/src/FC.cpp
    ${TOP_DIR}/core/src/INES.cpp
    ${TOP_DIR}/core/src/Joypad.cpp
    ${TOP_DIR}/core/src/PPU.cpp
    ${TOP_DIR}/core/src/Snapshot.cpp
)

if(FCPP_FCPP_SHARED_LIB)
    add_library(fcpp SHARED ${FCPP_CORE_SRCS})
else()
    add_library(fcpp STATIC ${FCPP_CORE_SRCS})
endif()

target_include_directories(fcpp PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/core/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:fcpp/include>
)

target_compile_definitions(fcpp PUBLIC
    FCPP_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

include(GenerateExportHeader)
generate_export_header(fcpp
    BASE_NAME "FCPP"
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/FCPPExport.hpp
)

install(
    TARGETS fcpp EXPORT FCPP
    ARCHIVE DESTINATION fcpp/lib
    LIBRARY DESTINATION fcpp/lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION fcpp/include
)

install(DIRECTORY ${TOP_DIR}/core/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION fcpp)

install(
    EXPORT FCPP
    FILE FCPPConfig.cmake
    NAMESPACE FCPP::
    DESTINATION fcpp/cmake
)
