#include "FCPP/Core/CPU.hpp"
#include "FCPP/Core/FC.hpp"

#define T clock->tick()
#define I pollInterrupt()

namespace fcpp::core::detail
{
    class CPUImpl
    {
    private:
        using Mode = std::uint16_t(CPUImpl::*)();

        enum class InterruptType
        {
            NMI, IRQ, BRK
        };

        struct InternalState
        {
            std::uint8_t dmaState : 2;
            std::uint8_t tickState : 1;
            bool requestNMI : 1;
            bool requestIRQ : 1;
            bool detectedNMI : 1;
            bool detectedIRQ : 1;

            std::uint8_t operator=(const std::uint8_t value) noexcept
            {
                dmaState = value & 3;
                tickState = value & (1 << 2);
                requestNMI = value & (1 << 3);
                requestIRQ = value & (1 << 4);
                detectedNMI = value & (1 << 5);
                detectedIRQ = value & (1 << 6);

                return value;
            }
            operator std::uint8_t() const noexcept
            {
                return detectedIRQ << 6 | detectedNMI << 5 | requestIRQ << 4 | requestNMI << 3 | tickState << 2 | dmaState;
            }
        };

        struct StatusRegister
        {
            std::uint8_t c : 1; //Carry
            std::uint8_t z : 1; //Zero
            std::uint8_t i : 1; //Interrupt
            std::uint8_t d : 1; //Decimal
            std::uint8_t b : 1; //Break
            std::uint8_t u : 1; //Unused
            std::uint8_t v : 1; //Overflow
            std::uint8_t n : 1; //Negative

            StatusRegister() noexcept :
                c(0), z(0), i(0), d(0),
                b(0), u(1), v(0), n(0) {}

            std::uint8_t operator=(const std::uint8_t value) noexcept
            {
                c = value & (1 << 0) ? 1 : 0;
                z = value & (1 << 1) ? 1 : 0;
                i = value & (1 << 2) ? 1 : 0;
                d = value & (1 << 3) ? 1 : 0;
                b = value & (1 << 4) ? 1 : 0;
                v = value & (1 << 6) ? 1 : 0;
                n = value & (1 << 7) ? 1 : 0;

                return value;
            }
            operator std::uint8_t() const noexcept
            {
                return
                    n << 7 | v << 6 | u << 5 | b << 4 |
                    d << 3 | i << 2 | z << 1 | c;
            }
            void updateNZ(std::uint8_t value) noexcept
            {
                n = (value & 0x80) ? 1 : 0;
                z = (value == 0) ? 1 : 0;
            }
        };
    private:
        template<typename Offset>
        static bool isCrossedPage(std::uint16_t addr, Offset offset) noexcept;
        void pollInterrupt() noexcept;
        void write(std::uint16_t addr, std::uint8_t data) noexcept;
        std::uint8_t read(std::uint16_t addr) noexcept;
        void push(std::uint8_t data) noexcept;
        std::uint8_t pop() noexcept;
    private: // reference https://www.nesdev.com/6502_cpu.txt
        template<InterruptType type> void interrupt() noexcept;
        void branch(bool condition) noexcept;

        std::uint16_t imm() noexcept;
        std::uint16_t zp0() noexcept;
        std::uint16_t zpx() noexcept;
        std::uint16_t zpy() noexcept;
        std::uint16_t ind() noexcept;
        std::uint16_t izx() noexcept;
        template<bool poll = false> std::uint16_t abs() noexcept;
        template<bool cross = true> std::uint16_t abx() noexcept;
        template<bool cross = true> std::uint16_t aby() noexcept;
        template<bool cross = true> std::uint16_t izy() noexcept;

        void STP() noexcept;

        void BRK() noexcept; void RTI() noexcept; void RTS() noexcept; void PHA() noexcept;
        void PHP() noexcept; void PLA() noexcept; void PLP() noexcept; void JSR() noexcept;

        void TAX() noexcept; void TAY() noexcept; void TXA() noexcept; void TXS() noexcept;
        void TYA() noexcept; void TSX() noexcept;

        void BCC() noexcept; void BCS() noexcept; void BNE() noexcept; void BEQ() noexcept;
        void BPL() noexcept; void BMI() noexcept; void BVC() noexcept; void BVS() noexcept;

        void CLC() noexcept; void CLD() noexcept; void CLI() noexcept; void CLV() noexcept;
        void SEC() noexcept; void SED() noexcept; void SEI() noexcept;

        void INX() noexcept; void INY() noexcept; void DEX() noexcept; void DEY() noexcept;

        template<Mode mode> void JMP() noexcept;
        template<Mode mode> void LDA() noexcept;
        template<Mode mode> void LDX() noexcept;
        template<Mode mode> void LDY() noexcept;
        template<Mode mode> void EOR() noexcept;
        template<Mode mode> void AND() noexcept;
        template<Mode mode> void ORA() noexcept;
        template<Mode mode> void ADC() noexcept;
        template<Mode mode> void SBC() noexcept;
        template<Mode mode> void CMP() noexcept;
        template<Mode mode> void CPX() noexcept;
        template<Mode mode> void CPY() noexcept;
        template<Mode mode> void BIT() noexcept;
        template<Mode mode> void LAX() noexcept;
        template<Mode mode> void ANE() noexcept;
        template<Mode mode> void LXA() noexcept;
        template<Mode mode> void ANC() noexcept;
        template<Mode mode> void ASR() noexcept;
        template<Mode mode> void ARR() noexcept;
        template<Mode mode> void LAS() noexcept;
        template<Mode mode> void SBX() noexcept;
        template<Mode mode = nullptr> void NOP() noexcept;
        template<Mode mode = nullptr> void ASL() noexcept;
        template<Mode mode = nullptr> void LSR() noexcept;
        template<Mode mode = nullptr> void ROL() noexcept;
        template<Mode mode = nullptr> void ROR() noexcept;
        template<Mode mode> void INC() noexcept;
        template<Mode mode> void DEC() noexcept;
        template<Mode mode> void SLO() noexcept;
        template<Mode mode> void SRE() noexcept;
        template<Mode mode> void RLA() noexcept;
        template<Mode mode> void RRA() noexcept;
        template<Mode mode> void ISB() noexcept;
        template<Mode mode> void DCP() noexcept;
        template<Mode mode> void STA() noexcept;
        template<Mode mode> void STX() noexcept;
        template<Mode mode> void STY() noexcept;
        template<Mode mode> void SHA() noexcept;
        template<Mode mode> void SHX() noexcept;
        template<Mode mode> void SHY() noexcept;
        template<Mode mode> void SAX() noexcept;
        template<Mode mode> void SHS() noexcept;
    public:
        void connect(Bus* bus, Clock* clock) noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void clear() noexcept;
        void exec() noexcept;

        void dma(std::uint16_t dst, std::uint16_t src, std::uint16_t size) noexcept;
        void reset() noexcept;
        void requestNMI(bool status) noexcept;
        void requestIRQ(bool status) noexcept;

        template<CPU::State::Type type> unsigned int get() const noexcept;
    private:
        std::uint16_t pc = 0;
        std::uint8_t a = 0, x = 0, y = 0, sp = 0;
        StatusRegister p{};
        InternalState i{};
    private:
        Bus* bus = nullptr;
        Clock* clock = nullptr;
    private:
        static constexpr std::uint16_t NMI_VECTOR = 0xfffa;
        static constexpr std::uint16_t RESET_VECTOR = 0xfffc;
        static constexpr std::uint16_t IRQ_VECTOR = 0xfffe;
    };

    template<typename Offset>
    inline bool CPUImpl::isCrossedPage(const std::uint16_t addr, const Offset offset) noexcept
    {
        return ((addr + offset) & 0xff00) != (addr & 0xff00);
    }
    inline void CPUImpl::pollInterrupt() noexcept
    {
        if (i.requestNMI) // The NMI input is edge-sensitive
        {
            i.requestNMI = false;
            i.detectedNMI = true;
        }
        else if (i.requestIRQ && !p.i) i.detectedIRQ = true; // IRQ input is level-sensitive
    }
    inline void CPUImpl::write(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        i.tickState = CPU::State::TICK_STATE_WRITE; T;
        bus->write<CPU>(addr, data);
    }
    inline std::uint8_t CPUImpl::read(const std::uint16_t addr) noexcept
    {
        i.tickState = CPU::State::TICK_STATE_READ; T;
        return bus->read<CPU>(addr);
    }
    inline void CPUImpl::push(const std::uint8_t data) noexcept
    {
        write(0x100 + (sp--), data);
    }
    inline std::uint8_t CPUImpl::pop() noexcept
    {
        return read(0x100 + (++sp));
    }

    template<CPUImpl::InterruptType type>
    inline void CPUImpl::interrupt() noexcept
    {
        if (type != InterruptType::BRK)
        {
            read(pc);
            read(pc);
        }

        push(static_cast<std::uint8_t>(pc >> 8));
        push(static_cast<std::uint8_t>(pc & 0xff));

        std::uint16_t vector = NMI_VECTOR;
        if (i.requestNMI) i.requestNMI = false;
        else if (type != InterruptType::NMI) vector = IRQ_VECTOR;

        if (type == InterruptType::BRK) push(p | 0x10);
        else push(p & 0xef);

        pc = read(vector);
        p.i = 1;
        pc |= static_cast<std::uint16_t>(read(vector + 1)) << 8;
    }
    inline void CPUImpl::branch(const bool condition) noexcept
    {
        I;
        std::uint8_t m = read(pc++);
        auto offset = static_cast<std::int16_t>(m < 128 ? m : m - 256);
        if (condition)
        {
            read(pc);
            if (isCrossedPage(pc, offset))
            {
                I;
                read((pc & 0xff00) | ((pc + offset) & 0x00ff));
            }
            pc += offset;
        }
    }
    inline std::uint16_t CPUImpl::imm() noexcept
    {
        return pc++;
    }
    inline std::uint16_t CPUImpl::zp0() noexcept
    {
        return read(pc++);
    }
    inline std::uint16_t CPUImpl::zpx() noexcept
    {
        std::uint16_t addr = zp0();
        read(addr);
        return (addr + x) & 0xff;
    }
    inline std::uint16_t CPUImpl::zpy() noexcept
    {
        std::uint16_t addr = zp0();
        read(addr);
        return (addr + y) & 0xff;
    }
    inline std::uint16_t CPUImpl::ind() noexcept
    {
        std::uint16_t pointer = abs();
        std::uint16_t addr = read(pointer); I;
        addr |= read((pointer & 0xff00) | ((pointer + 1) & 0x00ff)) << 8;
        return addr;
    }
    inline std::uint16_t CPUImpl::izx() noexcept
    {
        std::uint16_t pointer = zpx();
        std::uint16_t addr = read(pointer);
        addr |= read((pointer + 1) & 0x00ff) << 8;
        return addr;
    }
    template<bool poll> inline std::uint16_t CPUImpl::abs() noexcept
    {
        std::uint16_t addr = read(pc++);
        if (poll) I;
        addr |= read(pc++) << 8;
        return addr;
    }
    template<> inline std::uint16_t CPUImpl::abx<false>() noexcept
    {
        std::uint16_t addr = abs();
        read((addr & 0xff00) | ((addr + x) & 0x00ff));
        return addr + x;
    }
    template<> inline std::uint16_t CPUImpl::abx<true>() noexcept
    {
        std::uint16_t addr = abs();
        if (isCrossedPage(addr, x)) read((addr & 0xff00) | ((addr + x) & 0x00ff));
        return addr + x;
    }
    template<> inline std::uint16_t CPUImpl::aby<false>() noexcept
    {
        std::uint16_t addr = abs();
        read((addr & 0xff00) | ((addr + y) & 0x00ff));
        return addr + y;
    }
    template<> inline std::uint16_t CPUImpl::aby<true>() noexcept
    {
        std::uint16_t addr = abs();
        if (isCrossedPage(addr, y)) read((addr & 0xff00) | ((addr + y) & 0x00ff));
        return addr + y;
    }
    template<> inline std::uint16_t CPUImpl::izy<false>() noexcept
    {
        std::uint16_t pointer = zp0();
        std::uint16_t addr = read(pointer);
        addr |= read((pointer + 1) & 0x00ff) << 8;
        return addr + y;
    }
    template<> inline std::uint16_t CPUImpl::izy<true>() noexcept
    {
        std::uint16_t addr = izy<false>();
        if (isCrossedPage(addr - y, y)) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        return addr;
    }

    inline void CPUImpl::STP() noexcept
    {
        // Now you have frozen the CPU, we will reset it
        reset();
    }
    inline void CPUImpl::BRK() noexcept
    {
        read(pc++);
        interrupt<InterruptType::BRK>();
    }
    inline void CPUImpl::RTI() noexcept
    {
        read(pc); T;
        p = pop();
        pc = pop(); I;
        pc |= static_cast<std::uint16_t>(pop()) << 8;
    }
    inline void CPUImpl::RTS() noexcept
    {
        read(pc); T;
        pc = pop();
        pc |= static_cast<std::uint16_t>(pop()) << 8;
        pc++; I; T;
    }
    inline void CPUImpl::PHA() noexcept
    {
        read(pc); I;
        push(a);
    }
    inline void CPUImpl::PHP() noexcept
    {
        read(pc); I;
        push(p | 0x10);
    }
    inline void CPUImpl::PLA() noexcept
    {
        read(pc); T; I;
        a = pop();
        p.updateNZ(a);
    }
    inline void CPUImpl::PLP() noexcept
    {
        read(pc); T; I;
        p = pop();
    }
    inline void CPUImpl::JSR() noexcept
    {
        std::uint16_t addr = read(pc++); T;
        push(static_cast<std::uint8_t>(pc >> 8));
        push(static_cast<std::uint8_t>(pc)); I;
        pc = (read(pc) << 8) | addr;
    }
    inline void CPUImpl::TAX() noexcept
    {
        I; read(pc);
        p.updateNZ(x = a);
    }
    inline void CPUImpl::TAY() noexcept
    {
        I; read(pc);
        p.updateNZ(y = a);
    }
    inline void CPUImpl::TXA() noexcept
    {
        I; read(pc);
        p.updateNZ(a = x);
    }
    inline void CPUImpl::TXS() noexcept
    {
        I; read(pc);
        sp = x;
    }
    inline void CPUImpl::TYA() noexcept
    {
        I; read(pc);
        p.updateNZ(a = y);
    }
    inline void CPUImpl::TSX() noexcept
    {
        I; read(pc);
        p.updateNZ(x = sp);
    }
    inline void CPUImpl::BCC() noexcept
    {
        branch(p.c == 0);
    }
    inline void CPUImpl::BCS() noexcept
    {
        branch(p.c == 1);
    }
    inline void CPUImpl::BNE() noexcept
    {
        branch(p.z == 0);
    }
    inline void CPUImpl::BEQ() noexcept
    {
        branch(p.z == 1);
    }
    inline void CPUImpl::BPL() noexcept
    {
        branch(p.n == 0);
    }
    inline void CPUImpl::BMI() noexcept
    {
        branch(p.n == 1);
    }
    inline void CPUImpl::BVC() noexcept
    {
        branch(p.v == 0);
    }
    inline void CPUImpl::BVS() noexcept
    {
        branch(p.v == 1);
    }
    inline void CPUImpl::CLC() noexcept
    {
        I; read(pc);
        p.c = 0;
    }
    inline void CPUImpl::CLD() noexcept
    {
        I; read(pc);
        p.d = 0;
    }
    inline void CPUImpl::CLI() noexcept
    {
        I; read(pc);
        p.i = 0;
    }
    inline void CPUImpl::CLV() noexcept
    {
        I; read(pc);
        p.v = 0;
    }
    inline void CPUImpl::SEC() noexcept
    {
        I; read(pc);
        p.c = 1;
    }
    inline void CPUImpl::SED() noexcept
    {
        I; read(pc);
        p.d = 1;
    }
    inline void CPUImpl::SEI() noexcept
    {
        I; read(pc);
        p.i = 1;
    }
    inline void CPUImpl::INX() noexcept
    {
        I; read(pc);
        p.updateNZ(++x);
    }
    inline void CPUImpl::INY() noexcept
    {
        I; read(pc);
        p.updateNZ(++y);
    }
    inline void CPUImpl::DEX() noexcept
    {
        I; read(pc);
        p.updateNZ(--x);
    }
    inline void CPUImpl::DEY() noexcept
    {
        I; read(pc);
        p.updateNZ(--y);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::JMP() noexcept
    {
        pc = (this->*mode)();
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LDA() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(a = read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LDX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(x = read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LDY() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(y = read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::EOR() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(a ^= read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::AND() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(a &= read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ORA() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(a |= read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ADC() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);
        std::uint16_t r = a + m + p.c;

        p.c = r > 0xff ? 1 : 0;
        p.v = ((a ^ r) & (m ^ r) & 0x80) ? 1 : 0;
        p.updateNZ(a = static_cast<std::uint8_t>(r));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SBC() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = ~read(addr);
        std::uint16_t r = a + m + p.c;

        p.c = r > 0xff ? 1 : 0;
        p.v = ((a ^ r) & (m ^ r) & 0x80) ? 1 : 0;
        p.updateNZ(a = static_cast<std::uint8_t>(r));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::CMP() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);

        p.c = a >= m;
        p.updateNZ(a - m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::CPX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);

        p.c = x >= m;
        p.updateNZ(x - m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::CPY() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);

        p.c = y >= m;
        p.updateNZ(y - m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::BIT() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);

        p.z = (a & m) == 0;
        p.n = (m >> 7) & 1;
        p.v = (m >> 6) & 1;
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LAX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(x = a = read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ANE() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(a = (a | 0xee) & x & read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LXA() noexcept
    {// same as LAX in NES/FC
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(x = a = (a | 0xff) & read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ANC() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        a &= read(addr);
        p.c = a & 0x80 ? 1 : 0;
        p.updateNZ(a);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ASR() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        a &= read(addr);
        p.c = a & 0x01 ? 1 : 0;
        p.updateNZ(a >>= 1);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ARR() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        a &= read(addr);
        std::uint8_t r = (a >> 1) | (p.c << 7);
        p.c = (r >> 6) & 1;
        p.v = ((r >> 5) & 1) ^ ((r >> 6) & 1);
        p.updateNZ(a = r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LAS() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        p.updateNZ(x = a = sp &= read(addr));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SBX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        std::uint8_t m = read(addr);
        x &= a;
        p.c = x >= m;
        p.updateNZ(x -= m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::NOP() noexcept
    {
        if (mode != nullptr) (this->*mode)();
        I; T;
    }
    template<>
    inline void CPUImpl::ASL<nullptr>() noexcept
    {
        I; read(pc);
        p.c = a & 0x80 ? 1 : 0;
        p.updateNZ(a <<= 1);
    }
    template<>
    inline void CPUImpl::LSR<nullptr>() noexcept
    {
        I; read(pc);
        p.c = a & 0x01 ? 1 : 0;
        p.updateNZ(a >>= 1);
    }
    template<>
    inline void CPUImpl::ROL<nullptr>() noexcept
    {
        I; read(pc);
        std::uint8_t r = (a << 1) | p.c;
        p.c = a & 0x80 ? 1 : 0;
        p.updateNZ(a = r);
    }
    template<>
    inline void CPUImpl::ROR<nullptr>() noexcept
    {
        I; read(pc);
        std::uint8_t r = (a >> 1) | (p.c << 7);
        p.c = a & 0x01 ? 1 : 0;
        p.updateNZ(a = r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ASL() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = m << 1;
        write(addr, r);
        p.c = m & 0x80 ? 1 : 0;
        p.updateNZ(r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::LSR() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = m >> 1;
        write(addr, r);
        p.c = m & 0x01;
        p.updateNZ(r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ROL() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = (m << 1) | p.c;
        write(addr, r);
        p.c = m & 0x80 ? 1 : 0;
        p.updateNZ(r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ROR() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = (m >> 1) | (p.c << 7);
        write(addr, r);
        p.c = m & 0x01 ? 1 : 0;
        p.updateNZ(r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::INC() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        write(addr, ++m);
        p.updateNZ(m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::DEC() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        std::uint8_t m = read(addr);
        write(addr, m); I;
        write(addr, --m);
        p.updateNZ(m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SLO() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = m << 1;
        write(addr, r);
        p.c = m & 0x80 ? 1 : 0;
        p.updateNZ(a |= r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SRE() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = m >> 1;
        write(addr, r);
        p.c = m & 0x01 ? 1 : 0;
        p.updateNZ(a ^= r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::RLA() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = (m << 1) | p.c;
        write(addr, r);
        p.c = m & 0x80 ? 1 : 0;
        p.updateNZ(a &= r);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::RRA() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        std::uint8_t r = (m >> 1) | (p.c << 7);
        write(addr, r);

        std::uint16_t r2 = a + r + (m & 0x01);
        p.c = r2 > 0xff ? 1 : 0;
        p.v = ((a ^ r2) & (r ^ r2) & 0x80) ? 1 : 0;
        p.updateNZ(a = static_cast<std::uint8_t>(r2));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::ISB() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        write(addr, ++m);

        m = ~m;
        std::uint16_t r = a + m + p.c;
        p.c = r > 0xff ? 1 : 0;
        p.v = ((a ^ r) & (m ^ r) & 0x80) ? 1 : 0;
        p.updateNZ(a = static_cast<std::uint8_t>(r));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::DCP() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        std::uint8_t m = read(addr);
        write(addr, m); I;
        write(addr, --m);
        p.c = a >= m;
        p.updateNZ(a - m);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::STA() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        I;
        write(addr, a);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::STX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        write(addr, x);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::STY() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        write(addr, y);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SHA() noexcept
    {
        std::uint16_t addr = (this->*mode)();
        if (mode == &CPUImpl::izy<false>) read(((addr - y) & 0xff00) | ((addr) & 0x00ff));
        I;
        write(addr, a & x & ((addr >> 8) + 1));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SHX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        write((isCrossedPage(addr, -y) ? (x & addr) : addr), x & ((addr >> 8) + 1));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SHY() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        write((isCrossedPage(addr, -x) ? (y & addr) : addr), y & ((addr >> 8) + 1));
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SAX() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        write(addr, a & x);
    }
    template<CPUImpl::Mode mode>
    inline void CPUImpl::SHS() noexcept
    {
        std::uint16_t addr = (this->*mode)(); I;
        sp = a & x;
        write(addr, sp & ((addr >> 8) + 1));
    }

    void CPUImpl::connect(Bus* const bus, Clock* const clock) noexcept
    {
        this->bus = bus;
        this->clock = clock;
    }
    template<typename Accessor>
    inline void CPUImpl::access(Accessor& accessor) noexcept
    {
        accessor.access(pc);
        accessor.access(a);
        accessor.access(x);
        accessor.access(y);
        accessor.access(sp);
        accessor.access(p);
        accessor.access(i);
    }
    inline void CPUImpl::clear() noexcept
    {
        pc = a = x = y = sp = 0;
        p = {};
        i = {};
    }
    inline void CPUImpl::exec() noexcept
    {
        if (i.detectedNMI)
        {
            i.detectedNMI = false;
            interrupt<InterruptType::NMI>();
        }
        else if (i.detectedIRQ)
        {
            i.detectedIRQ = false;
            interrupt<InterruptType::IRQ>();
        }

        switch (read(pc++))
        {
        case 0x00: BRK();                       break;
        case 0x01: ORA<&CPUImpl::izx>();        break;
        case 0x02: STP();                       break;
        case 0x03: SLO<&CPUImpl::izx>();        break;
        case 0x04: NOP<&CPUImpl::zp0>();        break;
        case 0x05: ORA<&CPUImpl::zp0>();        break;
        case 0x06: ASL<&CPUImpl::zp0>();        break;
        case 0x07: SLO<&CPUImpl::zp0>();        break;
        case 0x08: PHP();                       break;
        case 0x09: ORA<&CPUImpl::imm>();        break;
        case 0x0a: ASL();                       break;
        case 0x0b: ANC<&CPUImpl::imm>();        break;
        case 0x0c: NOP<&CPUImpl::abs>();        break;
        case 0x0d: ORA<&CPUImpl::abs>();        break;
        case 0x0e: ASL<&CPUImpl::abs>();        break;
        case 0x0f: SLO<&CPUImpl::abs>();        break;
        case 0x10: BPL();                       break;
        case 0x11: ORA<&CPUImpl::izy>();        break;
        case 0x12: STP();                       break;
        case 0x13: SLO<&CPUImpl::izy<false>>(); break;
        case 0x14: NOP<&CPUImpl::zpx>();        break;
        case 0x15: ORA<&CPUImpl::zpx>();        break;
        case 0x16: ASL<&CPUImpl::zpx>();        break;
        case 0x17: SLO<&CPUImpl::zpx>();        break;
        case 0x18: CLC();                       break;
        case 0x19: ORA<&CPUImpl::aby>();        break;
        case 0x1a: NOP();                       break;
        case 0x1b: SLO<&CPUImpl::aby<false>>(); break;
        case 0x1c: NOP<&CPUImpl::abx>();        break;
        case 0x1d: ORA<&CPUImpl::abx>();        break;
        case 0x1e: ASL<&CPUImpl::abx<false>>(); break;
        case 0x1f: SLO<&CPUImpl::abx<false>>(); break;
        case 0x20: JSR();                       break;
        case 0x21: AND<&CPUImpl::izx>();        break;
        case 0x22: STP();                       break;
        case 0x23: RLA<&CPUImpl::izx>();        break;
        case 0x24: BIT<&CPUImpl::zp0>();        break;
        case 0x25: AND<&CPUImpl::zp0>();        break;
        case 0x26: ROL<&CPUImpl::zp0>();        break;
        case 0x27: RLA<&CPUImpl::zp0>();        break;
        case 0x28: PLP();                       break;
        case 0x29: AND<&CPUImpl::imm>();        break;
        case 0x2a: ROL();                       break;
        case 0x2b: ANC<&CPUImpl::imm>();        break;
        case 0x2c: BIT<&CPUImpl::abs>();        break;
        case 0x2d: AND<&CPUImpl::abs>();        break;
        case 0x2e: ROL<&CPUImpl::abs>();        break;
        case 0x2f: RLA<&CPUImpl::abs>();        break;
        case 0x30: BMI();                       break;
        case 0x31: AND<&CPUImpl::izy>();        break;
        case 0x32: STP();                       break;
        case 0x33: RLA<&CPUImpl::izy<false>>(); break;
        case 0x34: NOP<&CPUImpl::zpx>();        break;
        case 0x35: AND<&CPUImpl::zpx>();        break;
        case 0x36: ROL<&CPUImpl::zpx>();        break;
        case 0x37: RLA<&CPUImpl::zpx>();        break;
        case 0x38: SEC();                       break;
        case 0x39: AND<&CPUImpl::aby>();        break;
        case 0x3a: NOP();                       break;
        case 0x3b: RLA<&CPUImpl::aby<false>>(); break;
        case 0x3c: NOP<&CPUImpl::abx>();        break;
        case 0x3d: AND<&CPUImpl::abx>();        break;
        case 0x3e: ROL<&CPUImpl::abx<false>>(); break;
        case 0x3f: RLA<&CPUImpl::abx<false>>(); break;
        case 0x40: RTI();                       break;
        case 0x41: EOR<&CPUImpl::izx>();        break;
        case 0x42: STP();                       break;
        case 0x43: SRE<&CPUImpl::izx>();        break;
        case 0x44: NOP<&CPUImpl::zp0>();        break;
        case 0x45: EOR<&CPUImpl::zp0>();        break;
        case 0x46: LSR<&CPUImpl::zp0>();        break;
        case 0x47: SRE<&CPUImpl::zp0>();        break;
        case 0x48: PHA();                       break;
        case 0x49: EOR<&CPUImpl::imm>();        break;
        case 0x4a: LSR();                       break;
        case 0x4b: ASR<&CPUImpl::imm>();        break;
        case 0x4c: JMP<&CPUImpl::abs<true>>();  break;
        case 0x4d: EOR<&CPUImpl::abs>();        break;
        case 0x4e: LSR<&CPUImpl::abs>();        break;
        case 0x4f: SRE<&CPUImpl::abs>();        break;
        case 0x50: BVC();                       break;
        case 0x51: EOR<&CPUImpl::izy>();        break;
        case 0x52: STP();                       break;
        case 0x53: SRE<&CPUImpl::izy<false>>(); break;
        case 0x54: NOP<&CPUImpl::zpx>();        break;
        case 0x55: EOR<&CPUImpl::zpx>();        break;
        case 0x56: LSR<&CPUImpl::zpx>();        break;
        case 0x57: SRE<&CPUImpl::zpx>();        break;
        case 0x58: CLI();                       break;
        case 0x59: EOR<&CPUImpl::aby>();        break;
        case 0x5a: NOP();                       break;
        case 0x5b: SRE<&CPUImpl::aby<false>>(); break;
        case 0x5c: NOP<&CPUImpl::abx>();        break;
        case 0x5d: EOR<&CPUImpl::abx>();        break;
        case 0x5e: LSR<&CPUImpl::abx<false>>(); break;
        case 0x5f: SRE<&CPUImpl::abx<false>>(); break;
        case 0x60: RTS();                       break;
        case 0x61: ADC<&CPUImpl::izx>();        break;
        case 0x62: STP();                       break;
        case 0x63: RRA<&CPUImpl::izx>();        break;
        case 0x64: NOP<&CPUImpl::zp0>();        break;
        case 0x65: ADC<&CPUImpl::zp0>();        break;
        case 0x66: ROR<&CPUImpl::zp0>();        break;
        case 0x67: RRA<&CPUImpl::zp0>();        break;
        case 0x68: PLA();                       break;
        case 0x69: ADC<&CPUImpl::imm>();        break;
        case 0x6a: ROR();                       break;
        case 0x6b: ARR<&CPUImpl::imm>();        break;
        case 0x6c: JMP<&CPUImpl::ind>();        break;
        case 0x6d: ADC<&CPUImpl::abs>();        break;
        case 0x6e: ROR<&CPUImpl::abs>();        break;
        case 0x6f: RRA<&CPUImpl::abs>();        break;
        case 0x70: BVS();                       break;
        case 0x71: ADC<&CPUImpl::izy>();        break;
        case 0x72: STP();                       break;
        case 0x73: RRA<&CPUImpl::izy<false>>(); break;
        case 0x74: NOP<&CPUImpl::zpx>();        break;
        case 0x75: ADC<&CPUImpl::zpx>();        break;
        case 0x76: ROR<&CPUImpl::zpx>();        break;
        case 0x77: RRA<&CPUImpl::zpx>();        break;
        case 0x78: SEI();                       break;
        case 0x79: ADC<&CPUImpl::aby>();        break;
        case 0x7a: NOP();                       break;
        case 0x7b: RRA<&CPUImpl::aby<false>>(); break;
        case 0x7c: NOP<&CPUImpl::abx>();        break;
        case 0x7d: ADC<&CPUImpl::abx>();        break;
        case 0x7e: ROR<&CPUImpl::abx<false>>(); break;
        case 0x7f: RRA<&CPUImpl::abx<false>>(); break;
        case 0x80: NOP<&CPUImpl::imm>();        break;
        case 0x81: STA<&CPUImpl::izx>();        break;
        case 0x82: NOP<&CPUImpl::imm>();        break;
        case 0x83: SAX<&CPUImpl::izx>();        break;
        case 0x84: STY<&CPUImpl::zp0>();        break;
        case 0x85: STA<&CPUImpl::zp0>();        break;
        case 0x86: STX<&CPUImpl::zp0>();        break;
        case 0x87: SAX<&CPUImpl::zp0>();        break;
        case 0x88: DEY();                       break;
        case 0x89: NOP<&CPUImpl::imm>();        break;
        case 0x8a: TXA();                       break;
        case 0x8b: ANE<&CPUImpl::imm>();        break;
        case 0x8c: STY<&CPUImpl::abs>();        break;
        case 0x8d: STA<&CPUImpl::abs>();        break;
        case 0x8e: STX<&CPUImpl::abs>();        break;
        case 0x8f: SAX<&CPUImpl::abs>();        break;
        case 0x90: BCC();                       break;
        case 0x91: STA<&CPUImpl::izy<false>>(); break;
        case 0x92: STP();                       break;
        case 0x93: SHA<&CPUImpl::izy<false>>(); break;
        case 0x94: STY<&CPUImpl::zpx>();        break;
        case 0x95: STA<&CPUImpl::zpx>();        break;
        case 0x96: STX<&CPUImpl::zpy>();        break;
        case 0x97: SAX<&CPUImpl::zpy>();        break;
        case 0x98: TYA();                       break;
        case 0x99: STA<&CPUImpl::aby<false>>(); break;
        case 0x9a: TXS();                       break;
        case 0x9b: SHS<&CPUImpl::aby<false>>(); break;
        case 0x9c: SHY<&CPUImpl::abx<false>>(); break;
        case 0x9d: STA<&CPUImpl::abx<false>>(); break;
        case 0x9e: SHX<&CPUImpl::aby<false>>(); break;
        case 0x9f: SHA<&CPUImpl::aby<false>>(); break;
        case 0xa0: LDY<&CPUImpl::imm>();        break;
        case 0xa1: LDA<&CPUImpl::izx>();        break;
        case 0xa2: LDX<&CPUImpl::imm>();        break;
        case 0xa3: LAX<&CPUImpl::izx>();        break;
        case 0xa4: LDY<&CPUImpl::zp0>();        break;
        case 0xa5: LDA<&CPUImpl::zp0>();        break;
        case 0xa6: LDX<&CPUImpl::zp0>();        break;
        case 0xa7: LAX<&CPUImpl::zp0>();        break;
        case 0xa8: TAY();                       break;
        case 0xa9: LDA<&CPUImpl::imm>();        break;
        case 0xaa: TAX();                       break;
        case 0xab: LXA<&CPUImpl::imm>();        break;
        case 0xac: LDY<&CPUImpl::abs>();        break;
        case 0xad: LDA<&CPUImpl::abs>();        break;
        case 0xae: LDX<&CPUImpl::abs>();        break;
        case 0xaf: LAX<&CPUImpl::abs>();        break;
        case 0xb0: BCS();                       break;
        case 0xb1: LDA<&CPUImpl::izy>();        break;
        case 0xb2: STP();                       break;
        case 0xb3: LAX<&CPUImpl::izy>();        break;
        case 0xb4: LDY<&CPUImpl::zpx>();        break;
        case 0xb5: LDA<&CPUImpl::zpx>();        break;
        case 0xb6: LDX<&CPUImpl::zpy>();        break;
        case 0xb7: LAX<&CPUImpl::zpy>();        break;
        case 0xb8: CLV();                       break;
        case 0xb9: LDA<&CPUImpl::aby>();        break;
        case 0xba: TSX();                       break;
        case 0xbb: LAS<&CPUImpl::aby>();        break;
        case 0xbc: LDY<&CPUImpl::abx>();        break;
        case 0xbd: LDA<&CPUImpl::abx>();        break;
        case 0xbe: LDX<&CPUImpl::aby>();        break;
        case 0xbf: LAX<&CPUImpl::aby>();        break;
        case 0xc0: CPY<&CPUImpl::imm>();        break;
        case 0xc1: CMP<&CPUImpl::izx>();        break;
        case 0xc2: NOP<&CPUImpl::imm>();        break;
        case 0xc3: DCP<&CPUImpl::izx>();        break;
        case 0xc4: CPY<&CPUImpl::zp0>();        break;
        case 0xc5: CMP<&CPUImpl::zp0>();        break;
        case 0xc6: DEC<&CPUImpl::zp0>();        break;
        case 0xc7: DCP<&CPUImpl::zp0>();        break;
        case 0xc8: INY();                       break;
        case 0xc9: CMP<&CPUImpl::imm>();        break;
        case 0xca: DEX();                       break;
        case 0xcb: SBX<&CPUImpl::imm>();        break;
        case 0xcc: CPY<&CPUImpl::abs>();        break;
        case 0xcd: CMP<&CPUImpl::abs>();        break;
        case 0xce: DEC<&CPUImpl::abs>();        break;
        case 0xcf: DCP<&CPUImpl::abs>();        break;
        case 0xd0: BNE();                       break;
        case 0xd1: CMP<&CPUImpl::izy>();        break;
        case 0xd2: STP();                       break;
        case 0xd3: DCP<&CPUImpl::izy<false>>(); break;
        case 0xd4: NOP<&CPUImpl::zpx>();        break;
        case 0xd5: CMP<&CPUImpl::zpx>();        break;
        case 0xd6: DEC<&CPUImpl::zpx>();        break;
        case 0xd7: DCP<&CPUImpl::zpx>();        break;
        case 0xd8: CLD();                       break;
        case 0xd9: CMP<&CPUImpl::aby>();        break;
        case 0xda: NOP();                       break;
        case 0xdb: DCP<&CPUImpl::aby<false>>(); break;
        case 0xdc: NOP<&CPUImpl::abx>();        break;
        case 0xdd: CMP<&CPUImpl::abx>();        break;
        case 0xde: DEC<&CPUImpl::abx<false>>(); break;
        case 0xdf: DCP<&CPUImpl::abx<false>>(); break;
        case 0xe0: CPX<&CPUImpl::imm>();        break;
        case 0xe1: SBC<&CPUImpl::izx>();        break;
        case 0xe2: NOP<&CPUImpl::imm>();        break;
        case 0xe3: ISB<&CPUImpl::izx>();        break;
        case 0xe4: CPX<&CPUImpl::zp0>();        break;
        case 0xe5: SBC<&CPUImpl::zp0>();        break;
        case 0xe6: INC<&CPUImpl::zp0>();        break;
        case 0xe7: ISB<&CPUImpl::zp0>();        break;
        case 0xe8: INX();                       break;
        case 0xe9: SBC<&CPUImpl::imm>();        break;
        case 0xea: NOP();                       break;
        case 0xeb: SBC<&CPUImpl::imm>();        break;
        case 0xec: CPX<&CPUImpl::abs>();        break;
        case 0xed: SBC<&CPUImpl::abs>();        break;
        case 0xee: INC<&CPUImpl::abs>();        break;
        case 0xef: ISB<&CPUImpl::abs>();        break;
        case 0xf0: BEQ();                       break;
        case 0xf1: SBC<&CPUImpl::izy>();        break;
        case 0xf2: STP();                       break;
        case 0xf3: ISB<&CPUImpl::izy<false>>(); break;
        case 0xf4: NOP<&CPUImpl::zpx>();        break;
        case 0xf5: SBC<&CPUImpl::zpx>();        break;
        case 0xf6: INC<&CPUImpl::zpx>();        break;
        case 0xf7: ISB<&CPUImpl::zpx>();        break;
        case 0xf8: SED();                       break;
        case 0xf9: SBC<&CPUImpl::aby>();        break;
        case 0xfa: NOP();                       break;
        case 0xfb: ISB<&CPUImpl::aby<false>>(); break;
        case 0xfc: NOP<&CPUImpl::abx>();        break;
        case 0xfd: SBC<&CPUImpl::abx>();        break;
        case 0xfe: INC<&CPUImpl::abx<false>>(); break;
        case 0xff: ISB<&CPUImpl::abx<false>>(); break;
        }
    }

    inline void CPUImpl::dma(const std::uint16_t dst, const std::uint16_t src, const std::uint16_t size) noexcept
    {
        i.dmaState = CPU::State::DMA_STATE_ENABLE;
        if (clock->getCPUCycles() & 1) T;
        T;

        for (std::uint16_t count = 0; count < size - 2; count++) write(dst, read(src + count));

        write(dst, (i.dmaState = CPU::State::DMA_STATE_SECOND_LAST_TICK, read(src + size - 2)));
        i.dmaState = CPU::State::DMA_STATE_ENABLE;

        write(dst, (i.dmaState = CPU::State::DMA_STATE_LAST_TICK, read(src + size - 1)));
        i.dmaState = CPU::State::DMA_STATE_DISABLE;
    }
    inline void CPUImpl::reset() noexcept
    {
        T; T;
        T; sp--;
        T; sp--;
        T; sp--;
        pc = read(RESET_VECTOR);
        p.i = 1;
        pc |= static_cast<std::uint16_t>(read(RESET_VECTOR + 1)) << 8;
    }
    inline void CPUImpl::requestNMI(const bool status) noexcept
    {
        i.requestNMI = status;
    }
    inline void CPUImpl::requestIRQ(const bool status) noexcept
    {
        i.requestIRQ = status;
    }
    template<> inline unsigned int CPUImpl::get<CPU::State::Type::TickState>() const noexcept
    {
        return i.tickState;
    }
    template<> inline unsigned int CPUImpl::get<CPU::State::Type::DMAState>() const noexcept
    {
        return i.dmaState;
    }
}

struct fcpp::core::CPU::CPUData
{
    detail::CPUImpl impl{};
    struct IRQStatus
    {
        bool apu : 1;
        bool dmc : 1;
        bool mapper : 1;

        operator bool() const noexcept
        {
            return apu || dmc || mapper;
        }
    } irqStatus{};
};

fcpp::core::CPU::CPU() : dptr(std::make_unique<CPUData>()) {}
fcpp::core::CPU::~CPU() noexcept = default;

void fcpp::core::CPU::connect(void* const p) noexcept
{
    auto fptr = static_cast<FC*>(p);
    dptr->impl.connect(fptr->getBus(), fptr->getClock());
}
void fcpp::core::CPU::save(void* const p) noexcept
{
    dptr->impl.access(static_cast<Snapshot*>(p)->getWriter());
}
void fcpp::core::CPU::load(void* const p) noexcept
{
    dptr->impl.access(static_cast<Snapshot*>(p)->getReader());
}
void fcpp::core::CPU::reset() noexcept
{
    dptr->impl.clear();
}

void fcpp::core::CPU::requestDMA(const std::uint16_t dst, const std::uint8_t page) noexcept
{
    std::uint16_t src = static_cast<std::uint16_t>(page) << 8;
    dptr->impl.dma(dst, src, 256);
}
void fcpp::core::CPU::requestRESET() noexcept
{
    dptr->impl.reset();
}
void fcpp::core::CPU::requestNMI(const bool status) noexcept
{
    dptr->impl.requestNMI(status);
}
template<> void fcpp::core::CPU::requestIRQ<fcpp::core::CPU::IRQType::APU>(const bool status) noexcept
{
    dptr->irqStatus.apu = status;
    dptr->impl.requestIRQ(dptr->irqStatus);
}
template<> void fcpp::core::CPU::requestIRQ<fcpp::core::CPU::IRQType::DMC>(const bool status) noexcept
{
    dptr->irqStatus.dmc = status;
    dptr->impl.requestIRQ(dptr->irqStatus);
}
template<> void fcpp::core::CPU::requestIRQ<fcpp::core::CPU::IRQType::Mapper>(const bool status) noexcept
{
    dptr->irqStatus.mapper = status;
    dptr->impl.requestIRQ(dptr->irqStatus);
}

void fcpp::core::CPU::exec() noexcept
{
    dptr->impl.exec();
}

template<fcpp::core::CPU::State::Type type> unsigned int fcpp::core::CPU::get() const noexcept
{
    return dptr->impl.get<type>();
}

template unsigned int fcpp::core::CPU::get<fcpp::core::CPU::State::Type::TickState>() const noexcept;
template unsigned int fcpp::core::CPU::get<fcpp::core::CPU::State::Type::DMAState>() const noexcept;
