project(efcpp VERSION 1.0.0.0 LANGUAGES CXX)

add_library(efcpp STATIC
    ${TOP_DIR}/wasm/src/Emulator.cpp
)

target_include_directories(efcpp PUBLIC
    $<BUILD_INTERFACE:${TOP_DIR}/wasm/include>
)

target_link_libraries(efcpp PRIVATE fcpp)

target_compile_definitions(efcpp PUBLIC
    EFCPP_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

if(EMSCRIPTEN)
    target_compile_options(efcpp PRIVATE -sUSE_SDL=2)
    target_link_options(efcpp PUBLIC -sUSE_SDL=2)
else()
    target_link_libraries(efcpp PRIVATE
        SDL2::SDL2
    )
endif()