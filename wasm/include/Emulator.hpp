#ifndef FCPP_WASM_EMULATOR_HPP
#define FCPP_WASM_EMULATOR_HPP

#include <memory>

namespace fcpp::wasm
{
    class Emulator;
}

class fcpp::wasm::Emulator
{
private:
    struct EmulatorData;
public:
    Emulator();
    ~Emulator() noexcept;

    bool load(const char* path) noexcept;
    bool run() noexcept;
    const char* info() const noexcept;
private:
    const std::unique_ptr<EmulatorData> dptr;
};

#endif
