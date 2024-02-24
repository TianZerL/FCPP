#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include "FCPP/Core.hpp"
#include "FCPP/IO.hpp"
#include "FCPP/Tools.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>

class PatternTabWindow
{
public:
    ~PatternTabWindow() noexcept;
    bool create(const std::uint32_t* buffer) noexcept;
    void render() noexcept;
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    const std::uint32_t* frameBuffer = nullptr;
};

static void dumpMemory(const fcpp::tools::Debugger::MemoryView& memoryView)
{
    int cols = 32;
    int rows = memoryView.size() / cols;
    const std::uint8_t* ptr = memoryView.data();
    std::cout.fill('0');
    std::cout << std::dec << memoryView.size() << "B Memory: \n";
    std::cout << std::hex << std::uppercase;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++) std::cout << std::setw(2) << +ptr[i * cols + j] << ' ';
        std::cout << '\n';
    }
}

int main(int argc, char* argv[])
{
    fcpp::core::FC fc{};
    fcpp::tools::Debugger debugger{};

    if (!fc.insertCartridge(argc > 1 ? argv[argc - 1] : TEST_ROM_LOAD_PATH))
    {
        std::cerr << "Failed to load rom" << std::endl;
        return 0;
    }
    auto controller = fcpp::io::manager::create(0);
    controller->setTitle("fcpp");
    controller->setVerticalSync(true);
    controller->setScale(2.0);

    PatternTabWindow debugWindow{};

    debugger.connect(&fc);
    auto cpuView = debugger.getCPUView();
    auto patternTableView = debugger.getPatternTableView();
    controller->setRenderCallback([&]() {
        patternTableView.update();
        auto& registers = cpuView.registers();
        std::ostringstream oss{};
        oss << std::hex << std::uppercase
            << "  a: $" << +registers.a << " x: $" << +registers.x << " y: $" << +registers.y
            << " sp: $" << +registers.sp << " p: $" << +registers.p << " pc: $" << +registers.pc
            << "  " << cpuView.instruction();
        controller->setTitle(oss.str().c_str());
        debugWindow.render();
        });

    bool close = false;
    auto ramView = debugger.getRamView();
    auto vramView = debugger.getVRamView();
    auto pramView = debugger.getPRamView();
    controller->setKeyPressCallback([&](fcpp::io::Keyboard key) {
        switch (key)
        {
        case fcpp::io::Keyboard::Q:
            close = true;
            break;
        case fcpp::io::Keyboard::R:
            dumpMemory(ramView);
            break;
        case fcpp::io::Keyboard::V:
            dumpMemory(vramView);
            break;
        case fcpp::io::Keyboard::P:
            dumpMemory(pramView);
            break;
        default:
            break;
        }
        });
    if (!controller->create())
    {
        std::cerr << "Failed to create controller" << std::endl;
        return 0;
    }
    debugWindow.create(patternTableView.data());

    fc.connect(0, controller->getInputScanner(0));
    fc.connect(controller->getSampleBuffer());
    fc.connect(controller->getFrameBuffer());
    fc.powerOn();

    while (!close)
    {
        cpuView.update();
        fc.exec();
    }

    return 0;
}

PatternTabWindow::~PatternTabWindow() noexcept
{
    if (texture != nullptr) SDL_DestroyTexture(texture);
    if (renderer != nullptr) SDL_DestroyRenderer(renderer);
    if (window != nullptr) SDL_DestroyWindow(window);
}

bool PatternTabWindow::create(const std::uint32_t* const buffer) noexcept
{
    if (window == nullptr)
    {
        window = SDL_CreateWindow("debugger", 64, 64,
            fcpp::tools::Debugger::PatternTableView::pageWidth * 3,
            fcpp::tools::Debugger::PatternTableView::pageHeight * fcpp::tools::Debugger::PatternTableView::pageCount * 3,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr)
        {
            SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return false;
        }
    }
    if (renderer == nullptr)
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr)
        {
            SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
            return false;
        }
    }
    if (texture == nullptr)
    {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
            fcpp::tools::Debugger::PatternTableView::pageWidth,
            fcpp::tools::Debugger::PatternTableView::pageHeight * fcpp::tools::Debugger::PatternTableView::pageCount);
        if (texture == nullptr)
        {
            SDL_Log("SDL_CreateTexture Error: %s\n", SDL_GetError());
            return false;
        }
    }
    frameBuffer = buffer;
    return true;
}
void PatternTabWindow::render() noexcept
{
    if (SDL_UpdateTexture(texture, nullptr, frameBuffer, 128 * sizeof(std::uint32_t)) != 0) SDL_Log("SDL_UpdateTexture Error: %s\n", SDL_GetError());
    if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) != 0) SDL_Log("SDL_RenderCopy Error: %s\n", SDL_GetError());
    SDL_RenderPresent(renderer);
}
