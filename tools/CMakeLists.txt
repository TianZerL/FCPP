project(fcpp_tools VERSION 1.0.0.0 LANGUAGES CXX)

if(FCPP_SHARED_LIB)
    add_library(fcpp_tools SHARED)
else()
    add_library(fcpp_tools STATIC)
endif()

target_sources(fcpp_tools PRIVATE
    ${TOP_DIR}/tools/src/Debugger.cpp
)

target_include_directories(fcpp_tools PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/tools/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:fcpp/include>
)

target_link_libraries(fcpp_tools PRIVATE fcpp)

target_compile_definitions(fcpp_tools PUBLIC
    FCPP_TOOLS_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

set_target_properties(fcpp_tools PROPERTIES EXPORT_NAME "Tools")

include(GenerateExportHeader)
generate_export_header(fcpp_tools
    BASE_NAME "FCPP_TOOLS"
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/FCPPTOOLSExport.hpp
)

install(
    TARGETS fcpp_tools EXPORT FCPP
    ARCHIVE DESTINATION fcpp/lib
    LIBRARY DESTINATION fcpp/lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${TOP_DIR}/tools/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION fcpp)
