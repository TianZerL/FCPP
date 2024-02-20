project(fcpp_test_wasm VERSION 1.0.0.0 LANGUAGES CXX)

if(NOT ROM_PATH)
    set(ROM_PATH "D:/tmp/nova.nes")
endif()

add_executable(fcpp_test_wasm
    ${TOP_DIR}/test/wasm/src/Main.cpp
)

target_link_libraries(fcpp_test_wasm PRIVATE efcpp)

target_compile_definitions(fcpp_test_wasm PRIVATE
    ROM_LOAD_PATH=$<IF:$<BOOL:${EMSCRIPTEN}>,"rom.nes","${ROM_PATH}">
)

if(EMSCRIPTEN)
    target_link_options(fcpp_test_wasm PRIVATE "SHELL:--preload-file ${ROM_PATH}@rom.nes")
    set_target_properties(fcpp_test_wasm PROPERTIES SUFFIX ".html")
endif()

install(
    TARGETS fcpp_test_wasm
    RUNTIME DESTINATION test/wasm
)
