project(fcpp_core VERSION 1.0.0.0 LANGUAGES CXX)

if(FCPP_SHARED_LIB)
    add_library(fcpp SHARED)
else()
    add_library(fcpp STATIC)
endif()

target_sources(fcpp PRIVATE
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

target_include_directories(fcpp PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/core/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:fcpp/include>
)

target_compile_definitions(fcpp PUBLIC
    FCPP_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

fcpp_check_disable_flags(fcpp)

set_target_properties(fcpp PROPERTIES EXPORT_NAME "Core")

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
)

install(DIRECTORY ${TOP_DIR}/core/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION fcpp)
