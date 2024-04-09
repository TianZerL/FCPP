#include <cstdio>

#include <SDL_scancode.h>

#include "FCPP/Core.hpp"
#include "FCPP/Tools.hpp"

#include "IO.hpp"

static void dumpMemory(const fcpp::tools::Debugger::MemoryView& memoryView, FILE* stream = stdout)
{
    int cols = 16;
    int rows = memoryView.size() / cols;
    const std::uint8_t* ptr = memoryView.data();
    std::fprintf(stream, "%dB Memory: \n      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n", memoryView.size());
    for (int i = 0; i < rows; i++)
    {
        std::fprintf(stream, "%04x  ", i * 16);
        for (int j = 0; j < cols; j++) std::fprintf(stream, "%02X ", ptr[i * cols + j]);
        std::putchar('\n');
    }
}

int main(int argc, char* argv[])
{
    fcpp::core::FC fc{};
    fcpp::tools::Debugger debugger{};

    if (!fc.insertCartridge(argc > 1 ? argv[argc - 1] : TEST_ROM_LOAD_PATH))
    {
        std::fprintf(stderr, "Failed to load rom.");
        return 0;
    }

    char textBuffer[128 + 1] = {};
    bool stop = false, pause = false;

    debugger.connect(&fc);
    auto cpuView = debugger.getCPUView();
    auto patternTableView = debugger.getPatternTableView();
    auto ramView = debugger.getRamView();
    auto vramView = debugger.getVRamView();
    auto pramView = debugger.getPRamView();

    fc.connect(0, gIO.getInputScanner());
    fc.connect(gIO.getSampleBuffer());
    fc.connect(gIO.getFrameBuffer(patternTableView.data(), 
    [&](int key) {
        switch (key)
        {
        case SDL_SCANCODE_ESCAPE:
            stop = true;
            break;
        case SDL_SCANCODE_F1:
            dumpMemory(ramView);
            break;
        case SDL_SCANCODE_F2:
            dumpMemory(vramView);
            break;
        case SDL_SCANCODE_F3:
            dumpMemory(pramView);
            break;
        case SDL_SCANCODE_F4:
            pause = !pause;
            break;
        default:
            break;
        }
    },
    [&](const char** text) {
        patternTableView.update();
        auto& registers = cpuView.registers();
        std::snprintf(textBuffer, sizeof(textBuffer),
            "a: $%02X  x: $%02X  y: $%02X  sp: $%02X  p: $%02X\npc: $%04X  %s",
            registers.a, registers.x, registers.y, registers.sp, registers.p,
            registers.pc, cpuView.instruction());
        *text = textBuffer;
        },
    [&]() {
        stop = true;
    }));
    fc.powerOn();

    while (!stop)
    {
        cpuView.update();
        if (!pause) fc.exec();
        else gIO.update();
    }

    return 0;
}
