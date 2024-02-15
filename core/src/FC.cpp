#include <cstdint>
#include <utility>

#include "FCPP/Core/FC.hpp"

struct fcpp::core::FC::FCData
{
    Clock clock{};
    CPU cpu{};
    PPU ppu{};
    APU apu{};
    Bus bus{};
    Cartridge cartridge{};
    std::unique_ptr<Joypad> joypad[2]{};

    void init(FC* const fc) noexcept
    {
        clock.connect(fc);
        cpu.connect(fc);
        ppu.connect(fc);
        apu.connect(fc);
        bus.connect(fc);        
        cartridge.connect(fc);
    }
};

fcpp::core::FC::FC() : dptr(std::make_unique<FCData>())
{
    dptr->init(this);
}
fcpp::core::FC::FC(FC&& other) noexcept : dptr(std::move(other.dptr))
{
    dptr->init(this);
}
fcpp::core::FC::~FC() noexcept = default;

bool fcpp::core::FC::insertCartridge(const char* const path)
{
    return dptr->cartridge.load(path);
}
bool fcpp::core::FC::insertCartridge(const INES& content)
{
    return dptr->cartridge.load(content);
}
bool fcpp::core::FC::insertCartridge(INES&& content)
{
    return dptr->cartridge.load(std::move(content));
}

void fcpp::core::FC::connect(const int idx, InputScanner* const inputScanner) noexcept
{
    if (idx < (sizeof(dptr->joypad) / sizeof(dptr->joypad[0])))
    {
        if (inputScanner != nullptr)
        {
            dptr->joypad[idx] = Joypad::create(inputScanner->getJoypadType());
            dptr->joypad[idx]->set(inputScanner);
        }
        else dptr->joypad[idx].reset();
    }
}
void fcpp::core::FC::connect(FrameBuffer* const frameBuffer) noexcept
{
    dptr->ppu.set(frameBuffer);
}
void fcpp::core::FC::connect(SampleBuffer* const sampleBuffer) noexcept
{
    dptr->apu.set(sampleBuffer);
}

void fcpp::core::FC::setFrameRate(const double fps) noexcept
{
    dptr->clock.setFrameRate(fps);
}
void fcpp::core::FC::setSpriteLimit(const int limit) noexcept
{
    dptr->ppu.set<PPU::State::Type::SpriteLimit>(limit);
}

void fcpp::core::FC::powerOn() noexcept
{
    dptr->bus.reset(0xff);
    dptr->cpu.requestRESET();
}
void fcpp::core::FC::reset() noexcept
{
    dptr->clock.reset();
    dptr->cpu.reset();
    dptr->ppu.reset();
    dptr->apu.reset();    
    powerOn();
}

void fcpp::core::FC::save(Snapshot& snapshot) noexcept
{
    snapshot.rewindWriter();
    dptr->clock.save(&snapshot);
    dptr->cpu.save(&snapshot);
    dptr->ppu.save(&snapshot);
    dptr->apu.save(&snapshot);
    dptr->bus.save(&snapshot);
    dptr->cartridge.save(&snapshot);
}
void fcpp::core::FC::load(Snapshot& snapshot) noexcept
{
    if (!snapshot.size()) return;

    snapshot.rewindReader();
    dptr->clock.load(&snapshot);
    dptr->cpu.load(&snapshot);    
    dptr->ppu.load(&snapshot);
    dptr->apu.load(&snapshot);    
    dptr->bus.load(&snapshot);
    dptr->cartridge.load(&snapshot);
}

void fcpp::core::FC::exec() noexcept
{
    dptr->cpu.exec();
}

fcpp::core::Clock* fcpp::core::FC::getClock() noexcept
{
    return &dptr->clock;
}
fcpp::core::CPU* fcpp::core::FC::getCPU() noexcept
{
    return &dptr->cpu;
}
fcpp::core::PPU* fcpp::core::FC::getPPU() noexcept
{
    return &dptr->ppu;
}
fcpp::core::APU* fcpp::core::FC::getAPU() noexcept
{
    return &dptr->apu;
}
fcpp::core::Bus* fcpp::core::FC::getBus() noexcept
{
    return &dptr->bus;
}
fcpp::core::Cartridge* fcpp::core::FC::getCartridge() noexcept
{
    return &dptr->cartridge;
}
fcpp::core::Joypad* fcpp::core::FC::getJoypad(const int idx) noexcept
{
    return idx < (sizeof(dptr->joypad) / sizeof(dptr->joypad[0])) ? dptr->joypad[idx].get() : nullptr;
}
