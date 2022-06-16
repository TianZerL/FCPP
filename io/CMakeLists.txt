project(fcpp_io VERSION 1.0.0.0 LANGUAGES CXX)

if(FCPP_IO_SHARED_LIB)
    add_library(fcpp_io SHARED)
else()
    add_library(fcpp_io STATIC)
endif()

target_sources(fcpp_io PRIVATE
    ${TOP_DIR}/io/src/PaletteTable.cpp
)

target_include_directories(fcpp_io PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/io/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_compile_definitions(fcpp_io PUBLIC
    FCPP_IO_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

if(FCPP_IO_WITH_SFML2 OR FCPP_IO_WITH_SDL2 OR FCPP_IO_WITH_RAYLIB)
    target_include_directories(fcpp_io PUBLIC
        $<BUILD_INTERFACE:${TOP_DIR}/core/include>
    )
    target_link_libraries(fcpp_io PRIVATE fcpp_util)

    if(FCPP_IO_WITH_SFML2)
        target_sources(fcpp_io PRIVATE ${TOP_DIR}/io/src/SFML2Controller.cpp)
        target_link_libraries(fcpp_io PRIVATE
            sfml-graphics
            sfml-audio
            sfml-window
        )
        target_compile_definitions(fcpp_io PUBLIC FCPP_IO_SFML2)
    endif()
    if(FCPP_IO_WITH_SDL2)
        target_sources(fcpp_io PRIVATE ${TOP_DIR}/io/src/SDL2Controller.cpp)
        target_link_libraries(fcpp_io PRIVATE
            SDL2::SDL2
        )
        target_compile_definitions(fcpp_io PUBLIC FCPP_IO_SDL2)
    endif()
    if(FCPP_IO_WITH_RAYLIB)
        target_sources(fcpp_io PRIVATE ${TOP_DIR}/io/src/RayLibController.cpp)
        target_link_libraries(fcpp_io PRIVATE raylib)
        target_compile_definitions(fcpp_io PUBLIC FCPP_IO_RAYLIB)
    endif()
endif()

include(GenerateExportHeader)
generate_export_header(fcpp_io
    BASE_NAME "FCPP_IO"
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/FCPPIOExport.hpp
)
