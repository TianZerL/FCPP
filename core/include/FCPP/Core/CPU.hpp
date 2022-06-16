#ifndef FCPP_CORE_CPU_HPP
#define FCPP_CORE_CPU_HPP

#include <cstdint>
#include <memory>

namespace fcpp::core
{
    class CPU;
}

class fcpp::core::CPU
{
public:
    enum class IRQType
    {
        APU, DMC, Mapper
    };
    struct State
    {
        enum class Type
        {
            TickState, DMAState
        };
        static constexpr unsigned int TICK_STATE_READ = 1;
        static constexpr unsigned int TICK_STATE_WRITE = 0;
        static constexpr unsigned int DMA_STATE_ENABLE = 2;
        static constexpr unsigned int DMA_STATE_SECOND_LAST_TICK = 1;
        static constexpr unsigned int DMA_STATE_LAST_TICK = 3;
        static constexpr unsigned int DMA_STATE_DISABLE = 0;
    };
private:
    struct CPUData;
public:
    CPU();
    ~CPU() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;
    void reset() noexcept;

    void requestDMA(std::uint16_t dst, std::uint8_t page) noexcept;
    void requestRESET() noexcept;
    void requestNMI(bool status) noexcept;
    template<IRQType type> void requestIRQ(bool status) noexcept;

    void exec() noexcept;

    template<State::Type type> unsigned int get() const noexcept;
private:
    const std::unique_ptr<CPUData> dptr;
};

#endif
