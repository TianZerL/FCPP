project(fcpp_util VERSION 1.0.0.0 LANGUAGES CXX)

add_library(fcpp_util INTERFACE)

target_include_directories(fcpp_util INTERFACE ${TOP_DIR}/util/include)
