#ifndef FCPP_CORE_INTERFACE_INPUT_SCANNER_HPP
#define FCPP_CORE_INTERFACE_INPUT_SCANNER_HPP

#include <cstdint>

namespace fcpp::core
{
    class InputScanner;

    enum class JoypadType : std::uint8_t
    {
        Standard,
    };
}

class fcpp::core::InputScanner
{
public:
    InputScanner() = default;
    virtual ~InputScanner() = default;

    virtual std::uint8_t scan() noexcept = 0;
    virtual JoypadType getJoypadType() noexcept = 0;
};

#endif
