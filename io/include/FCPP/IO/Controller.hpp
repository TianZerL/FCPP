#ifndef FCPP_IO_CONTROLLER_HPP
#define FCPP_IO_CONTROLLER_HPP

#include <cstdint>
#include <functional>

#include "FCPP/Core/Interface/FrameBuffer.hpp"
#include "FCPP/Core/Interface/InputScanner.hpp"
#include "FCPP/Core/Interface/SampleBuffer.hpp"

#include "FCPP/IO/InputDevice.hpp"
#include "FCPP/IO/PaletteTable.hpp"

namespace fcpp::io
{
    class Controller;
}

class fcpp::io::Controller
{
public:
    class Info
    {
    public:
        Info() = default;
        virtual ~Info() = default;
        virtual int getRenderDriverCount() noexcept = 0;
        virtual const char* getRenderDriverName(int idx) noexcept = 0;
    };
    class JoystickHelper
    {
    public:
        JoystickHelper() = default;
        virtual ~JoystickHelper() = default;
        virtual bool joystickPressed(int port, fcpp::io::Joystick& buttons) noexcept = 0;
        virtual int joystickCount() noexcept = 0;
    };
public:
    Controller() = default;
    virtual ~Controller() = default;

    virtual bool create() noexcept = 0;
    virtual fcpp::core::FrameBuffer* getFrameBuffer() noexcept = 0;
    virtual fcpp::core::InputScanner* getInputScanner(int idx) noexcept = 0;
    virtual fcpp::core::SampleBuffer* getSampleBuffer() noexcept = 0;
    virtual void render() noexcept = 0;
    virtual bool setFullScreen(bool enable) noexcept = 0;
    virtual bool setBorderless(bool enable) noexcept = 0;
    virtual bool setVerticalSync(bool enable) noexcept = 0;
    virtual bool setRenderDriver(int idx) noexcept = 0;
    virtual bool setJoystickPort(int idx, int port) noexcept = 0;
    virtual void setScale(float factor) noexcept = 0;
    virtual void setTitle(const char* text) noexcept = 0;
    virtual void setFPSLimit(double fps) noexcept = 0;
    virtual void setVolume(float volume) noexcept = 0;
    virtual void setSampleRate(int rate) noexcept = 0;
    virtual void setJoypadType(int idx, fcpp::core::JoypadType type) noexcept = 0;
    virtual void setPaletteTable(const PaletteTable& paletteTable) noexcept = 0;
    virtual void setPaletteTable(PaletteTable&& paletteTable) noexcept = 0;
    virtual void setTurboButtonSpeed(int idx, std::uint8_t v) noexcept = 0;
    virtual void setFrameBufferData(const std::uint8_t* data) noexcept = 0;
    virtual void getFrameBufferData(std::uint8_t* data) const noexcept = 0;
    virtual void bind(int idx, int standardButtonIdx, Keyboard key) noexcept = 0;
    virtual void bind(int idx, int standardButtonIdx, Joystick button) noexcept = 0;
    virtual void setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept = 0;
    virtual void setFrameCompleteCallback(std::function<void()> callback) noexcept = 0;
    virtual void setRenderCallback(std::function<void()> callback) noexcept = 0;
    virtual void setCloseCallback(std::function<void()> callback) noexcept = 0;

    template<int n>
    void bind(int idx, Keyboard(&keys)[n]) noexcept;
    template<int n>
    void bind(int idx, Joystick(&buttons)[n]) noexcept;
public:
    static constexpr int FrameBufferSize = 256 * 240 * 4;
};

template<int n>
inline void fcpp::io::Controller::bind(const int idx, Keyboard(&keys)[n]) noexcept
{
    for (int i = 0; i < 10 && i < n; i++) bind(idx, i, keys[i]);
}
template<int n>
inline void fcpp::io::Controller::bind(const int idx, Joystick(&buttons)[n]) noexcept
{
    for (int i = 0; i < 10 && i < n; i++) bind(idx, i, buttons[i]);
}

#endif
