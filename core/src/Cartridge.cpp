#include <utility>
#include <vector>

#include "FCPP/Core/Cartridge.hpp"
#include "FCPP/Core/FC.hpp"

namespace fcpp::core::detail
{
    class Mapper
    {
    public:
        Mapper(INES* content, FC* fc);
        virtual ~Mapper() = default;

        virtual std::uint8_t readPRG(std::uint16_t addr) noexcept = 0;
        virtual void writePRG(std::uint16_t addr, std::uint8_t data) noexcept = 0;

        virtual std::uint8_t readCHR(std::uint16_t addr) noexcept = 0;
        virtual void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept = 0;

        virtual MirrorType getMirrorType() noexcept;
        virtual void sync() noexcept;

        virtual void save(Snapshot::Writer& writer) noexcept;
        virtual void load(Snapshot::Reader& reader) noexcept;
    protected:
        INES* content = nullptr;
        FC* fc = nullptr;
    };
    Mapper::Mapper(INES* const content, FC* const fc) : content(content), fc(fc) {}
    MirrorType Mapper::getMirrorType() noexcept
    {
        return content->getMirrorType();
    }
    void Mapper::sync() noexcept
    {
        return;
    }
    void Mapper::save(Snapshot::Writer& writer) noexcept
    {
        if (!content->getCHRBanks()) writer.access(content->getCHRData(), content->getCHRSize());
        return;
    }
    void Mapper::load(Snapshot::Reader& reader) noexcept
    {
        if (!content->getCHRBanks()) reader.access(content->getCHRData(), content->getCHRSize());
        return;
    }

    class Mapper0 : public Mapper
    {
    public:
        Mapper0(INES* content);
        ~Mapper0() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;
    private:
        std::uint8_t prgRam[0x2000]{};
    };
    Mapper0::Mapper0(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper0::readPRG(const std::uint16_t addr) noexcept
    {
        if (addr < 0x8000) return prgRam[addr & 0x1fff];

        return
            (content->getPRGBanks() == 1) ?
            content->readPRG(addr & 0x3fff) :
            content->readPRG(addr & 0x7fff);
    }
    void Mapper0::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr < 0x8000) prgRam[addr & 0x1fff] = data;
    }
    std::uint8_t Mapper0::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR(addr);
    }
    void Mapper0::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (content->getCHRBanks() == 0) content->writeCHR(addr, data);
    }

    class Mapper1 : public Mapper
    {
    public:
        Mapper1(INES* content);
        ~Mapper1() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint32_t getCHRBankAddr(std::uint16_t addr) const noexcept;
        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        MirrorType getMirrorType() noexcept override;

        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    private:
        std::uint8_t shiftRegister = 0;
        std::uint8_t counter = 0;
        std::uint8_t control = 0x0c;
        std::uint8_t chrBank0 = 0, chrBank1 = 0, prgBank = 0;
        std::uint8_t prgRam[0x2000]{};
    };
    Mapper1::Mapper1(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper1::readPRG(const std::uint16_t addr) noexcept
    {
        if (addr < 0x8000) return prgRam[addr & 0x1fff];

        std::uint32_t bank = 0;
        switch ((control >> 2) & 3)
        {
        case 0:
        case 1:
            bank = ((prgBank & 0x0f) >> 1) * 0x8000 + (addr & 0x7fff);
            break;
        case 2:
            bank = ((addr < 0xc000) ? 0 : (prgBank & 0x0f)) * 0x4000 + (addr & 0x3fff);
            break;
        case 3:
            bank = ((addr < 0xc000) ? (prgBank & 0x0f) : (content->getPRGBanks() - 1)) * 0x4000 + (addr & 0x3fff);
            break;
        }

        return content->readPRG(bank);
    }
    void Mapper1::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr < 0x8000) prgRam[addr & 0x1fff] = data;
        else if (data & 0x80)
        {
            shiftRegister = 0;
            counter = 0;
            control |= 0x0c;
        }
        else
        {
            shiftRegister = ((data & 1) << 4) | (shiftRegister >> 1);
            if (++counter == 5)
            {
                switch ((addr >> 13) & 0x03)
                {
                case 0:
                    control = shiftRegister & 0x1f;
                    break;
                case 1:
                    chrBank0 = shiftRegister & 0x1f;
                    break;
                case 2:
                    chrBank1 = shiftRegister & 0x1f;
                    break;
                case 3:
                    prgBank = shiftRegister & 0x1f;
                    break;
                }
                shiftRegister = 0;
                counter = 0;
            }
        }
    }
    inline std::uint32_t Mapper1::getCHRBankAddr(const std::uint16_t addr) const noexcept
    {
        std::uint32_t bank = 0;
        if (control & (1 << 4))
        {
            if (addr < 0x1000) bank = chrBank0 * 0x1000 + (addr & 0x0fff);
            else bank = chrBank1 * 0x1000 + (addr & 0x0fff);
        }
        else bank = (chrBank0 >> 1) * 0x2000 + (addr & 0x1fff);
        return bank;
    }
    std::uint8_t Mapper1::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR(getCHRBankAddr(addr));
    }
    void Mapper1::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (content->getCHRBanks() == 0) content->writeCHR(getCHRBankAddr(addr), data);
    }
    MirrorType Mapper1::getMirrorType() noexcept
    {
        switch (control & 3)
        {
        case 0:
            return MirrorType::SINGLE_SCREEN_LOWER_BANK;
        case 1:
            return MirrorType::SINGLE_SCREEN_UPPER_BANK;
        case 2:
            return MirrorType::VERTICAL;
        case 3:
            return MirrorType::HORIZONTAL;
        }
        return MirrorType::VERTICAL;
    }
    template<typename Accessor>
    inline void Mapper1::access(Accessor& accessor) noexcept
    {
        accessor.access(shiftRegister);
        accessor.access(counter);
        accessor.access(control);
        accessor.access(chrBank0);
        accessor.access(chrBank1);
        accessor.access(prgBank);
    }
    void Mapper1::save(Snapshot::Writer& writer) noexcept
    {
        access(writer);
        Mapper::save(writer);
    }
    void Mapper1::load(Snapshot::Reader& reader) noexcept
    {
        access(reader);
        Mapper::load(reader);
    }

    class Mapper2 : public Mapper
    {
    public:
        Mapper2(INES* content);
        ~Mapper2() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    protected:
        std::uint8_t bankSelect = 0;
    };
    Mapper2::Mapper2(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper2::readPRG(const std::uint16_t addr) noexcept
    {
        std::uint32_t bank = 0;
        if (addr < 0xc000) bank = bankSelect * 0x4000 + (addr & 0x3fff);
        else bank = (content->getPRGBanks() - 1) * 0x4000 + (addr & 0x3fff);

        return content->readPRG(bank);
    }
    void Mapper2::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = data & 0x0f;
    }
    std::uint8_t Mapper2::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR(addr);
    }
    void Mapper2::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        content->writeCHR(addr, data);
    }
    void Mapper2::save(Snapshot::Writer& writer) noexcept
    {
        writer.access(bankSelect);
        Mapper::save(writer);
    }
    void Mapper2::load(Snapshot::Reader& reader) noexcept
    {
        reader.access(bankSelect);
        Mapper::load(reader);
    }

    class Mapper3 : public Mapper
    {
    public:
        Mapper3(INES* content);
        ~Mapper3() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    private:
        std::uint8_t bankSelect = 0;
    };
    Mapper3::Mapper3(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper3::readPRG(const std::uint16_t addr) noexcept
    {
        return
            (content->getPRGBanks() == 1) ?
            content->readPRG(addr & 0x3fff) :
            content->readPRG(addr & 0x7fff);
    }
    void Mapper3::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = data & 0x03;
    }
    std::uint8_t Mapper3::readCHR(const std::uint16_t addr) noexcept
    {
        std::uint32_t bank = bankSelect * 0x2000 + (addr & 0x1fff);
        return content->readCHR(bank);
    }
    void Mapper3::writeCHR(const std::uint16_t /* addr */, const std::uint8_t /* data */) noexcept { /*not allowed*/ }
    void Mapper3::save(Snapshot::Writer& writer) noexcept
    {
        writer.access(bankSelect);
        Mapper::save(writer);
    }
    void Mapper3::load(Snapshot::Reader& reader) noexcept
    {
        reader.access(bankSelect);
        Mapper::load(reader);
    }

    class Mapper4 : public Mapper
    {
    public:
        using Mapper::Mapper;
        ~Mapper4() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint32_t getCHRBankAddr(std::uint16_t addr) const noexcept;
        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        MirrorType getMirrorType() noexcept override;
        void sync() noexcept override;

        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    private:
        std::uint64_t ppuA12HighCycle = 0;
        MirrorType mirrorType = MirrorType::VERTICAL;
        std::uint8_t bankSelect = 0;
        std::uint8_t period = 0;
        std::uint8_t counter = 0;
        bool irqEnabled = false;
        std::uint8_t bankRegister[8]{};
        std::uint8_t prgRam[0x2000]{};
    };
    std::uint8_t Mapper4::readPRG(const std::uint16_t addr) noexcept
    {
        if (addr < 0x8000) return prgRam[addr & 0x1fff];

        std::uint32_t bank = 0;
        if (addr < 0xa000) bank = (((bankSelect >> 6) & 1) ? (content->getPRGBanks() * 2 - 2) : bankRegister[6]) * 0x2000 + (addr & 0x1fff);
        else if (addr < 0xc000) bank = bankRegister[7] * 0x2000 + (addr & 0x1fff);
        else if (addr < 0xe000) bank = (((bankSelect >> 6) & 1) ? bankRegister[6] : (content->getPRGBanks() * 2 - 2)) * 0x2000 + (addr & 0x1fff);
        else bank = (content->getPRGBanks() * 2 - 1) * 0x2000 + (addr & 0x1fff);

        return content->readPRG(bank);
    }
    void Mapper4::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr < 0x8000) prgRam[addr & 0x1fff] = data;
        else switch (((addr >> 12) & 6) | (addr & 1))
        {
        case 0: // 0x8000
            bankSelect = data;
            break;
        case 1: // 0x8001
            bankRegister[bankSelect & 0x07] = data;
            break;
        case 2: // 0xa000
            mirrorType = (data & 1) ? MirrorType::HORIZONTAL : MirrorType::VERTICAL;
            break;
        case 4: // 0xc000
            period = data;
            break;
        case 5: // 0xc001
            counter = 0;
            break;
        case 6: // 0xe000
            fc->getCPU()->requestIRQ<CPU::IRQType::Mapper>(irqEnabled = false);
            break;
        case 7: // 0xe001
            irqEnabled = true;
            break;
        }
    }
    inline std::uint32_t Mapper4::getCHRBankAddr(const std::uint16_t addr) const noexcept
    {
        std::uint32_t bank = 0;
        if ((bankSelect >> 7) & 1)
        { //R0 and R1 ignore the bottom bit, as the value written still counts banks in 1KB units but odd numbered banks can't be selected.
            if (addr < 0x0400) bank = bankRegister[2] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x0800) bank = bankRegister[3] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x0c00) bank = bankRegister[4] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x1000) bank = bankRegister[5] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x1800) bank = (bankRegister[0] >> 1) * 0x0800 + (addr & 0x07ff);
            else bank = (bankRegister[1] >> 1) * 0x0800 + (addr & 0x07ff);
        }
        else
        {
            if (addr < 0x0800) bank = (bankRegister[0] >> 1) * 0x0800 + (addr & 0x07ff);
            else if (addr < 0x1000) bank = (bankRegister[1] >> 1) * 0x0800 + (addr & 0x07ff);
            else if (addr < 0x1400) bank = bankRegister[2] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x1800) bank = bankRegister[3] * 0x0400 + (addr & 0x03ff);
            else if (addr < 0x1c00) bank = bankRegister[4] * 0x0400 + (addr & 0x03ff);
            else bank = bankRegister[5] * 0x0400 + (addr & 0x03ff);
        }
        return bank;
    }
    std::uint8_t Mapper4::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR(getCHRBankAddr(addr));
    }
    void Mapper4::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (content->getCHRBanks() == 0) content->writeCHR(getCHRBankAddr(addr), data);
    }
    MirrorType Mapper4::getMirrorType() noexcept
    {
        return content->getMirrorType() == MirrorType::FOUR_SCREEN ? MirrorType::FOUR_SCREEN : mirrorType;
    }
    void Mapper4::sync() noexcept
    {
        if (fc->getPPU()->get<PPU::State::Type::AddressBus>() & (1 << 12)) //A12
        {
            std::uint64_t ppuCycles = fc->getClock()->getPPUCycles();
            if (ppuCycles - ppuA12HighCycle > 16)
            {
                //if zero or the reload flag is true, it's reloaded with the IRQ latched value at $C000; otherwise, it decrements.
                if (counter == 0) counter = period;
                else counter--;
                //checks the IRQ counter transition 1 to 0, whether from decrementing or reloading.
                if (counter == 0 && irqEnabled) fc->getCPU()->requestIRQ<CPU::IRQType::Mapper>(true);
            }
            ppuA12HighCycle = ppuCycles;
        }
    }
    template<typename Accessor>
    inline void Mapper4::access(Accessor& accessor) noexcept
    {
        accessor.access(ppuA12HighCycle);
        accessor.access(mirrorType);
        accessor.access(bankSelect);
        accessor.access(period);
        accessor.access(counter);
        accessor.access(irqEnabled);
        accessor.access(bankRegister, sizeof(bankRegister));
        accessor.access(prgRam, sizeof(prgRam));
    }
    void Mapper4::save(Snapshot::Writer& writer) noexcept
    {
        access(writer);
    }
    void Mapper4::load(Snapshot::Reader& reader) noexcept
    {
        access(reader);
    }

    class Mapper7 : public Mapper
    {
    public:
        Mapper7(INES* content);
        ~Mapper7() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        MirrorType getMirrorType() noexcept override;

        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    private:
        std::uint8_t bankSelect = 0;
    };
    Mapper7::Mapper7(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper7::readPRG(const std::uint16_t addr) noexcept
    {
        return content->readPRG((bankSelect & 0x07) * 0x8000 + (addr & 0x7fff));
    }
    void Mapper7::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = data & 0x17;
    }
    std::uint8_t Mapper7::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR(addr);
    }
    void Mapper7::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        content->writeCHR(addr, data);
    }
    MirrorType fcpp::core::detail::Mapper7::getMirrorType() noexcept
    {
        return (bankSelect & 0x10) ? MirrorType::SINGLE_SCREEN_UPPER_BANK : MirrorType::SINGLE_SCREEN_LOWER_BANK;
    }
    void Mapper7::save(Snapshot::Writer& writer) noexcept
    {
        writer.access(bankSelect);
        Mapper::save(writer);
    }
    void Mapper7::load(Snapshot::Reader& reader) noexcept
    {
        reader.access(bankSelect);
        Mapper::load(reader);
    }

    class Mapper9 : public Mapper
    {
    public:
        using Mapper::Mapper;
        ~Mapper9() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        MirrorType getMirrorType() noexcept override;
        void sync() noexcept override;

        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    protected:
        MirrorType mirrorType = MirrorType::VERTICAL;
        std::uint8_t prgBankSelect = 0;
        std::uint16_t ppuReadAddr = 0;
        std::uint8_t latch[2]{};
        std::uint8_t chrBankSelect0[2]{};
        std::uint8_t chrBankSelect1[2]{};
        std::uint8_t prgRam[0x2000]{};
    };
    std::uint8_t Mapper9::readPRG(const std::uint16_t addr) noexcept
    {
        if (addr < 0x8000) return prgRam[addr & 0x1fff];

        std::uint32_t bank = 0;
        if (addr < 0xa000) bank = prgBankSelect * 0x2000 + (addr & 0x1fff);
        else if (addr < 0xc000) bank = (content->getPRGBanks() * 2 - 3) * 0x2000 + (addr & 0x1fff);
        else if (addr < 0xe000) bank = (content->getPRGBanks() * 2 - 2) * 0x2000 + (addr & 0x1fff);
        else bank = (content->getPRGBanks() * 2 - 1) * 0x2000 + (addr & 0x1fff);

        return content->readPRG(bank);
    }
    void Mapper9::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr < 0x8000) prgRam[addr & 0x1fff] = data;
        else switch ((addr >> 12) & 7)
        {
        case 2:
            prgBankSelect = data & 0x0f;
            break;
        case 3:
            chrBankSelect0[0] = data & 0x1f;
            break;
        case 4:
            chrBankSelect0[1] = data & 0x1f;
            break;
        case 5:
            chrBankSelect1[0] = data & 0x1f;
            break;
        case 6:
            chrBankSelect1[1] = data & 0x1f;
            break;
        case 7:
            mirrorType = (data & 1) ? MirrorType::HORIZONTAL : MirrorType::VERTICAL;
            break;
        }
    }
    std::uint8_t Mapper9::readCHR(const std::uint16_t addr) noexcept
    {
        std::uint32_t bank = 0;
        if (addr < 0x1000) bank = chrBankSelect0[latch[0]] * 0x1000 + (addr & 0x0fff);
        else bank = chrBankSelect1[latch[1]] * 0x1000 + (addr & 0x0fff);

        return content->readCHR(bank);
    }
    void Mapper9::writeCHR(const std::uint16_t /* addr */, const std::uint8_t /* data */) noexcept { /*not allowed*/ }
    MirrorType Mapper9::getMirrorType() noexcept
    {
        return mirrorType;
    }
    void Mapper9::sync() noexcept
    {
        auto ppuAddr = static_cast<std::uint16_t>(fc->getPPU()->get<PPU::State::Type::AddressBus>());
        if ((ppuAddr != 0x0fd8) && (ppuAddr != 0x0fe8) && (ppuAddr < 0x1fd8 || ppuAddr > 0x1fdf) && (ppuAddr < 0x1fe8 || ppuAddr > 0x1fef))
        {
            if (ppuReadAddr == 0x0fd8) latch[0] = 0;
            else if (ppuReadAddr == 0x0fe8) latch[0] = 1;
            else if (ppuReadAddr >= 0x1fd8 && ppuReadAddr <= 0x1fdf) latch[1] = 0;
            else if (ppuReadAddr >= 0x1fe8 && ppuReadAddr <= 0x1fef) latch[1] = 1;
        }
        ppuReadAddr = ppuAddr;
    }
    template<typename Accessor>
    inline void Mapper9::access(Accessor& accessor) noexcept
    {
        accessor.access(mirrorType);
        accessor.access(prgBankSelect);
        accessor.access(ppuReadAddr);
        accessor.access(latch, sizeof(latch));
        accessor.access(chrBankSelect0, sizeof(chrBankSelect0));
        accessor.access(chrBankSelect1, sizeof(chrBankSelect1));
        accessor.access(prgRam, sizeof(prgRam));
    }
    void Mapper9::save(Snapshot::Writer& writer) noexcept
    {
        access(writer);
    }
    void Mapper9::load(Snapshot::Reader& reader) noexcept
    {
        access(reader);
    }

    class Mapper10 : public Mapper9
    {
    public:
        using Mapper9::Mapper9;
        ~Mapper10() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void sync() noexcept override;
    };
    std::uint8_t Mapper10::readPRG(std::uint16_t addr) noexcept
    {
        if (addr < 0x8000) return prgRam[addr & 0x1fff];

        std::uint32_t bank = 0;
        if (addr < 0xc000) bank = prgBankSelect * 0x4000 + (addr & 0x3fff);
        else bank = (content->getPRGBanks() - 1) * 0x4000 + (addr & 0x3fff);

        return content->readPRG(bank);
    }
    void Mapper10::sync() noexcept
    {
        auto ppuAddr = static_cast<std::uint16_t>(fc->getPPU()->get<PPU::State::Type::AddressBus>());
        if ((ppuAddr < 0x0fd8 || ppuAddr > 0x0fdf) && (ppuAddr < 0x0fe8 || ppuAddr > 0x0fef) &&
            (ppuAddr < 0x1fd8 || ppuAddr > 0x1fdf) && (ppuAddr < 0x1fe8 || ppuAddr > 0x1fef))
        {
            if (ppuReadAddr >= 0x0fd8 && ppuReadAddr <= 0x0fdf) latch[0] = 0;
            else if (ppuReadAddr >= 0x0fe8 && ppuReadAddr <= 0x0fef) latch[0] = 1;
            else if (ppuReadAddr >= 0x1fd8 && ppuReadAddr <= 0x1fdf) latch[1] = 0;
            else if (ppuReadAddr >= 0x1fe8 && ppuReadAddr <= 0x1fef) latch[1] = 1;
        }
        ppuReadAddr = ppuAddr;
    }

    class Mapper11 : public Mapper
    {
    public:
        Mapper11(INES* content);
        ~Mapper11() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    protected:
        std::uint8_t bankSelect = 0;
    };
    Mapper11::Mapper11(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper11::readPRG(const std::uint16_t addr) noexcept
    {
        return content->readPRG((bankSelect & 0x03) * 0x8000 + (addr & 0x7fff));
    }
    void Mapper11::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = data;
    }
    std::uint8_t Mapper11::readCHR(const std::uint16_t addr) noexcept
    {
        return content->readCHR((bankSelect >> 4) * 0x2000 + (addr & 0x1fff));
    }
    void Mapper11::writeCHR(const std::uint16_t /* addr */, const std::uint8_t /* data */) noexcept { /*not allowed*/ }
    void Mapper11::save(Snapshot::Writer& writer) noexcept
    {
        writer.access(bankSelect);
        Mapper::save(writer);
    }
    void Mapper11::load(Snapshot::Reader& reader) noexcept
    {
        reader.access(bankSelect);
        Mapper::load(reader);
    }

    class Mapper13 : public Mapper
    {
    public:
        Mapper13(INES* content);
        ~Mapper13() override = default;

        std::uint8_t readPRG(std::uint16_t addr) noexcept override;
        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;

        std::uint32_t getCHRBankAddr(std::uint16_t addr) const noexcept;
        std::uint8_t readCHR(std::uint16_t addr) noexcept override;
        void writeCHR(std::uint16_t addr, std::uint8_t data) noexcept override;

        void save(Snapshot::Writer& writer) noexcept override;
        void load(Snapshot::Reader& reader) noexcept override;
    protected:
        std::uint8_t bankSelect = 0;
        std::uint8_t chrRam[0x2000]{};
    };
    Mapper13::Mapper13(INES* const content) : Mapper(content, nullptr) {}
    std::uint8_t Mapper13::readPRG(const std::uint16_t addr) noexcept
    {
        return content->readPRG(addr & 0x7fff);
    }
    void Mapper13::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = data & 0x03;
    }
    inline std::uint32_t Mapper13::getCHRBankAddr(const std::uint16_t addr) const noexcept
    {
        std::uint32_t bank = 0;
        if (addr < 0x1000) bank = addr & 0x0fff;
        else bank = bankSelect * 0x1000 + (addr & 0x0fff);
        return bank;
    }
    std::uint8_t Mapper13::readCHR(const std::uint16_t addr) noexcept
    {
        std::uint32_t bank = getCHRBankAddr(addr);
        return bank >= 0x2000 ? chrRam[bank & 0x1fff] : content->readCHR(bank);
    }
    void Mapper13::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        std::uint32_t bank = getCHRBankAddr(addr);
        if (bank >= 0x2000) chrRam[bank & 0x1fff] = data;
        else content->writeCHR(bank, data);
    }
    void Mapper13::save(Snapshot::Writer& writer) noexcept
    {
        writer.access(bankSelect);
        Mapper::save(writer);
    }
    void Mapper13::load(Snapshot::Reader& reader) noexcept
    {
        reader.access(bankSelect);
        Mapper::load(reader);
    }

    class Mapper94 : public Mapper2
    {
    public:
        using Mapper2::Mapper2;
        ~Mapper94() override = default;

        void writePRG(std::uint16_t addr, std::uint8_t data) noexcept override;
    };
    void Mapper94::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        if (addr & 0x8000) bankSelect = (data >> 2) & 0x07;
    }

    static std::unique_ptr<Mapper> createMapper(INES* const content, FC* const fc)
    {
        switch (content->getMapperType())
        {
        case 0:
            return std::make_unique<Mapper0>(content);
        case 1:
            return std::make_unique<Mapper1>(content);
        case 2: case 180:
            return std::make_unique<Mapper2>(content);
        case 3:
            return std::make_unique<Mapper3>(content);
        case 4:
            return std::make_unique<Mapper4>(content, fc);
        case 7:
            return std::make_unique<Mapper7>(content);
        case 9:
            return std::make_unique<Mapper9>(content, fc);
        case 10:
            return std::make_unique<Mapper10>(content, fc);
        case 11:
            return std::make_unique<Mapper11>(content);
        case 13:
            return std::make_unique<Mapper13>(content);
        case 94:
            return std::make_unique<Mapper94>(content);
        default:
            return nullptr;
        }
    }
}

bool fcpp::core::Cartridge::support(const INES& rom)
{
    switch (rom.getMapperType())
    {
    case 0: case 1: case 2: case 3:
    case 4: case 7: case 9: case 10:
    case 11:case 13:case 94:case 180:
        return true;
    }
    return false;
}

struct fcpp::core::Cartridge::CartridgeData
{
    FC* fc = nullptr;
    INES content{};
    std::unique_ptr<detail::Mapper> mapper{};
};

fcpp::core::Cartridge::Cartridge() : dptr(std::make_unique<CartridgeData>()) {}
fcpp::core::Cartridge::~Cartridge() noexcept = default;

void fcpp::core::Cartridge::connect(void* const p) noexcept
{
    dptr->fc = static_cast<FC*>(p);
}
void fcpp::core::Cartridge::save(void* p) noexcept
{
    dptr->mapper->save(static_cast<Snapshot*>(p)->getWriter());
}
void fcpp::core::Cartridge::load(void* p) noexcept
{
    dptr->mapper->load(static_cast<Snapshot*>(p)->getReader());
}

bool fcpp::core::Cartridge::load(const char* const path)
{
    if (!dptr->content.load(path)) return false;
    return (dptr->mapper = detail::createMapper(&dptr->content, dptr->fc)) != nullptr;
}
bool fcpp::core::Cartridge::load(const INES& content)
{
    dptr->content = content;
    return (dptr->mapper = detail::createMapper(&dptr->content, dptr->fc)) != nullptr;
}
bool fcpp::core::Cartridge::load(INES&& content)
{
    dptr->content = std::move(content);
    return (dptr->mapper = detail::createMapper(&dptr->content, dptr->fc)) != nullptr;
}

fcpp::core::INES& fcpp::core::Cartridge::getContent() noexcept
{
    return dptr->content;
}
fcpp::core::MirrorType fcpp::core::Cartridge::getMirrorType() noexcept
{
    return dptr->mapper->getMirrorType();
}

std::uint8_t fcpp::core::Cartridge::readPRG(const std::uint16_t addr) noexcept
{
    return dptr->mapper->readPRG(addr);
}
void fcpp::core::Cartridge::writePRG(const std::uint16_t addr, const std::uint8_t data) noexcept
{
    dptr->mapper->writePRG(addr, data);
}

std::uint8_t fcpp::core::Cartridge::readCHR(const std::uint16_t addr) noexcept
{
    return dptr->mapper->readCHR(addr);
}
void fcpp::core::Cartridge::writeCHR(const std::uint16_t addr, const std::uint8_t data) noexcept
{
    dptr->mapper->writeCHR(addr, data);
}

void fcpp::core::Cartridge::sync() noexcept
{
    dptr->mapper->sync();
}
