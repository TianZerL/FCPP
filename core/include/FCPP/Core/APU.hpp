#ifndef FCPP_CORE_APU_HPP
#define FCPP_CORE_APU_HPP

#include <cstdint>
#include <memory>

#include "FCPP/Core/Interface/SampleBuffer.hpp"

namespace fcpp::core
{
    class APU;
}

class fcpp::core::APU
{
private:
    struct APUData;
public:
    APU();
    ~APU() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;
    void reset() noexcept;

    void exec() noexcept;

    template<int reg> std::uint8_t get() noexcept;
    template<int reg> void set(std::uint8_t v) noexcept;
    void set(SampleBuffer* sampleBuffer) noexcept;
private:
    const std::unique_ptr<APUData> dptr;
};

#endif
