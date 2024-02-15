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
    class FCPP_EXPORT FC;
}

class fcpp::core::FC
{
private:
    struct FCData;
public:
    FC();
    FC(FC&&) noexcept;
    ~FC() noexcept;

    bool insertCartridge(const char* path);
    bool insertCartridge(const INES& content);
    bool insertCartridge(INES&& content);

    // connect joypad to pecified index
    void connect(int idx, InputScanner* inputScanner) noexcept;
    // connect frame buffer to PPU
    void connect(FrameBuffer* frameBuffer) noexcept;
    // connect sample buffer to APU
    void connect(SampleBuffer* sampleBuffer) noexcept;

    void setFrameRate(double fps) noexcept;
    // min 8, max 16
    void setSpriteLimit(int limit) noexcept;

    void powerOn() noexcept;
    void reset() noexcept;

    void save(Snapshot& snapshot) noexcept;
    void load(Snapshot& snapshot) noexcept;

    void exec() noexcept;

    Clock* getClock() noexcept;    
    CPU* getCPU() noexcept;
    PPU* getPPU() noexcept;
    APU* getAPU() noexcept;
    Bus* getBus() noexcept;
    Cartridge* getCartridge() noexcept;
    Joypad* getJoypad(int idx) noexcept;
private:
    std::unique_ptr<FCData> dptr;
};

#endif
