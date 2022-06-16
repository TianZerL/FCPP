#include "FCPP/Core/Clock.hpp"
#include "FCPP/Core/FC.hpp"

struct fcpp::core::Clock::ClockData
{
    PPU* ppu = nullptr;
    APU* apu = nullptr;

    std::uint64_t CPUCycles = 0;
    double fps = 60.0;
};

fcpp::core::Clock::Clock() : dptr(std::make_unique<ClockData>()) {}
fcpp::core::Clock::~Clock() noexcept = default;

void fcpp::core::Clock::connect(void* const p) noexcept
{
    auto fptr = static_cast<FC*>(p);
    dptr->ppu = fptr->getPPU();
    dptr->apu = fptr->getAPU();
}
void fcpp::core::Clock::save(void* const p) noexcept
{
    static_cast<Snapshot*>(p)->getWriter().access(dptr->CPUCycles);
}
void fcpp::core::Clock::load(void* const p) noexcept
{
    static_cast<Snapshot*>(p)->getReader().access(dptr->CPUCycles);
}
void fcpp::core::Clock::reset() noexcept
{
    dptr->CPUCycles = 0;
}

void fcpp::core::Clock::tick() noexcept
{
    dptr->ppu->exec();
    dptr->ppu->exec();
    dptr->ppu->exec();
    dptr->apu->exec();
    dptr->CPUCycles++;
}
void fcpp::core::Clock::setFrameRate(const double fps) noexcept
{
    dptr->fps = fps > 1.0 ? fps : 1.0;
}
std::uint64_t fcpp::core::Clock::getAPUCycles() noexcept
{
    return getCPUCycles() / 2;
}
std::uint64_t fcpp::core::Clock::getCPUCycles() noexcept
{
    return dptr->CPUCycles;
}
std::uint64_t fcpp::core::Clock::getPPUCycles() noexcept
{
    return getCPUCycles() * 3;
}
double fcpp::core::Clock::getAPUFrequency() noexcept
{
    return getCPUFrequency() / 2;
}
double fcpp::core::Clock::getCPUFrequency() noexcept
{
    return getPPUFrequency() / 3;
}
double fcpp::core::Clock::getPPUFrequency() noexcept
{
    return 341.0 * 262.0 * dptr->fps;
}
