project(fcpp_test_wasm VERSION 1.0.0.0 LANGUAGES CXX)

add_executable(fcpp_test_wasm
    ${TOP_DIR}/test/wasm/src/Main.cpp
)

target_link_libraries(fcpp_test_wasm PRIVATE efcpp)

target_compile_definitions(fcpp_test_wasm PRIVATE
    TEST_ROM_LOAD_PATH=$<IF:$<BOOL:${EMSCRIPTEN}>,"rom.nes","${TEST_ROM_PATH}">
)

if(EMSCRIPTEN)
    if(FCPP_BUILD_TEST_WASM_EMBIND)
        target_link_options(fcpp_test_wasm PRIVATE -lembind)
        target_compile_definitions(fcpp_test_wasm PRIVATE BUILD_EMBIND)
    else()
        target_link_options(fcpp_test_wasm PRIVATE "SHELL:--preload-file ${TEST_ROM_PATH}@rom.nes")
        set_target_properties(fcpp_test_wasm PROPERTIES SUFFIX ".html")
    endif()
endif()

install(
    TARGETS fcpp_test_wasm
    RUNTIME DESTINATION test/wasm
)
