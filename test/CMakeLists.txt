if(NOT TEST_ROM_PATH)
    set(TEST_ROM_PATH "D:/tmp/nova.nes")
endif()
if(FCPP_BUILD_TEST_CORE)
    add_subdirectory(core)
endif()
if(FCPP_BUILD_TEST_WASM)
    add_subdirectory(wasm)
endif()
if(FCPP_BUILD_TEST_DEBUGGER)
    add_subdirectory(debugger)
endif()
