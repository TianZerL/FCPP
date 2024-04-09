#ifndef FCPP_TEST_DEBUGGER_IO_HPP
#define FCPP_TEST_DEBUGGER_IO_HPP

#include <functional>

#include "FCPP/Core.hpp"

#define gIO (Controller::instance())

class Controller
{
public:
    static Controller& instance() noexcept;
private:
    Controller() noexcept;
    ~Controller() noexcept;
public:
    fcpp::core::FrameBuffer* getFrameBuffer(const std::uint32_t* patternBuffer,
        std::function<void(int)> keyPressCallback,
        std::function<void(const char** text)> updateCallback,
        std::function<void()> closeCallback) noexcept;
    fcpp::core::InputScanner* getInputScanner() noexcept;
    fcpp::core::SampleBuffer* getSampleBuffer() noexcept;

    void update() noexcept;
};

#endif
