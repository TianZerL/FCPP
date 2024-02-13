#ifndef FCPP_CORE_PPU_HPP
#define FCPP_CORE_PPU_HPP

#include <cstdint>
#include <memory>

#include "FCPP/Core/Interface/FrameBuffer.hpp"

namespace fcpp::core
{
    class PPU;
}

class fcpp::core::PPU
{
public:
    enum class Registers
    {
        PPUCTRL, PPUMASK, PPUSTATUS, OAMADDR, OAMDATA, PPUSCROLL, PPUADDR, PPUDATA
    };
    struct State
    {
        enum class Type
        {
            SpriteLimit, AddressBus
        };
    };
private:
    struct PPUData;
public:
    PPU();
    ~PPU() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;
    void reset() noexcept;

    void exec() noexcept;

    template<Registers reg> std::uint8_t get() noexcept;
    template<Registers reg> void set(std::uint8_t v) noexcept;
    template<State::Type type> unsigned int get() const noexcept;
    template<State::Type type> void set(unsigned int v) noexcept;
    void set(FrameBuffer* frameBuffer) noexcept;
private:
    const std::unique_ptr<PPUData> dptr;
};

#endif
