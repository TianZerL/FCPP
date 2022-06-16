#ifndef FCPP_CORE_BUS_HPP
#define FCPP_CORE_BUS_HPP

#include <cstdint>
#include <memory>

namespace fcpp::core
{
    class Bus;
}

class fcpp::core::Bus
{
private:
    struct BusData;
public:
    Bus();
    ~Bus() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;
    void reset(std::uint8_t v) noexcept;

    template<typename Accessor>
    std::uint8_t read(std::uint16_t addr) noexcept;
    template<typename Accessor>
    void write(std::uint16_t addr, std::uint8_t data) noexcept;
private:
    const std::unique_ptr<BusData> dptr;
};

#endif
