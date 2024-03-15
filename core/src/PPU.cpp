#include <cstring>

#include "FCPP/Core/PPU.hpp"
#include "FCPP/Core/FC.hpp"

namespace fcpp::core::detail
{
    class PPUImpl
    {
    private:
        enum class ScanlineType
        {
            PRE, VISIBLE, POST, NMI
        };

        struct CtrlRegister
        {
            std::uint8_t n : 2; // nametable select (NN)
            std::uint8_t i : 1; // increment mode (I)
            std::uint8_t s : 1; // sprite tile select (S)
            std::uint8_t b : 1; // background tile select (B)
            std::uint8_t h : 1; // sprite height (H)
            std::uint8_t p : 1; // PPU master/slave (P)
            std::uint8_t v : 1; // NMI enable (V)

            std::uint8_t operator=(const std::uint8_t value) noexcept
            {
                n = value & 3;
                i = value & (1 << 2) ? 1 : 0;
                s = value & (1 << 3) ? 1 : 0;
                b = value & (1 << 4) ? 1 : 0;
                h = value & (1 << 5) ? 1 : 0;
                p = value & (1 << 6) ? 1 : 0;
                v = value & (1 << 7) ? 1 : 0;

                return value;
            }
            operator std::uint8_t() const noexcept
            {
                return
                    v << 7 | p << 6 | h << 5 | b << 4 |
                    s << 3 | i << 2 | n;
            }
            std::uint16_t increment() const noexcept
            {
                return i == 0 ? 1 : 32;
            }
            std::uint16_t bgPatAddr() const noexcept
            {
                return b == 0 ? 0x0000 : 0x1000;
            }
            std::uint16_t spPatAddr() const noexcept
            {
                return s == 0 ? 0x0000 : 0x1000;
            }
            std::uint8_t spHeight() const noexcept
            {
                return h == 0 ? 8 : 16;
            }
        };

        struct MaskRegister
        {
            std::uint8_t g : 1; // Greyscale (0: normal color, 1: produce a greyscale display)
            std::uint8_t m : 1; // 1: Show background in leftmost 8 pixels of screen, 0: Hide
            std::uint8_t M : 1; // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
            std::uint8_t b : 1; // 1: Show background
            std::uint8_t s : 1; // 1: Show sprites
            std::uint8_t R : 1; // Emphasize red (green on PAL/Dendy)
            std::uint8_t G : 1; // Emphasize green (red on PAL/Dendy)
            std::uint8_t B : 1; // Emphasize blue

            std::uint8_t operator=(const std::uint8_t value) noexcept
            {
                g = value & (1 << 0) ? 1 : 0;
                m = value & (1 << 1) ? 1 : 0;
                M = value & (1 << 2) ? 1 : 0;
                b = value & (1 << 3) ? 1 : 0;
                s = value & (1 << 4) ? 1 : 0;
                R = value & (1 << 5) ? 1 : 0;
                G = value & (1 << 6) ? 1 : 0;
                B = value & (1 << 7) ? 1 : 0;

                return value;
            }
            operator std::uint8_t() const noexcept
            {
                return
                    B << 7 | G << 6 | R << 5 | s << 4 |
                    b << 3 | M << 2 | m << 1 | g;
            }
            bool rendering() const noexcept
            {
                return b || s;
            }
        };

        struct StatusRegister
        {
            std::uint8_t u : 5; // Least significant bits previously written into a PPU register
            std::uint8_t o : 1; // Sprite overflow.
            std::uint8_t s : 1; // Sprite 0 Hit.
            std::uint8_t v : 1; // Vertical blank has started

            std::uint8_t operator=(const std::uint8_t value) noexcept
            {
                o = value & (1 << 5) ? 1 : 0;
                s = value & (1 << 6) ? 1 : 0;
                v = value & (1 << 7) ? 1 : 0;

                return value;
            }
            operator std::uint8_t() const noexcept
            {
                return v << 7 | s << 6 | o << 5;
            }
        };

        struct PPUAddress
        {
            std::uint16_t coarseX : 5;  // Tile X
            std::uint16_t coarseY : 5;  // Tile Y
            std::uint16_t n : 2;        // Name table index
            std::uint16_t fineY : 3;    // Tile fine y
            std::uint16_t u : 1;        // Unused

            std::uint16_t operator=(const std::uint16_t value) noexcept
            {
                coarseX = value & 0x001f;
                coarseY = (value >> 5) & 0x001f;
                n = (value >> 10) & 0x0003;
                fineY = (value >> 12) & 0x0007;

                return value;
            }
            operator std::uint16_t() const noexcept
            {
                return fineY << 12 | n << 10 | coarseY << 5 | coarseX;
            }
            std::uint16_t operator+=(const std::uint16_t n) noexcept
            {
                return *this = *this + n;
            }

            std::uint16_t ntAddr() const noexcept
            {
                return 0x2000 | (*this & 0x0fff);
            }
            std::uint16_t atAddr() const noexcept
            {
                return 0x23c0 | (n << 10) | ((coarseY >> 2) << 3) | (coarseX >> 2);
            }
            void incH() noexcept
            {
                if (coarseX == 0x1f) *this = *this ^ 0x41f;
                else coarseX++;
            }
            void incV() noexcept
            {
                if (fineY < 7) fineY++;
                else
                {
                    fineY = 0;
                    if (coarseY == 31) coarseY = 0;
                    else if (coarseY == 29)
                    {
                        coarseY = 0;
                        n ^= 2;
                    }
                    else coarseY++;
                }
            }
            void updateH(const PPUAddress& t) noexcept
            {
                *this = (*this & ~0x041f) | (t & 0x041f);
            }
            void updateV(const PPUAddress& t) noexcept
            {
                *this = (*this & ~0x7be0) | (t & 0x7be0);
            }
        };

        struct BackgroundData
        {
            std::uint16_t bgShiftL = 0, bgShiftH = 0;
            std::uint8_t atShiftL = 0, atShiftH = 0;
            std::uint8_t atLatchL = 0, atLatchH = 0;
            std::uint8_t nt = 0, at = 0, bgL = 0, bgH = 0;

            void reload() noexcept
            {
                bgShiftL = (bgShiftL & 0xff00) | bgL;
                bgShiftH = (bgShiftH & 0xff00) | bgH;

                atLatchL = (at & 1) ? 1 : 0;
                atLatchH = (at & 2) ? 1 : 0;
            }
            void shift() noexcept
            {
                bgShiftL <<= 1;
                bgShiftH <<= 1;
                atShiftL = (atShiftL << 1) | atLatchL;
                atShiftH = (atShiftH << 1) | atLatchH;
            }
        };

        struct OAM
        {
            struct Sprite
            {
                std::uint8_t id;     // Index in OAM.
                std::uint8_t x;      // X position.
                std::uint8_t y;      // Y position.
                std::uint8_t tile;   // Tile index.
                std::uint8_t attr;   // Attributes.
                std::uint8_t spL;    // Tile data (low).
                std::uint8_t spH;    // Tile data (high).
            };

            std::uint8_t mem[0x100];     // VRAM for sprite properties.
            Sprite buf[16];              // Sprite buffers.
            std::uint8_t spCount = 0;
            std::uint8_t spLimit = 16;

            OAM() noexcept
            {
                std::memset(mem, 0xff, sizeof(mem));
                std::memset(buf, 0xff, sizeof(buf));
            }
        };
    private:
        void write(std::uint16_t addr, std::uint8_t data) noexcept;
        std::uint8_t read(std::uint16_t addr) noexcept;

        void spriteEvaluation() noexcept;
        void spriteLoad() noexcept;
        void backgroundLoad() noexcept;
        void incrementAddr() noexcept;
        void draw() noexcept;

        template<ScanlineType s> void cycle() noexcept;
    public:
        void connect(Cartridge* cartridge, Bus* bus, CPU* cpu) noexcept;
        void setFrameBuffer(FrameBuffer* frameBuffer) noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void clear() noexcept;
        void exec() noexcept;

        template<PPU::Registers reg> void set(std::uint8_t v) noexcept;
        template<PPU::Registers reg> std::uint8_t get() noexcept;
        template<PPU::State::Type type> void set(unsigned int v) noexcept;
        template<PPU::State::Type type> unsigned int get() const noexcept;
    private:
        std::uint16_t dot = 0, scanline = 0;
        MaskRegister mask{};
        StatusRegister status{};
        CtrlRegister ctrl{};

        bool oddFrame = false;
        bool latch = false;
        std::uint8_t fineX = 0;
        std::uint8_t oamAddr = 0;
        std::uint8_t dataBuffer = 0;
        std::uint8_t updateAddrDelay = 0;
        std::uint16_t addrBus = 0;
        PPUAddress vAddr{}, tAddr{};
        BackgroundData bgData{};
        OAM oam{};
    private:
        Cartridge* cartridge = nullptr;
        Bus* bus = nullptr;
        CPU* cpu = nullptr;
        FrameBuffer* frameBuffer = nullptr;
        const std::uint32_t* paletteTable = nullptr;
    private:
        static constexpr std::uint32_t defaultPaletteTable[64] = {
            0xff7c7c7c, 0xff0000fc, 0xff0000bc, 0xff4428bc, 0xff940084, 0xffa80020, 0xffa81000, 0xff881400,
            0xff503000, 0xff007800, 0xff006800, 0xff005800, 0xff004058, 0xff000000, 0xff000000, 0xff000000,
            0xffbcbcbc, 0xff0078f8, 0xff0058f8, 0xff6844fc, 0xffd800cc, 0xffe40058, 0xfff83800, 0xffe45c10,
            0xffac7c00, 0xff00b800, 0xff00a800, 0xff00a844, 0xff008888, 0xff000000, 0xff000000, 0xff000000,
            0xfff8f8f8, 0xff3cbcfc, 0xff6888fc, 0xff9878f8, 0xfff878f8, 0xfff85898, 0xfff87858, 0xfffca044,
            0xfff8b800, 0xffb8f818, 0xff58d854, 0xff58f898, 0xff00e8d8, 0xff787878, 0xff000000, 0xff000000,
            0xfffcfcfc, 0xffa4e4fc, 0xffb8b8f8, 0xffd8b8f8, 0xfff8b8f8, 0xfff8a4c0, 0xfff0d0b0, 0xfffce0a8,
            0xfff8d878, 0xffd8f878, 0xffb8f8b8, 0xffb8f8d8, 0xff00fcfc, 0xfff8d8f8, 0xff000000, 0xff000000
        }; // ARGB
    };

    inline void PPUImpl::write(const std::uint16_t addr, const std::uint8_t data) noexcept
    {
        bus->write<PPU>(addr, data);
    }
    inline std::uint8_t PPUImpl::read(const std::uint16_t addr) noexcept
    {
        return bus->read<PPU>(addr);
    }

    inline void PPUImpl::spriteEvaluation() noexcept
    {
        oam.spCount = 0;
        for (int i = 0; i < 64; i++)
        {
            std::uint16_t diff = scanline - oam.mem[i * 4 + 0];
            if (diff < ctrl.spHeight())
            {
                if (oam.spCount == 8)
                {
                    status.o = 1;
                    if (oam.spLimit == 8) break;
                }
                if (oam.spCount < oam.spLimit)
                {
                    oam.buf[oam.spCount].id = i;
                    oam.buf[oam.spCount].y = oam.mem[i * 4 + 0];
                    oam.buf[oam.spCount].tile = oam.mem[i * 4 + 1];
                    oam.buf[oam.spCount].attr = oam.mem[i * 4 + 2];
                    oam.buf[oam.spCount].x = oam.mem[i * 4 + 3];
                    if ((++oam.spCount == oam.spLimit) && oam.spLimit > 8) break;
                }
            }
        }
    }
    inline void PPUImpl::spriteLoad() noexcept
    {
        if (dot > 260 && dot < 318 && (dot & 7) == 5)
        {
            int n = (dot - 257) / 8;
            if (n < oam.spCount)
            {
                do
                {   // Different address modes depending on the sprite height:
                    if (ctrl.spHeight() == 16) addrBus = ((oam.buf[n].tile & 1) * 0x1000) + ((oam.buf[n].tile & 0xfe) * 16);
                    else addrBus = ctrl.spPatAddr() + (oam.buf[n].tile * 16);

                    std::uint16_t spY = scanline - oam.buf[n].y; // Line inside the sprite.
                    if (oam.buf[n].attr & 0x80) spY ^= ctrl.spHeight() - 1; // Vertical flip.
                    addrBus += spY + (spY & 0x08); // Select the second tile if on 8x16.

                    oam.buf[n].spL = read(addrBus);
                    oam.buf[n].spH = read(addrBus += 8);
                } while (dot == 317 && ++n < oam.spCount);
            }
            else if (ctrl.spHeight() == 16) addrBus = 0x1fe8;
            else addrBus = ctrl.spPatAddr() + 0x0ff8;
        }
    }
    inline void PPUImpl::backgroundLoad() noexcept
    {
        if ((dot >= 2 && dot <= 254) || (dot >= 322 && dot <= 337))
        {
            bgData.shift();
            switch ((dot - 1) & 7)
            {
            case 0:
                bgData.reload();
                addrBus = vAddr.ntAddr();
                break;
            case 1:
                bgData.nt = read(addrBus);
                break;
            case 2:
                addrBus = vAddr.atAddr();
                break;
            case 3:
                bgData.at = read(addrBus);
                if (vAddr.coarseY & 2) bgData.at >>= 4;
                if (vAddr.coarseX & 2) bgData.at >>= 2;
                break;
            case 4:
                addrBus = ctrl.bgPatAddr() + (bgData.nt * 16) + vAddr.fineY;
                break;
            case 5:
                bgData.bgL = read(addrBus);
                break;
            case 6:
                addrBus += 8;
                break;
            case 7:
                bgData.bgH = read(addrBus);
                vAddr.incH();
                break;
            }
        }
        else if (dot == 255)
        {
            bgData.shift();
            addrBus += 8;
            vAddr.incV();
        }
        else if (dot == 256)
        {
            bgData.shift();
            bgData.bgH = read(addrBus);
        }
        else if (dot == 257)
        {
            bgData.shift();
            bgData.reload();
            vAddr.updateH(tAddr);
        }
        else if (dot == 1 || dot == 321 || dot == 339) addrBus = vAddr.ntAddr();
        else if (dot == 338 || dot == 340) bgData.nt = read(addrBus);

    }
    inline void PPUImpl::incrementAddr() noexcept
    {
        if ((scanline < 240 || scanline == 261) && mask.rendering())
        {
            vAddr.incH();
            vAddr.incV();
        }
        else vAddr += ctrl.increment();
    }
    inline void PPUImpl::draw() noexcept
    {
        const int x = dot - 2;
        std::uint8_t palette = 0;
        if (mask.rendering())
        {
            std::uint8_t spPalette = 0;
            bool spPriority = false;

            if (mask.b && !(!mask.m && x < 8))
            {
                palette = (((bgData.bgShiftH >> (15 - fineX)) & 1) << 1) | ((bgData.bgShiftL >> (15 - fineX)) & 1);
                if (palette) palette |=
                    ((((bgData.atShiftH >> (7 - fineX)) & 1) << 1) | ((bgData.atShiftL >> (7 - fineX)) & 1)) << 2;
            }
            if (mask.s && !(!mask.M && x < 8))
            {
                for (int i = 0; i < oam.spCount; i++) // Small id is preferred
                {
                    std::uint16_t spX = x - oam.buf[i].x;
                    if (spX >= 8) continue; // Not in range.

                    if (oam.buf[i].attr & 0x40) spX ^= 7; // Horizontal flip.
                    spPalette = (((oam.buf[i].spH >> (7 - spX)) & 1) << 1) | ((oam.buf[i].spL >> (7 - spX)) & 1);
                    if (spPalette == 0) continue; // Transparent pixel.

                    if (oam.buf[i].id == 0 && !status.s && palette && x != 255) status.s = 1; // Sprite zero hit
                    spPalette |= ((oam.buf[i].attr & 0x03) + 0x04) << 2;
                    spPriority = !(oam.buf[i].attr & 0x20);
                    break;
                }
            }
            if (spPalette && (palette == 0 || spPriority)) palette = spPalette;
        }
        else palette = (~vAddr & 0x3f00) ? 0 : vAddr & 0x1f;
        frameBuffer->setPixel(x, scanline, paletteTable[read(0x3f00 + palette) & (mask.g ? 0x30 : 0x3f)]);
    }

    template<> // reference https://www.nesdev.org/wiki/PPU_rendering
    inline void PPUImpl::cycle<PPUImpl::ScanlineType::VISIBLE>() noexcept
    {
        if (dot >= 2 && dot <= 257) draw();
        if (mask.rendering())
        {
            backgroundLoad();
            spriteLoad();
            if (dot == 258) spriteEvaluation();
        }
    }
    template<>
    inline void PPUImpl::cycle<PPUImpl::ScanlineType::POST>() noexcept
    {
        if (dot == 0) frameBuffer->completedSignal();
    }
    template<>
    inline void PPUImpl::cycle<PPUImpl::ScanlineType::NMI>() noexcept
    {
        if (dot == 1)
        {
            status.v = 1;
            cpu->requestNMI(ctrl.v);
        }
    }
    template<>
    inline void PPUImpl::cycle<PPUImpl::ScanlineType::PRE>() noexcept
    {
        if (dot == 2) status.o = status.s = status.v = 0;
        if (mask.rendering())
        {
            backgroundLoad();
            spriteLoad();
            if (dot >= 280 && dot <= 304) vAddr.updateV(tAddr);
            else if (dot == 339 && oddFrame) dot++;
        }
    }

    void PPUImpl::connect(Cartridge* const cartridge, Bus* const bus, CPU* const cpu) noexcept
    {
        this->cartridge = cartridge;
        this->bus = bus;
        this->cpu = cpu;
    }
    void PPUImpl::setFrameBuffer(FrameBuffer* const frameBuffer) noexcept
    {
        this->frameBuffer = frameBuffer;
        auto externalPaletteTable = frameBuffer->getPaletteTable();
        this->paletteTable = (externalPaletteTable == nullptr) ? defaultPaletteTable : externalPaletteTable;
    }
    template<typename Accessor>
    inline void PPUImpl::access(Accessor& accessor) noexcept
    {
        accessor.access(mask);
        accessor.access(status);
        accessor.access(ctrl);
        accessor.access(vAddr);
        accessor.access(tAddr);
        accessor.access(dot);
        accessor.access(scanline);
        accessor.access(oddFrame);
        accessor.access(latch);
        accessor.access(fineX);
        accessor.access(oamAddr);
        accessor.access(dataBuffer);
        accessor.access(updateAddrDelay);
        accessor.access(addrBus);
        accessor.access(bgData.bgShiftL);
        accessor.access(bgData.bgShiftH);
        accessor.access(bgData.atShiftL);
        accessor.access(bgData.atShiftH);
        accessor.access(bgData.atLatchL);
        accessor.access(bgData.atLatchH);
        accessor.access(bgData.nt);
        accessor.access(bgData.at);
        accessor.access(bgData.bgL);
        accessor.access(bgData.bgH);
        accessor.access(oam.spCount);
        accessor.access(oam.buf, sizeof(oam.buf));
        accessor.access(oam.mem, sizeof(oam.mem));
    }
    inline void PPUImpl::clear() noexcept
    {
        oddFrame = latch = false;
        scanline = dot = fineX = oamAddr = dataBuffer = updateAddrDelay = 0;
        ctrl = {};
        mask = {};
        status = {};
        bgData = {};
    }
    inline void PPUImpl::exec() noexcept
    {
        if (scanline < 240) cycle<ScanlineType::VISIBLE>();
        else if (scanline == 240) cycle<ScanlineType::POST>();
        else if (scanline == 241) cycle<ScanlineType::NMI>();
        else if (scanline == 261) cycle<ScanlineType::PRE>();

        cartridge->sync();

        if (updateAddrDelay && (--updateAddrDelay == 0)) vAddr = tAddr;
        if (++dot > 340)
        {
            dot = 0;
            if (++scanline > 261)
            {
                scanline = 0;
                oddFrame = !oddFrame;
            }
        }
    }

    template<> inline void PPUImpl::set<PPU::Registers::PPUCTRL>(const std::uint8_t v) noexcept
    {
        if (!(v & 0x80)) cpu->requestNMI(false);
        else if (!ctrl.v && status.v && !(scanline == 261 && dot == 2)) cpu->requestNMI(true); // NMI shouldn't occur if VBlank flag is clear
        ctrl = v;
        tAddr.n = ctrl.n;
    }
    template<> inline void PPUImpl::set<PPU::Registers::PPUMASK>(const std::uint8_t v) noexcept
    {
        mask = v;
    }
    template<> inline void PPUImpl::set<PPU::Registers::OAMADDR>(const std::uint8_t v) noexcept
    {
        oamAddr = v;
    }
    template<> inline void PPUImpl::set<PPU::Registers::OAMDATA>(const std::uint8_t v) noexcept
    {
        oam.mem[oamAddr++] = v;
    }
    template<> inline void PPUImpl::set<PPU::Registers::PPUSCROLL>(const std::uint8_t v) noexcept
    {
        if (!latch)
        {
            fineX = v & 0x07;
            tAddr.coarseX = v >> 3;
        }
        else
        {
            tAddr.fineY = v & 0x07;
            tAddr.coarseY = v >> 3;
        }
        latch = !latch;
    }
    template<> inline void PPUImpl::set<PPU::Registers::PPUADDR>(const std::uint8_t v) noexcept
    {
        if (!latch) tAddr = (static_cast<std::uint16_t>(v & 0x3f) << 8) | (tAddr & 0x00ff);
        else
        {
            tAddr = (tAddr & 0xff00) | v;
            updateAddrDelay = 3;
        }
        latch = !latch;
    }
    template<> inline void PPUImpl::set<PPU::Registers::PPUDATA>(const std::uint8_t v) noexcept
    {
        write(vAddr, v);
        incrementAddr();
    }
    template<> inline std::uint8_t PPUImpl::get<PPU::Registers::PPUSTATUS>() noexcept
    {
        if (scanline == 241 && dot > 1 && dot < 5)
        {// https://www.nesdev.org/wiki/PPU_frame_timing#VBL_Flag_Timing
            if (dot == 2) status.v = 0;
            cpu->requestNMI(false);
        }
        std::uint8_t ret = status;
        status.v = 0;
        latch = false;
        return ret;
    }
    template<> inline std::uint8_t PPUImpl::get<PPU::Registers::OAMDATA>() noexcept
    {   //The 2,3,4 bit of the attribute byte is always zero
        return (oamAddr & 0x03) == 2 ? oam.mem[oamAddr] & 0xe3 : oam.mem[oamAddr];
    }
    template<> inline std::uint8_t PPUImpl::get<PPU::Registers::PPUDATA>() noexcept
    {
        std::uint8_t ret = 0;
        if ((addrBus = (vAddr & 0x3fff)) < 0x3f00)
        {
            ret = dataBuffer;
            dataBuffer = read(addrBus);
        }
        else
        {
            ret = read(addrBus);
            dataBuffer = read(addrBus - 0x1000);
        }
        incrementAddr();
        return ret;
    }
    template<> inline void PPUImpl::set<PPU::State::Type::SpriteLimit>(const unsigned int v) noexcept
    {
        oam.spLimit = v < 8 ? 8 : (16 < v ? 16 : v);
    }
    template<> inline unsigned int PPUImpl::get<PPU::State::Type::SpriteLimit>() const noexcept
    {
        return oam.spLimit;
    }
    template<> inline unsigned int PPUImpl::get<PPU::State::Type::AddressBus>() const noexcept
    { // A0-A13 are not tristated and output the contents of the V register whenever rendering is disabled.
        return (mask.rendering() ? addrBus : vAddr) & 0x3fff;
    }
}

struct fcpp::core::PPU::PPUData
{
    detail::PPUImpl impl{};
    std::uint8_t openBusData = 0;
};

fcpp::core::PPU::PPU() : dptr(std::make_unique<PPUData>()) {}
fcpp::core::PPU::~PPU() noexcept = default;

void fcpp::core::PPU::connect(void* const p) noexcept
{
    auto fptr = static_cast<FC*>(p);
    dptr->impl.connect(fptr->getCartridge(), fptr->getBus(), fptr->getCPU());
}
void fcpp::core::PPU::save(void* const p) noexcept
{
    auto& writer = static_cast<Snapshot*>(p)->getWriter();
    writer.access(dptr->openBusData);
    dptr->impl.access(writer);
}
void fcpp::core::PPU::load(void* const p) noexcept
{
    auto& reader = static_cast<Snapshot*>(p)->getReader();
    reader.access(dptr->openBusData);
    dptr->impl.access(reader);
}
void fcpp::core::PPU::reset() noexcept
{
    dptr->openBusData = 0;
    dptr->impl.clear();
}

void fcpp::core::PPU::exec() noexcept
{
    dptr->impl.exec();
}

template<>
std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUSTATUS>() noexcept
{
    return dptr->openBusData = dptr->impl.get<Registers::PPUSTATUS>() | (dptr->openBusData & 0x1f);
}
template<>
std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::OAMDATA>() noexcept
{
    return dptr->openBusData = dptr->impl.get<Registers::OAMDATA>();
}
template<>
std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUDATA>() noexcept
{
    return dptr->openBusData = dptr->impl.get<Registers::PPUDATA>();
}
template<fcpp::core::PPU::Registers reg>
std::uint8_t fcpp::core::PPU::get() noexcept
{
    return dptr->openBusData;
}
template<>
void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUSTATUS>(const std::uint8_t v) noexcept
{
    dptr->openBusData = v;
}
template<fcpp::core::PPU::Registers reg>
void fcpp::core::PPU::set(const std::uint8_t v) noexcept
{
    dptr->impl.set<reg>(dptr->openBusData = v);
}
template<fcpp::core::PPU::State::Type type>
unsigned int fcpp::core::PPU::get() const noexcept
{
    return dptr->impl.get<type>();
}
template<fcpp::core::PPU::State::Type type>
void fcpp::core::PPU::set(const unsigned int v) noexcept
{
    dptr->impl.set<type>(v);
}
void fcpp::core::PPU::set(FrameBuffer* const frameBuffer) noexcept
{
    dptr->impl.setFrameBuffer(frameBuffer);
}

template std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUCTRL>() noexcept;
template std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUMASK>() noexcept;
template std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::OAMADDR>() noexcept;
template std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUSCROLL>() noexcept;
template std::uint8_t fcpp::core::PPU::get<fcpp::core::PPU::Registers::PPUADDR>() noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUCTRL>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUMASK>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::OAMADDR>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::OAMDATA>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUSCROLL>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUADDR>(const std::uint8_t) noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::Registers::PPUDATA>(const std::uint8_t) noexcept;
template unsigned int fcpp::core::PPU::get<fcpp::core::PPU::State::Type::SpriteLimit>() const noexcept;
template unsigned int fcpp::core::PPU::get<fcpp::core::PPU::State::Type::AddressBus>() const noexcept;
template void fcpp::core::PPU::set<fcpp::core::PPU::State::Type::SpriteLimit>(const unsigned int) noexcept;
