#include <iostream>
#include <utility>

#include "Emulator.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#ifdef BUILD_EMBIND
#include <emscripten/bind.h>

static bool running = false;
#endif
#endif

static void start(std::string path)
{
    static fcpp::wasm::Emulator emu{};

    std::cout << "Load ROM: " << path << std::endl;
    if (!emu.load(std::move(path)))
    {
        std::cout << "Unsupported ROM file" << std::endl;
        return;
    }
    std::cout << "Start!" << std::endl;

#ifdef __EMSCRIPTEN__
#ifdef BUILD_EMBIND
    running = true;
#endif
    emscripten_set_main_loop_arg([](void* data) {
        if (!static_cast<fcpp::wasm::Emulator*>(data)->run()) emscripten_cancel_main_loop();
        }, &emu, 0, 1);
#else
    while (emu.run());
#endif
}

#if defined(__EMSCRIPTEN__) && defined(BUILD_EMBIND)
static void stop()
{
    if (!running) return;
    emscripten_cancel_main_loop();
    running = false;
    std::cout << "Stopped!" << std::endl;
}

EMSCRIPTEN_BINDINGS(fcpp) {
    emscripten::function("start", &start);
    emscripten::function("stop", &stop);
    emscripten::function("info", &fcpp::wasm::Emulator::info);
}
#else
int main(int argc, char* argv[])
{
    std::cout << fcpp::wasm::Emulator::info() << std::endl;
    start(argc > 1 ? argv[argc - 1] : TEST_ROM_LOAD_PATH);
    return 0;
}
#endif
