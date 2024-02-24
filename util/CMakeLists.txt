project(fcpp_util VERSION 1.0.0.0 LANGUAGES CXX)

add_library(fcpp_util INTERFACE)

target_include_directories(fcpp_util INTERFACE
    $<BUILD_INTERFACE:${TOP_DIR}/util/include>
    $<INSTALL_INTERFACE:fcpp/include>
)

set_target_properties(fcpp_util PROPERTIES EXPORT_NAME "Util")

install(TARGETS fcpp_util EXPORT FCPP)

install(DIRECTORY ${TOP_DIR}/util/include DESTINATION fcpp)
