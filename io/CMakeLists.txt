project(fcpp_io VERSION 1.0.0.0 LANGUAGES CXX)

if(FCPP_SHARED_LIB)
    add_library(fcpp_io SHARED)
else()
    add_library(fcpp_io STATIC)
endif()

target_sources(fcpp_io PRIVATE
    ${TOP_DIR}/io/src/Manager.cpp
    ${TOP_DIR}/io/src/PaletteTable.cpp
)

target_include_directories(fcpp_io PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/io/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:fcpp_io/include>
)

target_compile_definitions(fcpp_io PUBLIC
    FCPP_IO_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

fcpp_check_disable_flags(fcpp_io)

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
        if (APPLE)
            target_link_libraries(fcpp_io PRIVATE "-framework IOKit")
            target_link_libraries(fcpp_io PRIVATE "-framework Cocoa")
            target_link_libraries(fcpp_io PRIVATE "-framework OpenGL")
        endif()
    endif()
endif()

set_target_properties(fcpp_io PROPERTIES EXPORT_NAME "IO")

include(GenerateExportHeader)
generate_export_header(fcpp_io
    BASE_NAME "FCPP_IO"
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/FCPPIOExport.hpp
)

if(NOT CHECK_NOT_EXPORT_FCPP_IO)
    install(
        TARGETS fcpp_io EXPORT FCPP_IO
        ARCHIVE DESTINATION fcpp_io/lib
        LIBRARY DESTINATION fcpp_io/lib
        RUNTIME DESTINATION bin
    )

    install(DIRECTORY ${TOP_DIR}/io/include ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION fcpp_io)

    install(
        EXPORT FCPP_IO
        FILE FCPPIOTargets.cmake
        NAMESPACE FCPP::
        DESTINATION fcpp_io/cmake
    )

    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/FCPPIOConfig.cmake.in" [[
    @PACKAGE_INIT@

    if(NOT @FCPP_SHARED_LIB@ AND @FCPP_IO_WITH_SFML2@ OR @FCPP_IO_WITH_SDL2@ OR @FCPP_IO_WITH_RAYLIB@)
        include(CMakeFindDependencyMacro)
        if(@FCPP_IO_WITH_SFML2@)
            find_dependency(SFML 2 COMPONENTS graphics audio window REQUIRED)
        endif()
        if(@FCPP_IO_WITH_SDL2@)
            find_dependency(SDL2 REQUIRED)
        endif()
        if(@FCPP_IO_WITH_RAYLIB@)
            find_dependency(raylib REQUIRED)
        endif()
    endif()

    include("${CMAKE_CURRENT_LIST_DIR}/FCPPIOTargets.cmake")
    check_required_components(MathFunctions)
    ]])

    include(CMakePackageConfigHelpers)
    configure_package_config_file(${CMAKE_CURRENT_BINARY_DIR}/FCPPIOConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/FCPPIOConfig.cmake"
        INSTALL_DESTINATION fcpp_io/cmake
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FCPPIOConfig.cmake"
        DESTINATION fcpp_io/cmake
    )
endif()
