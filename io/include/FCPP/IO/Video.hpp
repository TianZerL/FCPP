#ifndef FCPP_IO_VIDEO_HPP
#define FCPP_IO_VIDEO_HPP

#include <functional>
#include <utility>

#include "FCPP/Core/Interface/FrameBuffer.hpp"

#include "FCPP/IO/InputDevice.hpp"
#include "FCPP/IO/PaletteTable.hpp"

#include "FCPP/Util/FPSLimiter.hpp"

namespace fcpp::io::detail
{
    class Video;
}

class fcpp::io::detail::Video : public fcpp::core::FrameBuffer
{
protected:
    Video() = default;
    ~Video() override = default;
public:
    void setPaletteTable(const PaletteTable& data);
    void setPaletteTable(PaletteTable&& data) noexcept;
    void setFPSLimit(double fps) noexcept;

    void setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept;
    void setFrameCompleteCallback(std::function<void()> callback) noexcept;
    void setRenderCallback(std::function<void()> callback) noexcept;
    void setCloseCallback(std::function<void()> callback) noexcept;
protected:
    using FrameBuffer::setPixel;
    using FrameBuffer::completedSignal;
    const std::uint32_t* getPaletteTable() noexcept override;
protected:
    fcpp::io::PaletteTable paletteTable{};
    fcpp::util::FPSLimiter fpsLimiter{ 60.0 };

    std::function<void(Keyboard)> keyPressCallback{};
    std::function<void()> frameCompletedCallback{};
    std::function<void()> renderCallback{};
    std::function<void()> closeCallback{};
};

inline void fcpp::io::detail::Video::setPaletteTable(const PaletteTable& data)
{
    paletteTable = data;
}
inline void fcpp::io::detail::Video::setPaletteTable(PaletteTable&& data) noexcept
{
    paletteTable = std::move(data);
}
inline void fcpp::io::detail::Video::setFPSLimit(const double fps) noexcept
{
    fpsLimiter.set(fps > 0.0 ? fps : 500.0);
}
inline void fcpp::io::detail::Video::setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept
{
    keyPressCallback = std::move(callback);
}
inline void fcpp::io::detail::Video::setFrameCompleteCallback(std::function<void()> callback) noexcept
{
    frameCompletedCallback = std::move(callback);
}
inline void fcpp::io::detail::Video::setRenderCallback(std::function<void()> callback) noexcept
{
    renderCallback = std::move(callback);
}
inline void fcpp::io::detail::Video::setCloseCallback(std::function<void()> callback) noexcept
{
    closeCallback = std::move(callback);
}
inline const std::uint32_t* fcpp::io::detail::Video::getPaletteTable() noexcept
{
    return paletteTable.get();
}

#endif
