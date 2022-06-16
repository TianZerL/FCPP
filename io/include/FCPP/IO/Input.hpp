#ifndef FCPP_IO_INPUT_HPP
#define FCPP_IO_INPUT_HPP

#include "FCPP/Core/Interface/InputScanner.hpp"

namespace fcpp::io::detail
{
    class Input;
}

class fcpp::io::detail::Input : public fcpp::core::InputScanner
{
protected:
    Input() = default;
    ~Input() override = default;
public:
    void setTurboSpeed(std::uint8_t v) noexcept;
    void setJoypadType(fcpp::core::JoypadType type) noexcept;
protected:
    using InputScanner::scan;
    fcpp::core::JoypadType getJoypadType() noexcept override;
protected:
    std::uint8_t turbo = 0;
    std::uint8_t turboShift = 2;
    fcpp::core::JoypadType joypadType = fcpp::core::JoypadType::Standard;
};

inline void fcpp::io::detail::Input::setTurboSpeed(const std::uint8_t v) noexcept
{
    constexpr std::uint8_t minSpeed = 1, maxSpeed = 8;
    std::uint8_t speed = v < minSpeed ? minSpeed : (maxSpeed < v ? maxSpeed : v);
    turboShift = 8 - speed;
}
inline void fcpp::io::detail::Input::setJoypadType(const fcpp::core::JoypadType type) noexcept
{
    joypadType = type;
}
inline fcpp::core::JoypadType fcpp::io::detail::Input::getJoypadType() noexcept
{
    return joypadType;
}

#endif
