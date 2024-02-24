project(fcpp_test_core VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(fcpp_test_core
    ${TOP_DIR}/test/core/src/Main.cpp
)

target_link_libraries(fcpp_test_core PRIVATE fcpp)

target_compile_definitions(fcpp_test_core PRIVATE
    TEST_ROM_LOAD_PATH="${TEST_ROM_PATH}"
)

install(
    TARGETS fcpp_test_core
    RUNTIME DESTINATION test/core
)
