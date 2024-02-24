project(fcpp_test_debugger VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(fcpp_test_debugger
    ${TOP_DIR}/test/debugger/src/Main.cpp
)

target_link_libraries(fcpp_test_debugger PRIVATE
    fcpp
    fcpp_io
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
