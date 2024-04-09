project(fcpp_test_debugger VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(fcpp_test_debugger
    ${TOP_DIR}/test/debugger/src/IO.cpp
    ${TOP_DIR}/test/debugger/src/Main.cpp
)

target_include_directories(fcpp_test_debugger PRIVATE
    $<BUILD_INTERFACE:${TOP_DIR}/test/debugger/include>
)

target_link_libraries(fcpp_test_debugger PRIVATE
    fcpp
    fcpp_util
    fcpp_tools
    SDL2::SDL2
)

target_compile_definitions(fcpp_test_debugger PRIVATE
    TEST_ROM_LOAD_PATH="${TEST_ROM_PATH}"
)

install(
    TARGETS fcpp_test_debugger
    RUNTIME DESTINATION test/debugger
)
