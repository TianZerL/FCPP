#ifndef FCPP_CORE_CLOCK_HPP
#define FCPP_CORE_CLOCK_HPP

#include <cstdint>
#include <memory>

namespace fcpp::core
{
    class Clock;
}

class fcpp::core::Clock
{
private:
    struct ClockData;
public:
    Clock();
    ~Clock() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;
    void reset() noexcept;

    void tick() noexcept;
    void setFrameRate(double fps) noexcept;
    std::uint64_t getAPUCycles() noexcept;
    std::uint64_t getCPUCycles() noexcept;
    std::uint64_t getPPUCycles() noexcept;
    double getAPUFrequency() noexcept;
    double getCPUFrequency() noexcept;
    double getPPUFrequency() noexcept;
private:
    const std::unique_ptr<ClockData> dptr;
};

#endif
