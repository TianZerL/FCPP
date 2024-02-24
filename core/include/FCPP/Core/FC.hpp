#ifndef FCPP_CORE_FC_HPP
#define FCPP_CORE_FC_HPP

#include <memory>

#include <FCPPExport.hpp>

#include "FCPP/Core/Bus.hpp"
#include "FCPP/Core/Cartridge.hpp"
#include "FCPP/Core/CPU.hpp"
#include "FCPP/Core/Clock.hpp"
#include "FCPP/Core/PPU.hpp"
#include "FCPP/Core/APU.hpp"
#include "FCPP/Core/Joypad.hpp"
#include "FCPP/Core/Snapshot.hpp"

namespace fcpp::core
{
    class FC;
}

class fcpp::core::FC
{
private:
    struct FCData;
public:
    FCPP_EXPORT FC();
    FCPP_EXPORT FC(FC&&) noexcept;
    FCPP_EXPORT ~FC() noexcept;

    FCPP_EXPORT bool insertCartridge(const char* path);
    FCPP_EXPORT bool insertCartridge(const INES& content);
    FCPP_EXPORT bool insertCartridge(INES&& content);

    // connect joypad to pecified index
    FCPP_EXPORT void connect(int idx, InputScanner* inputScanner) noexcept;
    // connect frame buffer to PPU
    FCPP_EXPORT void connect(FrameBuffer* frameBuffer) noexcept;
    // connect sample buffer to APU
    FCPP_EXPORT void connect(SampleBuffer* sampleBuffer) noexcept;

    FCPP_EXPORT void setFrameRate(double fps) noexcept;
    // min 8, max 16
    FCPP_EXPORT void setSpriteLimit(int limit) noexcept;

    FCPP_EXPORT void powerOn() noexcept;
    FCPP_EXPORT void reset() noexcept;

    FCPP_EXPORT void save(Snapshot& snapshot) noexcept;
    FCPP_EXPORT void load(Snapshot& snapshot) noexcept;

    FCPP_EXPORT void exec() noexcept;

    FCPP_EXPORT Clock* getClock() noexcept;
    FCPP_EXPORT CPU* getCPU() noexcept;
    FCPP_EXPORT PPU* getPPU() noexcept;
    FCPP_EXPORT APU* getAPU() noexcept;
    FCPP_EXPORT Bus* getBus() noexcept;
    FCPP_EXPORT Cartridge* getCartridge() noexcept;
    FCPP_EXPORT Joypad* getJoypad(int idx) noexcept;
private:
    std::unique_ptr<FCData> dptr;
};

#endif
