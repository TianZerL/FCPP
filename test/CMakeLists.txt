project(fcpp_test VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(fcpp_test
    src/Test.cpp
)

target_link_libraries(fcpp_test PRIVATE fcpp)
