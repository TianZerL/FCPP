#ifndef FCPP_CORE_JOYPAD_HPP
#define FCPP_CORE_JOYPAD_HPP

#include <cstdint>
#include <memory>

#include "FCPP/Core/Interface/InputScanner.hpp"

namespace fcpp::core
{
    class Joypad;
    class StandardJoypad;
}

class fcpp::core::Joypad
{
public:
    static std::unique_ptr<Joypad> create(JoypadType type);
public:
    Joypad() = default;
    virtual ~Joypad() = default;

    virtual void write(std::uint8_t data) noexcept = 0;
    virtual std::uint8_t read() noexcept = 0;

    void set(InputScanner* inputScanner) noexcept;
protected:
    InputScanner* inputScanner = nullptr;
};

class fcpp::core::StandardJoypad : public fcpp::core::Joypad
{
private:
    struct StandardJoypadData;
public:
    struct Buttons
    {
        static constexpr int A = 0;
        static constexpr int B = 1;
        static constexpr int SELECT = 2;
        static constexpr int START = 3;
        static constexpr int UP = 4;
        static constexpr int DOWN = 5;
        static constexpr int LEFT = 6;
        static constexpr int RIGHT = 7;
    };
public:
    StandardJoypad();
    ~StandardJoypad() noexcept override;

    void write(std::uint8_t data) noexcept override;
    std::uint8_t read() noexcept override;
private:
    const std::unique_ptr<StandardJoypadData> dptr;
};

#endif
