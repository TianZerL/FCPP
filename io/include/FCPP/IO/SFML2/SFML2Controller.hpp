#ifndef FCPP_IO_SFML2_CONTROLLER_HPP
#define FCPP_IO_SFML2_CONTROLLER_HPP

#include <cstdint>
#include <memory>

#include <FCPPIOExport.hpp>

#include "FCPP/IO/Controller.hpp"

namespace fcpp::io
{
    class FCPP_IO_EXPORT SFML2Controller;
}

class fcpp::io::SFML2Controller : public fcpp::io::Controller
{
private:
    struct SFML2ControllerData;
public:
    class Info : public Controller::Info
    {
    public:
        Info() = default;
        ~Info() override = default;
        int getRenderDriverCount() noexcept override;
        const char* getRenderDriverName(int idx) noexcept override;
    };
    class JoystickHelper : public Controller::JoystickHelper
    {
    public:
        JoystickHelper() = default;
        ~JoystickHelper() override = default;
        bool joystickPressed(int port, fcpp::io::Joystick& buttons) noexcept override;
        int joystickCount() noexcept override;
    };
public:
    SFML2Controller();
    ~SFML2Controller() noexcept override;

    bool create() noexcept override;
    fcpp::core::FrameBuffer* getFrameBuffer() noexcept override;
    fcpp::core::InputScanner* getInputScanner(int idx) noexcept override;
    fcpp::core::SampleBuffer* getSampleBuffer() noexcept override;
    void render() noexcept override;
    bool setFullScreen(bool enable) noexcept override;
    bool setBorderless(bool enable) noexcept override;
    bool setVerticalSync(bool enable) noexcept override;
    bool setRenderDriver(int idx) noexcept override;
    bool setJoystickPort(int idx, int port) noexcept override;
    void setScale(float factor) noexcept override;
    void setTitle(const char* text) noexcept override;
    void setFPSLimit(double fps) noexcept override;
    void setVolume(float volume) noexcept override;
    void setSampleRate(int rate) noexcept override;
    void setJoypadType(int idx, fcpp::core::JoypadType type) noexcept override;
    void setPaletteTable(const PaletteTable& paletteTable) noexcept override;
    void setPaletteTable(PaletteTable&& paletteTable) noexcept override;
    void setTurboButtonSpeed(int idx, std::uint8_t v) noexcept override;
    void setFrameBufferData(const std::uint8_t* data) noexcept override;
    void getFrameBufferData(std::uint8_t* data) const noexcept override;
    void bind(int idx, int standardButtonIdx, Keyboard key) noexcept override;
    void bind(int idx, int standardButtonIdx, Joystick button) noexcept override;
    void setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept override;
    void setFrameCompleteCallback(std::function<void()> callback) noexcept override;
    void setRenderCallback(std::function<void()> callback) noexcept override;
    void setCloseCallback(std::function<void()> callback) noexcept override;
private:
    const std::unique_ptr<SFML2ControllerData> dptr;
};

#endif
