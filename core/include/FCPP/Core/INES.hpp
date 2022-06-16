#ifndef FCPP_CORE_INES_HPP
#define FCPP_CORE_INES_HPP

#include <cstddef>
#include <cstdint>
#include <memory>

#include <FCPPExport.hpp>

namespace fcpp::core
{
    class FCPP_EXPORT INES;

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
    INES();
    INES(const INES&);
    INES(INES&&) noexcept;
    ~INES() noexcept;
    INES& operator=(const INES&);
    INES& operator=(INES&&) noexcept;

    bool load(const char* path);
    bool load(const std::uint8_t* buffer, std::size_t size) noexcept;

    std::uint8_t getPRGBanks() const noexcept;
    std::uint8_t getCHRBanks() const noexcept;
    std::uint8_t getMapperType() const noexcept;
    MirrorType getMirrorType() const noexcept;
    std::uint8_t* getPRGData() const noexcept;
    std::size_t getPRGSize() const noexcept;
    std::uint8_t* getCHRData() const noexcept;
    std::size_t getCHRSize() const noexcept;
    std::uint8_t readPRG(std::uint32_t addr) noexcept;
    void writePRG(std::uint32_t addr, std::uint8_t data) noexcept;
    std::uint8_t readCHR(std::uint32_t addr) noexcept;
    void writeCHR(std::uint32_t addr, std::uint8_t data) noexcept;
private:
    std::unique_ptr<INESData> dptr;
};

#endif
