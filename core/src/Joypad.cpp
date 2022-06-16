#include "FCPP/Core/Joypad.hpp"

std::unique_ptr<fcpp::core::Joypad> fcpp::core::Joypad::create(const JoypadType type)
{
    switch (type)
    {
    case JoypadType::Standard:
        return std::make_unique<StandardJoypad>();
    default:
        return nullptr;
    }
}

void fcpp::core::Joypad::set(InputScanner* const inputScanner) noexcept
{
    this->inputScanner = inputScanner;
}

struct fcpp::core::StandardJoypad::StandardJoypadData
{
    std::uint8_t bits = 0;
    bool strobe = false;
};

fcpp::core::StandardJoypad::StandardJoypad() : dptr(std::make_unique<StandardJoypadData>()) {}
fcpp::core::StandardJoypad::~StandardJoypad() noexcept = default;

void fcpp::core::StandardJoypad::write(const std::uint8_t data) noexcept
{
    if (dptr->strobe && !(data & 1)) dptr->bits = inputScanner->scan();
    dptr->strobe = data & 1;
}
std::uint8_t fcpp::core::StandardJoypad::read() noexcept
{
    if (dptr->strobe) return 0x40 | (inputScanner->scan() & 1);
    std::uint8_t lsb = 0x40 | (dptr->bits & 1);
    dptr->bits = 0x80 | (dptr->bits >> 1);
    return lsb;
}
