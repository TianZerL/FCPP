#include "FCPP/Tools/Debugger.hpp"

namespace fcpp::tools::detail
{
    inline static const char* instructionDecode(std::uint32_t opcode) noexcept
    {
        switch (opcode)
        {
        case 0x00: return "BRK:imp";
        case 0x01: return "ORA:izx";
        case 0x02: return "STP:imp";
        case 0x03: return "SLO:izx";
        case 0x04: return "NOP:zp0";
        case 0x05: return "ORA:zp0";
        case 0x06: return "ASL:zp0";
        case 0x07: return "SLO:zp0";
        case 0x08: return "PHP:imp";
        case 0x09: return "ORA:imm";
        case 0x0a: return "ASL:imp";
        case 0x0b: return "ANC:imm";
        case 0x0c: return "NOP:abs";
        case 0x0d: return "ORA:abs";
        case 0x0e: return "ASL:abs";
        case 0x0f: return "SLO:abs";
        case 0x10: return "BPL:imp";
        case 0x11: return "ORA:izy";
        case 0x12: return "STP:imp";
        case 0x13: return "SLO:izy";
        case 0x14: return "NOP:zpx";
        case 0x15: return "ORA:zpx";
        case 0x16: return "ASL:zpx";
        case 0x17: return "SLO:zpx";
        case 0x18: return "CLC:imp";
        case 0x19: return "ORA:aby";
        case 0x1a: return "NOP:imp";
        case 0x1b: return "SLO:aby";
        case 0x1c: return "NOP:abx";
        case 0x1d: return "ORA:abx";
        case 0x1e: return "ASL:abx";
        case 0x1f: return "SLO:abx";
        case 0x20: return "JSR:imp";
        case 0x21: return "AND:izx";
        case 0x22: return "STP:imp";
        case 0x23: return "RLA:izx";
        case 0x24: return "BIT:zp0";
        case 0x25: return "AND:zp0";
        case 0x26: return "ROL:zp0";
        case 0x27: return "RLA:zp0";
        case 0x28: return "PLP:imp";
        case 0x29: return "AND:imm";
        case 0x2a: return "ROL:imp";
        case 0x2b: return "ANC:imm";
        case 0x2c: return "BIT:abs";
        case 0x2d: return "AND:abs";
        case 0x2e: return "ROL:abs";
        case 0x2f: return "RLA:abs";
        case 0x30: return "BMI:imp";
        case 0x31: return "AND:izy";
        case 0x32: return "STP:imp";
        case 0x33: return "RLA:izy";
        case 0x34: return "NOP:zpx";
        case 0x35: return "AND:zpx";
        case 0x36: return "ROL:zpx";
        case 0x37: return "RLA:zpx";
        case 0x38: return "SEC:imp";
        case 0x39: return "AND:aby";
        case 0x3a: return "NOP:imp";
        case 0x3b: return "RLA:aby";
        case 0x3c: return "NOP:abx";
        case 0x3d: return "AND:abx";
        case 0x3e: return "ROL:abx";
        case 0x3f: return "RLA:abx";
        case 0x40: return "RTI:imp";
        case 0x41: return "EOR:izx";
        case 0x42: return "STP:imp";
        case 0x43: return "SRE:izx";
        case 0x44: return "NOP:zp0";
        case 0x45: return "EOR:zp0";
        case 0x46: return "LSR:zp0";
        case 0x47: return "SRE:zp0";
        case 0x48: return "PHA:imp";
        case 0x49: return "EOR:imm";
        case 0x4a: return "LSR:imp";
        case 0x4b: return "ASR:imm";
        case 0x4c: return "JMP:abs";
        case 0x4d: return "EOR:abs";
        case 0x4e: return "LSR:abs";
        case 0x4f: return "SRE:abs";
        case 0x50: return "BVC:imp";
        case 0x51: return "EOR:izy";
        case 0x52: return "STP:imp";
        case 0x53: return "SRE:izy";
        case 0x54: return "NOP:zpx";
        case 0x55: return "EOR:zpx";
        case 0x56: return "LSR:zpx";
        case 0x57: return "SRE:zpx";
        case 0x58: return "CLI:imp";
        case 0x59: return "EOR:aby";
        case 0x5a: return "NOP:imp";
        case 0x5b: return "SRE:aby";
        case 0x5c: return "NOP:abx";
        case 0x5d: return "EOR:abx";
        case 0x5e: return "LSR:abx";
        case 0x5f: return "SRE:abx";
        case 0x60: return "RTS:imp";
        case 0x61: return "ADC:izx";
        case 0x62: return "STP:imp";
        case 0x63: return "RRA:izx";
        case 0x64: return "NOP:zp0";
        case 0x65: return "ADC:zp0";
        case 0x66: return "ROR:zp0";
        case 0x67: return "RRA:zp0";
        case 0x68: return "PLA:imp";
        case 0x69: return "ADC:imm";
        case 0x6a: return "ROR:imp";
        case 0x6b: return "ARR:imm";
        case 0x6c: return "JMP:ind";
        case 0x6d: return "ADC:abs";
        case 0x6e: return "ROR:abs";
        case 0x6f: return "RRA:abs";
        case 0x70: return "BVS:imp";
        case 0x71: return "ADC:izy";
        case 0x72: return "STP:imp";
        case 0x73: return "RRA:izy";
        case 0x74: return "NOP:zpx";
        case 0x75: return "ADC:zpx";
        case 0x76: return "ROR:zpx";
        case 0x77: return "RRA:zpx";
        case 0x78: return "SEI:imp";
        case 0x79: return "ADC:aby";
        case 0x7a: return "NOP:imp";
        case 0x7b: return "RRA:aby";
        case 0x7c: return "NOP:abx";
        case 0x7d: return "ADC:abx";
        case 0x7e: return "ROR:abx";
        case 0x7f: return "RRA:abx";
        case 0x80: return "NOP:imm";
        case 0x81: return "STA:izx";
        case 0x82: return "NOP:imm";
        case 0x83: return "SAX:izx";
        case 0x84: return "STY:zp0";
        case 0x85: return "STA:zp0";
        case 0x86: return "STX:zp0";
        case 0x87: return "SAX:zp0";
        case 0x88: return "DEY:imp";
        case 0x89: return "NOP:imm";
        case 0x8a: return "TXA:imp";
        case 0x8b: return "ANE:imm";
        case 0x8c: return "STY:abs";
        case 0x8d: return "STA:abs";
        case 0x8e: return "STX:abs";
        case 0x8f: return "SAX:abs";
        case 0x90: return "BCC:imp";
        case 0x91: return "STA:izy";
        case 0x92: return "STP:imp";
        case 0x93: return "SHA:izy";
        case 0x94: return "STY:zpx";
        case 0x95: return "STA:zpx";
        case 0x96: return "STX:zpy";
        case 0x97: return "SAX:zpy";
        case 0x98: return "TYA:imp";
        case 0x99: return "STA:aby";
        case 0x9a: return "TXS:imp";
        case 0x9b: return "SHS:aby";
        case 0x9c: return "SHY:abx";
        case 0x9d: return "STA:abx";
        case 0x9e: return "SHX:aby";
        case 0x9f: return "SHA:aby";
        case 0xa0: return "LDY:imm";
        case 0xa1: return "LDA:izx";
        case 0xa2: return "LDX:imm";
        case 0xa3: return "LAX:izx";
        case 0xa4: return "LDY:zp0";
        case 0xa5: return "LDA:zp0";
        case 0xa6: return "LDX:zp0";
        case 0xa7: return "LAX:zp0";
        case 0xa8: return "TAY:imp";
        case 0xa9: return "LDA:imm";
        case 0xaa: return "TAX:imp";
        case 0xab: return "LXA:imm";
        case 0xac: return "LDY:abs";
        case 0xad: return "LDA:abs";
        case 0xae: return "LDX:abs";
        case 0xaf: return "LAX:abs";
        case 0xb0: return "BCS:imp";
        case 0xb1: return "LDA:izy";
        case 0xb2: return "STP:imp";
        case 0xb3: return "LAX:izy";
        case 0xb4: return "LDY:zpx";
        case 0xb5: return "LDA:zpx";
        case 0xb6: return "LDX:zpy";
        case 0xb7: return "LAX:zpy";
        case 0xb8: return "CLV:imp";
        case 0xb9: return "LDA:aby";
        case 0xba: return "TSX:imp";
        case 0xbb: return "LAS:aby";
        case 0xbc: return "LDY:abx";
        case 0xbd: return "LDA:abx";
        case 0xbe: return "LDX:aby";
        case 0xbf: return "LAX:aby";
        case 0xc0: return "CPY:imm";
        case 0xc1: return "CMP:izx";
        case 0xc2: return "NOP:imm";
        case 0xc3: return "DCP:izx";
        case 0xc4: return "CPY:zp0";
        case 0xc5: return "CMP:zp0";
        case 0xc6: return "DEC:zp0";
        case 0xc7: return "DCP:zp0";
        case 0xc8: return "INY:imp";
        case 0xc9: return "CMP:imm";
        case 0xca: return "DEX:imp";
        case 0xcb: return "SBX:imm";
        case 0xcc: return "CPY:abs";
        case 0xcd: return "CMP:abs";
        case 0xce: return "DEC:abs";
        case 0xcf: return "DCP:abs";
        case 0xd0: return "BNE:imp";
        case 0xd1: return "CMP:izy";
        case 0xd2: return "STP:imp";
        case 0xd3: return "DCP:izy";
        case 0xd4: return "NOP:zpx";
        case 0xd5: return "CMP:zpx";
        case 0xd6: return "DEC:zpx";
        case 0xd7: return "DCP:zpx";
        case 0xd8: return "CLD:imp";
        case 0xd9: return "CMP:aby";
        case 0xda: return "NOP:imp";
        case 0xdb: return "DCP:aby";
        case 0xdc: return "NOP:abx";
        case 0xdd: return "CMP:abx";
        case 0xde: return "DEC:abx";
        case 0xdf: return "DCP:abx";
        case 0xe0: return "CPX:imm";
        case 0xe1: return "SBC:izx";
        case 0xe2: return "NOP:imm";
        case 0xe3: return "ISB:izx";
        case 0xe4: return "CPX:zp0";
        case 0xe5: return "SBC:zp0";
        case 0xe6: return "INC:zp0";
        case 0xe7: return "ISB:zp0";
        case 0xe8: return "INX:imp";
        case 0xe9: return "SBC:imm";
        case 0xea: return "NOP:imp";
        case 0xeb: return "SBC:imm";
        case 0xec: return "CPX:abs";
        case 0xed: return "SBC:abs";
        case 0xee: return "INC:abs";
        case 0xef: return "ISB:abs";
        case 0xf0: return "BEQ:imp";
        case 0xf1: return "SBC:izy";
        case 0xf2: return "STP:imp";
        case 0xf3: return "ISB:izy";
        case 0xf4: return "NOP:zpx";
        case 0xf5: return "SBC:zpx";
        case 0xf6: return "INC:zpx";
        case 0xf7: return "ISB:zpx";
        case 0xf8: return "SED:imp";
        case 0xf9: return "SBC:aby";
        case 0xfa: return "NOP:imp";
        case 0xfb: return "ISB:aby";
        case 0xfc: return "NOP:abx";
        case 0xfd: return "SBC:abx";
        case 0xfe: return "INC:abx";
        case 0xff: return "ISB:abx";
        }
        return "Unknown";
    }
}

struct fcpp::tools::Debugger::DebuggerData
{
    fcpp::core::CPU* cpu = nullptr;
    fcpp::core::Bus* bus = nullptr;

    std::unique_ptr<std::uint32_t[]> patternTableBuffer{};

    const char* instruction = nullptr;
    fcpp::core::CPU::Registers registers{};

    static constexpr std::uint32_t paletteTable[64] = {
        0xff7c7c7c, 0xff0000fc, 0xff0000bc, 0xff4428bc, 0xff940084, 0xffa80020, 0xffa81000, 0xff881400,
        0xff503000, 0xff007800, 0xff006800, 0xff005800, 0xff004058, 0xff000000, 0xff000000, 0xff000000,
        0xffbcbcbc, 0xff0078f8, 0xff0058f8, 0xff6844fc, 0xffd800cc, 0xffe40058, 0xfff83800, 0xffe45c10,
        0xffac7c00, 0xff00b800, 0xff00a800, 0xff00a844, 0xff008888, 0xff000000, 0xff000000, 0xff000000,
        0xfff8f8f8, 0xff3cbcfc, 0xff6888fc, 0xff9878f8, 0xfff878f8, 0xfff85898, 0xfff87858, 0xfffca044,
        0xfff8b800, 0xffb8f818, 0xff58d854, 0xff58f898, 0xff00e8d8, 0xff787878, 0xff000000, 0xff000000,
        0xfffcfcfc, 0xffa4e4fc, 0xffb8b8f8, 0xffd8b8f8, 0xfff8b8f8, 0xfff8a4c0, 0xfff0d0b0, 0xfffce0a8,
        0xfff8d878, 0xffd8f878, 0xffb8f8b8, 0xffb8f8d8, 0xff00fcfc, 0xfff8d8f8, 0xff000000, 0xff000000
    };
};

fcpp::tools::Debugger::Debugger() : dptr(std::make_unique<DebuggerData>()) {}
fcpp::tools::Debugger::~Debugger() noexcept = default;

void fcpp::tools::Debugger::connect(fcpp::core::FC* const fc) noexcept
{
    dptr->cpu = fc->getCPU();
    dptr->bus = fc->getBus();
}

fcpp::tools::Debugger::MemoryView fcpp::tools::Debugger::getRamView() const noexcept
{
    return DataView<std::uint8_t>{dptr->bus->dump<fcpp::core::Bus::MemoryType::RAM>(), 0x0800};
}
fcpp::tools::Debugger::MemoryView fcpp::tools::Debugger::getVRamView() const noexcept
{
    return DataView<std::uint8_t>{dptr->bus->dump<fcpp::core::Bus::MemoryType::VRAM>(), 0x1000};
}
fcpp::tools::Debugger::MemoryView fcpp::tools::Debugger::getPRamView() const noexcept
{
    return DataView<std::uint8_t>{dptr->bus->dump<fcpp::core::Bus::MemoryType::PRAM>(), 0x20};
}
fcpp::tools::Debugger::CPUView fcpp::tools::Debugger::getCPUView() const noexcept
{
    return CPUView{ &dptr->instruction, &dptr->registers, [this](CPUView& view) {
            dptr->registers = dptr->cpu->dump();
            dptr->instruction = detail::instructionDecode(dptr->bus->read<fcpp::core::CPU>(dptr->registers.pc));
        }
    };
}
fcpp::tools::Debugger::PatternTableView fcpp::tools::Debugger::getPatternTableView() const noexcept
{
    if (!dptr->patternTableBuffer) dptr->patternTableBuffer = std::make_unique<std::uint32_t[]>(32768);
    auto buffer = dptr->patternTableBuffer.get();
    auto bus = dptr->bus;
    auto pram = bus->dump<fcpp::core::Bus::MemoryType::PRAM>();
    return PatternTableView{ dptr->patternTableBuffer.get(), [=]()
        {
            for (int tile = 0; tile < 0x200; tile++)
            {
                int page = ((tile >> 4) & 0x10) ^ 0x10;
                int pixelCol = (tile & 15) * 8;
                int pixelRow = (tile / 16) * 8;
                int pixelPos = pixelRow * 16 * 8 + pixelCol;

                std::uint8_t bytes[16] = {};
                std::uint8_t* plane0 = bytes;
                std::uint8_t* plane1 = bytes + 8;

                for (int count = 0; count < 16; count++) bytes[count] = bus->read<fcpp::core::PPU>(tile * 16 + count);

                for (int row = 0; row < 8; row++) for (int col = 0; col < 8; col++)
                {
                    int paletteIdx = page | (((plane1[row] >> (7 - col)) & 1) << 1) | ((plane0[row] >> (7 - col)) & 1);
                    int colorIdx = 0x3f & pram[paletteIdx];
                    int pixelIdx = pixelPos + row * 16 * 8 + col;
                    buffer[pixelIdx] = DebuggerData::paletteTable[colorIdx];
                }
            }
        }
    };
}
