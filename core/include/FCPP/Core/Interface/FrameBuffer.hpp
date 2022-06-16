#ifndef FCPP_CORE_INTERFACE_FRAME_BUFFER_HPP
#define FCPP_CORE_INTERFACE_FRAME_BUFFER_HPP

#include <cstdint>

namespace fcpp::core
{
    class FrameBuffer;
}

class fcpp::core::FrameBuffer
{
public:
    FrameBuffer() = default;
    virtual ~FrameBuffer() = default;

    virtual void setPixel(int x, int y, std::uint32_t color) noexcept = 0;
    virtual void completedSignal() noexcept = 0;
    virtual const std::uint32_t* getPaletteTable() noexcept = 0;
};

#endif
