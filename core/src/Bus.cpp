#include <cstring>

#include "FCPP/Core/Bus.hpp"
#include "FCPP/Core/FC.hpp"

namespace fcpp::core::detail
{
    inline std::uint16_t nameTableAddress(const std::uint16_t addr, const MirrorType mirrorType) noexcept
    {
        switch (mirrorType)
        {
        case MirrorType::SINGLE_SCREEN_LOWER_BANK:
            return addr & 0x03ff;
        case MirrorType::SINGLE_SCREEN_UPPER_BANK:
            return 0x0400 + (addr & 0x03ff);
        case MirrorType::VERTICAL:
            return addr & 0x07ff;
        case MirrorType::HORIZONTAL:
            return ((addr >> 1) & 0x0400) + (addr & 0x03ff);
        case MirrorType::FOUR_SCREEN:
        default:
            return addr & 0x0fff;
        }
    }
}

struct fcpp::core::Bus::BusData
{
    FC* fc = nullptr;
    std::uint8_t cpuOpenBusData = 0;
    std::uint8_t ram[0x0800]{};
    std::uint8_t vram[0x1000]{};
    std::uint8_t pram[0x20]{};
};

fcpp::core::Bus::Bus() : dptr(std::make_unique<BusData>()) {}
fcpp::core::Bus::~Bus() noexcept = default;

void fcpp::core::Bus::connect(void* const p) noexcept
{
    dptr->fc = static_cast<FC*>(p);
}
void fcpp::core::Bus::save(void* const p) noexcept
{
    auto& writer = static_cast<Snapshot*>(p)->getWriter();
    writer.access(dptr->cpuOpenBusData);
    writer.access(dptr->ram, sizeof(dptr->ram));
    writer.access(dptr->vram, sizeof(dptr->vram));
    writer.access(dptr->pram, sizeof(dptr->pram));
}
void fcpp::core::Bus::load(void* const p) noexcept
{
    auto& reader = static_cast<Snapshot*>(p)->getReader();
    reader.access(dptr->cpuOpenBusData);
    reader.access(dptr->ram, sizeof(dptr->ram));
    reader.access(dptr->vram, sizeof(dptr->vram));
    reader.access(dptr->pram, sizeof(dptr->pram));
}
void fcpp::core::Bus::reset(const std::uint8_t v) noexcept
{
    std::memset(dptr->ram, v, sizeof(dptr->ram));
    std::memset(dptr->vram, v, sizeof(dptr->vram));
    std::memset(dptr->pram, v, sizeof(dptr->pram));
}

template<>
std::uint8_t fcpp::core::Bus::read<fcpp::core::CPU>(std::uint16_t addr) noexcept
{
    if (addr < 0x2000) return dptr->cpuOpenBusData = dptr->ram[addr & 0x07ff];
    else if (addr < 0x4000)
    {
        switch (addr & 0x0007)
        {
        case 0:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUCTRL>();
        case 1:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUMASK>();
        case 2:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUSTATUS>();
        case 3:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::OAMADDR>();
        case 4:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::OAMDATA>();
        case 5:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUSCROLL>();
        case 6:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUADDR>();
        case 7:
            return dptr->cpuOpenBusData = dptr->fc->getPPU()->get<PPU::Registers::PPUDATA>();
        default:
            return dptr->cpuOpenBusData;
        }
    }
    else if (addr == 0x4015) return dptr->cpuOpenBusData = dptr->fc->getAPU()->get<0x15>() | (dptr->cpuOpenBusData & 0x20); //APU
    else if (addr == 0x4016) //Joypad 1
    {
        auto joypad = dptr->fc->getJoypad(0);
        return dptr->cpuOpenBusData = (joypad != nullptr) ? joypad->read() : 0;
    }
    else if (addr == 0x4017) //Joypad 2
    {
        auto joypad = dptr->fc->getJoypad(1);
        return dptr->cpuOpenBusData = (joypad != nullptr) ? joypad->read() : 0;
    }
    else if (addr >= 0x4100) return dptr->cpuOpenBusData = dptr->fc->getCartridge()->readPRG(addr);
    else return dptr->cpuOpenBusData;
}
template<>
void fcpp::core::Bus::write<fcpp::core::CPU>(std::uint16_t addr, const std::uint8_t data) noexcept
{
    dptr->cpuOpenBusData = data;
    if (addr < 0x2000) dptr->ram[addr & 0x07ff] = data;
    else if (addr < 0x4000)
    {
        switch (addr & 0x0007)
        {
        case 0:
            dptr->fc->getPPU()->set<PPU::Registers::PPUCTRL>(data);
            break;
        case 1:
            dptr->fc->getPPU()->set<PPU::Registers::PPUMASK>(data);
            break;
        case 2:
            dptr->fc->getPPU()->set<PPU::Registers::PPUSTATUS>(data);
            break;
        case 3:
            dptr->fc->getPPU()->set<PPU::Registers::OAMADDR>(data);
            break;
        case 4:
            dptr->fc->getPPU()->set<PPU::Registers::OAMDATA>(data);
            break;
        case 5:
            dptr->fc->getPPU()->set<PPU::Registers::PPUSCROLL>(data);
            break;
        case 6:
            dptr->fc->getPPU()->set<PPU::Registers::PPUADDR>(data);
            break;
        case 7:
            dptr->fc->getPPU()->set<PPU::Registers::PPUDATA>(data);
            break;
        }
    }
    else if (addr <= 0x4013)
    {
        switch (addr & 0xff)
        {
        case 0x00:
            dptr->fc->getAPU()->set<0x00>(data);
            break;
        case 0x01:
            dptr->fc->getAPU()->set<0x01>(data);
            break;
        case 0x02:
            dptr->fc->getAPU()->set<0x02>(data);
            break;
        case 0x03:
            dptr->fc->getAPU()->set<0x03>(data);
            break;
        case 0x04:
            dptr->fc->getAPU()->set<0x04>(data);
            break;
        case 0x05:
            dptr->fc->getAPU()->set<0x05>(data);
            break;
        case 0x06:
            dptr->fc->getAPU()->set<0x06>(data);
            break;
        case 0x07:
            dptr->fc->getAPU()->set<0x07>(data);
            break;
        case 0x08:
            dptr->fc->getAPU()->set<0x08>(data);
            break;
        case 0x0a:
            dptr->fc->getAPU()->set<0x0a>(data);
            break;
        case 0x0b:
            dptr->fc->getAPU()->set<0x0b>(data);
            break;
        case 0x0c:
            dptr->fc->getAPU()->set<0x0c>(data);
            break;
        case 0x0e:
            dptr->fc->getAPU()->set<0x0e>(data);
            break;
        case 0x0f:
            dptr->fc->getAPU()->set<0x0f>(data);
            break;
        case 0x10:
            dptr->fc->getAPU()->set<0x10>(data);
            break;
        case 0x11:
            dptr->fc->getAPU()->set<0x11>(data);
            break;
        case 0x12:
            dptr->fc->getAPU()->set<0x12>(data);
            break;
        case 0x13:
            dptr->fc->getAPU()->set<0x13>(data);
            break;
        }
    }
    else if (addr == 0x4014) dptr->fc->getCPU()->requestDMA(0x2004, data);
    else if (addr == 0x4015) dptr->fc->getAPU()->set<0x15>(data);
    else if (addr == 0x4016)
    {
        fcpp::core::Joypad* joypad = nullptr;
        for (int i = 0; (joypad = dptr->fc->getJoypad(i)) != nullptr; i++) joypad->write(data);
    }
    else if (addr == 0x4017) dptr->fc->getAPU()->set<0x17>(data);
    else if (addr >= 0x4100) dptr->fc->getCartridge()->writePRG(addr, data);
}

template<>
std::uint8_t fcpp::core::Bus::read<fcpp::core::PPU>(std::uint16_t addr) noexcept
{
    addr &= 0x3fff;
    if (addr < 0x2000) return dptr->fc->getCartridge()->readCHR(addr);
    else if (addr < 0x3f00) return dptr->vram[detail::nameTableAddress(addr, dptr->fc->getCartridge()->getMirrorType())];
    else return dptr->pram[addr & ((addr & 0x0003) == 0x0000 ? 0x000f : 0x001f)];
}
template<>
void fcpp::core::Bus::write<fcpp::core::PPU>(std::uint16_t addr, const std::uint8_t data) noexcept
{
    addr &= 0x3fff;
    if (addr < 0x2000) dptr->fc->getCartridge()->writeCHR(addr, data);
    else if (addr < 0x3f00) dptr->vram[detail::nameTableAddress(addr, dptr->fc->getCartridge()->getMirrorType())] = data;
    else dptr->pram[addr & ((addr & 0x0003) == 0x0000 ? 0x000f : 0x001f)] = data;
}
