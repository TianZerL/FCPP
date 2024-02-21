#ifndef FCPP_WASM_EMULATOR_HPP
#define FCPP_WASM_EMULATOR_HPP

#include <memory>
#include <string>

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

    bool load(std::string path) noexcept;
    bool run() noexcept;

    static std::string info() noexcept;
private:
    const std::unique_ptr<EmulatorData> dptr;
};

#endif
