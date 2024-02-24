#ifndef FCPP_CORE_INES_HPP
#define FCPP_CORE_INES_HPP

#include <cstddef>
#include <cstdint>
#include <memory>

#include <FCPPExport.hpp>

namespace fcpp::core
{
    class INES;

    enum class MirrorType : std::uint8_t
    {
        SINGLE_SCREEN_LOWER_BANK,
        SINGLE_SCREEN_UPPER_BANK,
        VERTICAL,
        HORIZONTAL,
        FOUR_SCREEN
    };
}

class fcpp::core::INES
{
private:
    struct INESData;
public:
    FCPP_EXPORT INES();
    FCPP_EXPORT INES(const INES&);
    FCPP_EXPORT INES(INES&&) noexcept;
    FCPP_EXPORT ~INES() noexcept;
    FCPP_EXPORT INES& operator=(const INES&);
    FCPP_EXPORT INES& operator=(INES&&) noexcept;

    FCPP_EXPORT bool load(const char* path);
    FCPP_EXPORT bool load(const std::uint8_t* buffer, std::size_t size) noexcept;

    FCPP_EXPORT std::uint8_t getPRGBanks() const noexcept;
    FCPP_EXPORT std::uint8_t getCHRBanks() const noexcept;
    FCPP_EXPORT std::uint8_t getMapperType() const noexcept;
    FCPP_EXPORT MirrorType getMirrorType() const noexcept;
    FCPP_EXPORT std::uint8_t* getPRGData() const noexcept;
    FCPP_EXPORT std::size_t getPRGSize() const noexcept;
    FCPP_EXPORT std::uint8_t* getCHRData() const noexcept;
    FCPP_EXPORT std::size_t getCHRSize() const noexcept;

    FCPP_EXPORT std::uint8_t readPRG(std::uint32_t addr) noexcept;
    FCPP_EXPORT void writePRG(std::uint32_t addr, std::uint8_t data) noexcept;
    FCPP_EXPORT std::uint8_t readCHR(std::uint32_t addr) noexcept;
    FCPP_EXPORT void writeCHR(std::uint32_t addr, std::uint8_t data) noexcept;
private:
    std::unique_ptr<INESData> dptr;
};

#endif
