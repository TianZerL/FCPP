#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "FCPP/Util/FPSLimiter.hpp"

#include "Emulator.hpp"

int main(int argc, char* argv[])
{
    fcpp::wasm::Emulator emu{};

    std::cout << "Load: " << ROM_LOAD_NAME << std::endl;
    if (!emu.load(ROM_LOAD_NAME))
    {
        std::cerr << "Failed to load ROM file" << std::endl;
        return 0;
    }

    std::cout << "Start!" << std::endl;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg([](void* data) {
            if (!static_cast<fcpp::wasm::Emulator*>(data)->run()) emscripten_cancel_main_loop();
        }, &emu, 0, 1);
#else
    fcpp::util::FPSLimiter fpsLimiter{ 60 };
    while (emu.run()) fpsLimiter.wait();
#endif
    return 0;
}
