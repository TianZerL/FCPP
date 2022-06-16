project(tfcpp VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(tfcpp
    ${TOP_DIR}/cli/src/Main.cpp
)

target_include_directories(tfcpp PRIVATE
    $<BUILD_INTERFACE:${TOP_DIR}/cli/include>
)

target_link_libraries(tfcpp PRIVATE
    fcpp
    fcpp_io
    fcpp_util
)

target_compile_definitions(tfcpp PRIVATE
    TFCPP_BUILD_DATE="${TODAY}"
    TFCPP_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

install(
    TARGETS tfcpp
    ARCHIVE DESTINATION tfcpp/lib
    LIBRARY DESTINATION tfcpp/lib
    RUNTIME DESTINATION bin
)
