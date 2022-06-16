#ifndef FCPP_CORE_CARTRIDGE_HPP
#define FCPP_CORE_CARTRIDGE_HPP

#include <memory>

#include <FCPPExport.hpp>

#include "FCPP/Core/INES.hpp"

namespace fcpp::core
{
    class Cartridge;
}

class fcpp::core::Cartridge
{
private:
    struct CartridgeData;
public:
    FCPP_EXPORT static bool support(const INES& rom);
public:
    Cartridge();
    ~Cartridge() noexcept;

    void connect(void* p) noexcept;
    void save(void* p) noexcept;
    void load(void* p) noexcept;

    bool load(const char* path);
    bool load(const INES& content);
    bool load(INES&& content);

    FCPP_EXPORT INES& getContent() noexcept;
    MirrorType getMirrorType() noexcept;

    std::uint8_t readPRG(std::uint16_t addr) noexcept;
    void writePRG(std::uint16_t addr, std::uint8_t data) noexcept;

    std::uint8_t readCHR(std::uint16_t addr) noexcept;
    void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept;

    void sync() noexcept;
private:
    const std::unique_ptr<CartridgeData> dptr;
};

#endif
