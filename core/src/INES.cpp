#include <fstream>
#include <iterator>
#include <vector>

#include "FCPP/Core/INES.hpp"

struct fcpp::core::INES::INESData
{
    std::uint8_t prgBanks = 0;
    std::uint8_t chrBanks = 0;
    std::uint8_t mapperType = 0;
    MirrorType mirrorType = MirrorType::VERTICAL;

    std::vector<std::uint8_t> prgRom{};
    std::vector<std::uint8_t> chrRom{};
};

fcpp::core::INES::INES() : dptr(std::make_unique<INESData>()) {}
fcpp::core::INES::INES(const INES& other) : dptr(std::make_unique<INESData>(*other.dptr)) {}
fcpp::core::INES::INES(INES&&) noexcept = default;
fcpp::core::INES::~INES() noexcept = default;
fcpp::core::INES& fcpp::core::INES::operator=(const INES & other)
{
    *dptr = *other.dptr;
    return *this;
}
fcpp::core::INES& fcpp::core::INES::operator=(INES&&) noexcept = default;

bool fcpp::core::INES::load(const char* const path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    std::vector<std::uint8_t> buffer((std::istreambuf_iterator<char>{file}), std::istreambuf_iterator<char>{});
    return load(buffer.data(), buffer.size());
}
bool fcpp::core::INES::load(const std::uint8_t* buffer, const std::size_t size) noexcept
{   /* reference https://wiki.nesdev.org/w/index.php?title=INES
    *The format of the header is as follows:
    * 0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
    * 4: Size of PRG ROM in 16 KB units
    * 5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
    * 6: Flags 6 - Mapper, mirroring, battery, trainer
    * 7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
    * 8: Flags 8 - PRG-RAM size (rarely used extension)
    * 9: Flags 9 - TV system (rarely used extension)
    * 10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
    * 11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
    */

    *dptr = {}; //clear data

    std::size_t length = 16;

    if (size < length) return false; // header = 16byte
    if (buffer[0] != 0x4e || buffer[1] != 0x45 || buffer[2] != 0x53 || buffer[3] != 0x1a) return false; //check magic number

    dptr->prgBanks = buffer[4];
    dptr->chrBanks = buffer[5];
    dptr->mapperType = (buffer[6] >> 4) | (buffer[7] & 0xf0);
    dptr->mirrorType = (buffer[6] & (1 << 3)) ? MirrorType::FOUR_SCREEN : (buffer[6] & 1) ? MirrorType::VERTICAL : MirrorType::HORIZONTAL;

    if (buffer[6] & (1 << 2)) // trainer
    {
        if (size < (length += 512)) return false; // header + trainer = 528byte
        else buffer += 512; //skip trainer
    }
    buffer += 16;

    std::size_t prgRomSize = dptr->prgBanks * static_cast<std::size_t>(0x4000); //16kb * banks
    std::size_t chrRomSize = dptr->chrBanks * static_cast<std::size_t>(0x2000); //8kb * banks

    if (size < (length + prgRomSize + chrRomSize)) return false; // header + trainer + prgRomSize + chrRomSize

    dptr->prgRom.assign(buffer, buffer + prgRomSize);
    buffer += prgRomSize;

    if (chrRomSize) dptr->chrRom.assign(buffer, buffer + chrRomSize);
    else dptr->chrRom.resize(0x2000); // 8kb video ram if no video rom

    return true;
}

std::uint8_t fcpp::core::INES::getPRGBanks() const noexcept
{
    return dptr->prgBanks;
}
std::uint8_t fcpp::core::INES::getCHRBanks() const noexcept
{
    return dptr->chrBanks;
}
std::uint8_t fcpp::core::INES::getMapperType() const noexcept
{
    return dptr->mapperType;
}
fcpp::core::MirrorType fcpp::core::INES::getMirrorType() const noexcept
{
    return dptr->mirrorType;
}
std::uint8_t* fcpp::core::INES::getPRGData() const noexcept
{
    return dptr->prgRom.data();
}
std::size_t fcpp::core::INES::getPRGSize() const noexcept
{
    return dptr->prgRom.size();
}
std::uint8_t* fcpp::core::INES::getCHRData() const noexcept
{
    return dptr->chrRom.data();
}
std::size_t fcpp::core::INES::getCHRSize() const noexcept
{
    return dptr->chrRom.size();
}
std::uint8_t fcpp::core::INES::readPRG(const std::uint32_t addr) noexcept
{
    return dptr->prgRom[addr];
}
void fcpp::core::INES::writePRG(const std::uint32_t addr, const std::uint8_t data) noexcept
{
    dptr->prgRom[addr] = data;
}
std::uint8_t fcpp::core::INES::readCHR(const std::uint32_t addr) noexcept
{
    return dptr->chrRom[addr];
}
void fcpp::core::INES::writeCHR(const std::uint32_t addr, const std::uint8_t data) noexcept
{
    dptr->chrRom[addr] = data;
}
