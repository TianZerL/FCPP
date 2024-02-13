#include "FCPP/Core/APU.hpp"
#include "FCPP/Core/FC.hpp"

namespace fcpp::core::detail
{
    struct Timer
    {
        std::uint16_t counter = 0;
        std::uint16_t period = 0;

        bool step() noexcept
        {
            if (counter) counter--;
            else
            {
                counter = period;
                return true;
            }
            return false;
        }
    };

    class Envelope
    {
    public:
        std::uint8_t getVolume() const noexcept;
        void set(std::uint8_t value) noexcept;
        void start() noexcept;
        void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t divider = 0;
        std::uint8_t decayLevel = 0;
        std::uint8_t volume = 0;
        bool startFlag = false;
        bool loopFlag = false;
        bool constantFlag = false;
    };
    inline std::uint8_t Envelope::getVolume() const noexcept
    {
        return constantFlag ? volume : decayLevel;
    }
    inline void Envelope::set(const std::uint8_t v) noexcept
    {
        volume = v & 0x0f;
        constantFlag = v & (1 << 4);
        loopFlag = v & (1 << 5);
    }
    inline void Envelope::start() noexcept
    {
        startFlag = true;
    }
    inline void Envelope::step() noexcept
    {   // reference https://wiki.nesdev.org/w/index.php?title=APU_Envelope
        if (startFlag)
        {
            startFlag = false;
            decayLevel = 15;
            divider = volume;
        }
        else if (divider) divider--;
        else
        {
            divider = volume;
            if (decayLevel) decayLevel--;
            else if (loopFlag) decayLevel = 15;
        }
    }
    template<typename Accessor>
    inline void Envelope::access(Accessor& accessor) noexcept
    {
        accessor.access(divider);
        accessor.access(decayLevel);
        accessor.access(volume);
        accessor.access(startFlag);
        accessor.access(loopFlag);
        accessor.access(constantFlag);
    }

    class LengthCounter
    {
    public:
        bool zero() const noexcept;
        void set(std::uint8_t v) noexcept;
        void reset() noexcept;
        void halt(bool v) noexcept;
        void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t counter = 0;
        bool haltFlag = false;
    private:
        static constexpr std::uint8_t lengthTable[32] = {
            10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,
            12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
        };
    };
    inline bool LengthCounter::zero() const noexcept
    {
        return counter == 0;
    }
    inline void LengthCounter::set(const std::uint8_t v) noexcept
    {
        counter = lengthTable[v];
    }
    inline void LengthCounter::reset() noexcept
    {
        counter = 0;
    }
    inline void LengthCounter::halt(const bool v) noexcept
    {
        haltFlag = v;
    }
    inline void LengthCounter::step() noexcept
    {
        if (!haltFlag && counter) counter--;
    }
    template<typename Accessor>
    inline void LengthCounter::access(Accessor& accessor) noexcept
    {
        accessor.access(counter);
        accessor.access(haltFlag);
    }

    class Sweep
    {
    public:
        void set(std::uint8_t v) noexcept;
        void reload() noexcept;
        template<std::uint16_t carry> void step(Timer& timer) noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t divider = 0;
        std::uint8_t period = 0;
        std::uint8_t shiftCount = 0;
        bool negateFlag = false;
        bool enableFlag = false;
        bool reloadFlag = false;
    };
    inline void Sweep::set(const std::uint8_t v) noexcept
    {
        shiftCount = v & 0x07;
        period = (v >> 4) & 0x07;
        negateFlag = v & (1 << 3);
        enableFlag = v & (1 << 7);
    }
    inline void Sweep::reload() noexcept
    {
        reloadFlag = true;
    }
    template<std::uint16_t carry>
    inline void Sweep::step(Timer& timer) noexcept
    {   // reference https://wiki.nesdev.org/w/index.php?title=APU_Sweep#Updating%20the%20period
        std::uint16_t delta = timer.period >> shiftCount;
        // Pulse1 adds the one's complement (-C-1)
        // Pulse2 adds the two's complement (-C)
        if (negateFlag) delta = ~delta + carry;

        std::uint16_t targetPeriod = timer.period + delta;
        // The target period is the sum of the current period and the change amount, clamped to zero if this sum is negative.
        if (targetPeriod & 0x8000) targetPeriod = 0;
        
        // If the sweep unit is disabled including because the shift count is zero,
        // the pulse channel's period is never updated, but muting logic still applies.
        bool mute = (timer.period < 8) || (targetPeriod > 0x07ff);

        if (divider == 0 && enableFlag && shiftCount != 0 && !mute) timer.period = targetPeriod;

        if (divider == 0 || reloadFlag)
        {
            divider = period;
            reloadFlag = false;
        }
        else divider--;
    }
    template<typename Accessor>
    inline void Sweep::access(Accessor& accessor) noexcept
    {
        accessor.access(divider);
        accessor.access(period);
        accessor.access(shiftCount);
        accessor.access(negateFlag);
        accessor.access(enableFlag);
        accessor.access(reloadFlag);
    }

    class LinearCounter
    {
    public:
        bool zero() const noexcept;
        void reload() noexcept;
        void set(std::uint8_t v) noexcept;
        void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t counter = 0;
        std::uint8_t period = 0;
        bool controlFlag = false;
        bool reloadFlag = false;
    };
    inline bool LinearCounter::zero() const noexcept
    {
        return counter == 0;
    }
    inline void LinearCounter::reload() noexcept
    {
        reloadFlag = true;
    }
    inline void LinearCounter::set(const std::uint8_t v) noexcept
    {
        controlFlag = v & (1 << 7);
        period = v & 0x7f;
    }
    inline void LinearCounter::step() noexcept
    {
        if (reloadFlag) counter = period;
        else if (counter) counter--;

        if (!controlFlag) reloadFlag = false;
    }
    template<typename Accessor>
    inline void LinearCounter::access(Accessor& accessor) noexcept
    {
        accessor.access(counter);
        accessor.access(period);
        accessor.access(controlFlag);
        accessor.access(reloadFlag);
    }

    class Buffer
    {
    public:
        bool empty() const noexcept;
        std::uint8_t move() noexcept;
        void set(std::uint8_t v) noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t buffer = 0;
        bool emptyFlag = false;
    };
    inline bool Buffer::empty() const noexcept
    {
        return emptyFlag;
    }
    inline std::uint8_t Buffer::move() noexcept
    {
        emptyFlag = true;
        return buffer;
    }
    inline void Buffer::set(const std::uint8_t v) noexcept
    {
        emptyFlag = false;
        buffer = v;
    }
    template<typename Accessor>
    inline void Buffer::access(Accessor& accessor) noexcept
    {
        accessor.access(buffer);
        accessor.access(emptyFlag);
    }

    class Channel
    {
    public:
        bool enable(bool v) noexcept;
        LengthCounter& getLengthCounter() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    protected:
        bool enabled = false;

        Timer timer{};
        LengthCounter lengthCounter{};
    };
    inline bool Channel::enable(const bool v) noexcept
    {
        return enabled = v;
    }
    inline LengthCounter& Channel::getLengthCounter() noexcept
    {
        return lengthCounter;
    }
    template<typename Accessor>
    inline void Channel::access(Accessor& accessor) noexcept
    {
        accessor.access(enabled);
        accessor.access(timer.counter);
        accessor.access(timer.period);
        lengthCounter.access(accessor);
    }

    class Pulse : public Channel
    {
    public:
        std::uint8_t output() const noexcept;
        template<int idx> void set(std::uint8_t v) noexcept;
        template<typename Unit, int... idx> void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t dutyCycle = 0;
        std::uint8_t offset = 7;

        Envelope envelope{};
        Sweep sweep{};
    private:
        static constexpr std::uint8_t sequenceLookupTable[4] = {
            0b01000000, 0b01100000, 0b01111000, 0b10011111
        };
    };
    inline std::uint8_t Pulse::output() const noexcept
    {//muting if frequency higher than 13kHz
        return (!enabled || (timer.period < 8) || (timer.period > 0x07ff) ||
            lengthCounter.zero() || !((dutyCycle >> offset) & 1)) ?
            0 : envelope.getVolume();
    }
    template<> inline void Pulse::set<0x00>(const std::uint8_t v) noexcept
    {
        envelope.set(v);
        lengthCounter.halt((v >> 5) & 1);
        dutyCycle = sequenceLookupTable[(v >> 6) & 3];
    }
    template<> inline void Pulse::set<0x01>(const std::uint8_t v) noexcept
    {
        sweep.set(v);
    }
    template<> inline void Pulse::set<0x02>(const std::uint8_t v) noexcept
    {
        timer.period = (timer.period & 0xff00) | static_cast<std::uint16_t>(v);
    }
    template<> inline void Pulse::set<0x03>(const std::uint8_t v) noexcept
    {
        timer.period = (static_cast<std::uint16_t>(v & 7) << 8) | (timer.period & 0x00ff);
        if (enabled) lengthCounter.set(v >> 3);
        envelope.start();
        offset = 7;
    }
    template<> inline void Pulse::step<Timer>() noexcept
    {
        if (timer.step()) offset = offset ? (offset - 1) : 7;
    }
    template<> inline void Pulse::step<Envelope>() noexcept
    {
        envelope.step();
    }
    template<> inline void Pulse::step<LengthCounter>() noexcept
    {
        lengthCounter.step();
    }
    template<> inline void Pulse::step<Sweep, 1>() noexcept
    {
        sweep.step<0>(timer); // Pulse1, carry is 0
    }
    template<> inline void Pulse::step<Sweep, 2>() noexcept
    {
        sweep.step<1>(timer); // Pulse2, carry is 1
    }
    template<typename Accessor>
    inline void Pulse::access(Accessor& accessor) noexcept
    {
        Channel::access(accessor);
        envelope.access(accessor);
        sweep.access(accessor);
        accessor.access(dutyCycle);
        accessor.access(offset);
    }

    class Triangle : public Channel
    {
    public:
        std::uint8_t output() const noexcept;
        template<int idx> void set(std::uint8_t v) noexcept;
        template<typename Unit> void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint8_t offset = 31;

        LinearCounter linearCounter{};
    private:
        static constexpr std::uint8_t sequenceLookupTable[32] = {
            15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
             0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
        };
    };
    inline std::uint8_t Triangle::output() const noexcept
    {//muting if frequency higher than 20kHz
        return (!enabled || (timer.period < 3) || (timer.period > 0x07ff) || linearCounter.zero() || lengthCounter.zero()) ?
            0 : sequenceLookupTable[offset];
    }
    template<> inline void Triangle::set<0x08>(const std::uint8_t v) noexcept
    {
        lengthCounter.halt((v >> 7) & 1);
        linearCounter.set(v);
    }
    template<> inline void Triangle::set<0x0a>(const std::uint8_t v) noexcept
    {
        timer.period = (timer.period & 0xff00) | static_cast<std::uint16_t>(v);
    }
    template<> inline void Triangle::set<0x0b>(const std::uint8_t v) noexcept
    {
        timer.period = (static_cast<std::uint16_t>(v & 7) << 8) | (timer.period & 0x00ff);
        if (enabled) lengthCounter.set(v >> 3);
        linearCounter.reload();
    }
    template<> inline void Triangle::step<Timer>() noexcept
    {//The sequencer is clocked by the timer as long as both the linear counter and the length counter are nonzero.
        if (timer.step() && !linearCounter.zero() && !lengthCounter.zero())
            offset = offset ? offset - 1 : 31;
    }
    template<> inline void Triangle::step<LengthCounter>() noexcept
    {
        lengthCounter.step();
    }
    template<> inline void Triangle::step<LinearCounter>() noexcept
    {
        linearCounter.step();
    }
    template<typename Accessor>
    inline void Triangle::access(Accessor& accessor) noexcept
    {
        Channel::access(accessor);
        linearCounter.access(accessor);
        accessor.access(offset);
    }

    class Noise : public Channel
    {
    public:
        std::uint8_t output() const noexcept;
        template<int idx> void set(std::uint8_t v) noexcept;
        template<typename Unit> void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint16_t shiftRegister = 0x01;
        bool mode = false;

        Envelope envelope{};
    private:
        static constexpr std::uint16_t sequenceLookupTable[16] = {
            4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
        };
    };
    inline std::uint8_t Noise::output() const noexcept
    {
        return (!enabled || (shiftRegister & 1) || lengthCounter.zero()) ? 0 : envelope.getVolume();
    }
    template<> inline void Noise::set<0x0c>(const std::uint8_t v) noexcept
    {
        envelope.set(v);
        lengthCounter.halt((v >> 5) & 1);
    }
    template<> inline void Noise::set<0x0e>(const std::uint8_t v) noexcept
    {
        mode = v & (1 << 7);
        timer.period = sequenceLookupTable[v & 0x0f];
    }
    template<> inline void Noise::set<0x0f>(const std::uint8_t v) noexcept
    {
        if (enabled) lengthCounter.set(v >> 3);
        envelope.start();
    }
    template<> inline void Noise::step<Timer>() noexcept
    {
        if (timer.step())
        {
            std::uint16_t shift = mode ? 6 : 1;

            std::uint16_t bit0 = shiftRegister & 1;
            std::uint16_t other = (shiftRegister >> shift) & 1;

            shiftRegister >>= 1;
            shiftRegister |= (bit0 ^ other) << 14;
        }
    }
    template<> inline void Noise::step<Envelope>() noexcept
    {
        envelope.step();
    }
    template<> inline void Noise::step<LengthCounter>() noexcept
    {
        lengthCounter.step();
    }
    template<typename Accessor>
    inline void Noise::access(Accessor& accessor) noexcept
    {
        Channel::access(accessor);
        envelope.access(accessor);
        accessor.access(shiftRegister);
        accessor.access(mode);
    }

    class DMC
    {
    public:
        void connect(Clock* clock, Bus* bus, CPU* cpu) noexcept;
        Buffer& getBuffer() noexcept;
        bool zeroBytesRemaining() const noexcept;
        void resetBytesRemaining() noexcept;
        bool checkInterrupt() const noexcept;
        void clearInterrupt() noexcept;
        void start() noexcept;
        void load() noexcept;
        std::uint8_t output() const noexcept;
        template<int idx> void set(std::uint8_t v) noexcept;
        template<typename Unit> void step() noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
    private:
        std::uint16_t bytesRemainingCounter = 0;
        std::uint16_t addressCounter = 0xc000;
        std::uint16_t sampleAddress = 0xc000;
        std::uint16_t sampleLength = 0;
        std::uint8_t outputLevel = 0;
        std::uint8_t bitsRemainingCounter = 8;
        std::uint8_t shiftRegister = 0;
        bool loading = false;
        bool silenceFlag = false;
        bool enableIRQ = false;
        bool interruptFlag = false;
        bool loopFlag = false;

        Timer timer{};
        Buffer buffer{};
    private:
        Clock* clock = nullptr;
        Bus* bus = nullptr;
        CPU* cpu = nullptr;
    private:
        static constexpr std::uint16_t sequenceLookupTable[16] = {
            428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54
        };
    };
    inline void DMC::connect(Clock* const clock, Bus* const bus, CPU* const cpu) noexcept
    {
        this->clock = clock;
        this->bus = bus;
        this->cpu = cpu;
    }
    inline Buffer& DMC::getBuffer() noexcept
    {
        return buffer;
    }
    inline bool DMC::zeroBytesRemaining() const noexcept
    {
        return bytesRemainingCounter == 0;
    }
    inline void DMC::resetBytesRemaining() noexcept
    {
        bytesRemainingCounter = 0;
    }
    inline bool DMC::checkInterrupt() const noexcept
    {
        return interruptFlag;
    }
    inline void DMC::clearInterrupt() noexcept
    {
        cpu->requestIRQ<CPU::IRQType::DMC>(interruptFlag = false);
    }
    inline void DMC::start() noexcept
    {
        addressCounter = sampleAddress;
        bytesRemainingCounter = sampleLength;
    }
    inline void DMC::load() noexcept
    {
        if (!bytesRemainingCounter || loading) return;
        loading = true;
        unsigned int stalled = cpu->get<CPU::State::Type::DMAState>();
        if (stalled == CPU::State::DMA_STATE_DISABLE) stalled = cpu->get<CPU::State::Type::TickState>() == CPU::State::TICK_STATE_READ ? 4 : 3;
        for (unsigned int i = 0; i < stalled; i++) clock->tick();

        buffer.set(bus->read<CPU>(addressCounter));
        if (addressCounter++ == 0xffff) addressCounter = 0x8000;
        if (--bytesRemainingCounter == 0)
        {
            if (loopFlag) start();
            else if (enableIRQ) cpu->requestIRQ<CPU::IRQType::DMC>(interruptFlag = true);
        }
        loading = false;
    }
    inline std::uint8_t DMC::output() const noexcept
    {
        return outputLevel;
    }
    template<> inline void DMC::set<0x10>(const std::uint8_t v) noexcept
    {
        if (!(enableIRQ = v & (1 << 7))) clearInterrupt();
        loopFlag = v & (1 << 6);
        timer.period = sequenceLookupTable[v & 0x0f] - 1;
    }
    template<> inline void DMC::set<0x11>(const std::uint8_t v) noexcept
    {
        outputLevel = v & 0x7f;
    }
    template<> inline void DMC::set<0x12>(const std::uint8_t v) noexcept
    {
        sampleAddress = 0xc000 + v * 64;
    }
    template<> inline void DMC::set<0x13>(const std::uint8_t v) noexcept
    {
        sampleLength = 1 + v * 16;
    }
    template<> inline void DMC::step<Timer>() noexcept
    {
        if (timer.step())
        {
            if (!silenceFlag)
            {
                if (shiftRegister & 1) outputLevel += (outputLevel <= 125 ? 2 : 0);
                else outputLevel -= (outputLevel >= 2 ? 2 : 0);
                shiftRegister >>= 1;
            }
            if (--bitsRemainingCounter == 0)
            {
                bitsRemainingCounter = 8;
                if (!(silenceFlag = buffer.empty()))
                {
                    shiftRegister = buffer.move();
                    load();
                }
            }
        }
    }
    template<typename Accessor>
    inline void DMC::access(Accessor& accessor) noexcept
    {
        buffer.access(accessor);
        accessor.access(bitsRemainingCounter);
        accessor.access(shiftRegister);
        accessor.access(bytesRemainingCounter);
        accessor.access(addressCounter);
        accessor.access(sampleAddress);
        accessor.access(sampleLength);
        accessor.access(outputLevel);
        accessor.access(loading);
        accessor.access(silenceFlag);
        accessor.access(enableIRQ);
        accessor.access(interruptFlag);
        accessor.access(loopFlag);
        accessor.access(timer.counter);
        accessor.access(timer.period);
    }

    class APUImpl
    {
    public:
        struct FrameCounter
        {
            int counter = 0;
            bool interruptInhibitFlag = false;
            bool stepModeFlag = false;

            std::uint8_t stepMode() const noexcept
            {
                return stepModeFlag ? 5 : 4;
            }
        };

        struct SampleTimer
        {
            double counter = 0.0;
            double period = 0.0;

            bool step() noexcept
            {
                if (counter < 1.0)
                {
                    counter += period;
                    return true;
                }
                else counter -= 1.0;
                return false;
            }
        };
    private:
        double output() const noexcept;
        template<typename Unit> void step() noexcept;
    public:
        void connect(Bus* bus, Clock* clock, CPU* cpu) noexcept;
        void setSampleBuffer(SampleBuffer* sampleBuffer) noexcept;
        template<typename Accessor> void access(Accessor& accessor) noexcept;
        void clear() noexcept;
        void exec() noexcept;

        template<int idx> std::uint8_t get() noexcept;
        template<int idx> void set(std::uint8_t v) noexcept;
    private:
        Pulse pulse1{}, pulse2{};
        Triangle triangle{};
        Noise noise{};
        DMC dmc{};

        FrameCounter frameCounter{};
        SampleTimer sampleTimer{};

        bool interruptFlag = false;
    private:
        Clock* clock = nullptr;
        CPU* cpu = nullptr;
        SampleBuffer* sampleBuffer = nullptr;
    };
    inline double APUImpl::output() const noexcept
    { // reference https://wiki.nesdev.org/w/index.php/APU_Mixer
        return
            0.00752 * (pulse1.output() + pulse2.output()) +
            0.00851 * triangle.output() +
            0.00494 * noise.output() +
            0.00335 * dmc.output();
    }
    template<> inline void APUImpl::step<Timer>() noexcept
    {
        if (clock->getCPUCycles() & 1)
        {
            pulse1.step<Timer>();
            pulse2.step<Timer>();
        }
        triangle.step<Timer>();
        noise.step<Timer>();
        dmc.step<Timer>();
    }
    template<> inline void APUImpl::step<Envelope>() noexcept
    {
        pulse1.step<Envelope>();
        pulse2.step<Envelope>();
        triangle.step<LinearCounter>();
        noise.step<Envelope>();
    }
    template<> inline void APUImpl::step<LengthCounter>() noexcept
    {
        pulse1.step<LengthCounter>();
        pulse2.step<LengthCounter>();
        triangle.step<LengthCounter>();
        noise.step<LengthCounter>();
    }
    template<> inline void APUImpl::step<Sweep>() noexcept
    {
        pulse1.step<Sweep, 1>();
        pulse2.step<Sweep, 2>();
    }
    template<> inline void APUImpl::step<APUImpl::FrameCounter>() noexcept
    {   // reference https://wiki.nesdev.org/w/index.php?title=APU_Frame_Counter
        if (frameCounter.stepMode() == 4)
        {
            if (++frameCounter.counter == (14915 * 2))
            {
                frameCounter.counter = 0;
                if (!frameCounter.interruptInhibitFlag) cpu->requestIRQ<CPU::IRQType::APU>(interruptFlag = true);
            }
            else if (frameCounter.counter == (3728 * 2 + 1) || frameCounter.counter == (11185 * 2 + 1)) step<Envelope>();
            else if (frameCounter.counter == (7456 * 2 + 1))
            {
                step<Envelope>();
                step<LengthCounter>();
                step<Sweep>();
            }
            else if (frameCounter.counter == (14914 * 2) && !frameCounter.interruptInhibitFlag) cpu->requestIRQ<CPU::IRQType::APU>(interruptFlag = true);
            else if (frameCounter.counter == (14914 * 2 + 1))
            {
                if (!frameCounter.interruptInhibitFlag) cpu->requestIRQ<CPU::IRQType::APU>(interruptFlag = true);
                step<Envelope>();
                step<LengthCounter>();
                step<Sweep>();
            }
        }
        else
        {
            if (++frameCounter.counter == (18641 * 2)) frameCounter.counter = 0;
            else if (frameCounter.counter == (3728 * 2 + 1) || frameCounter.counter == (11185 * 2 + 1)) step<Envelope>();
            else if (frameCounter.counter == (7456 * 2 + 1) || frameCounter.counter == (18640 * 2 + 1))
            {
                step<Envelope>();
                step<LengthCounter>();
                step<Sweep>();
            }
        }
    }
    void APUImpl::connect(Bus* const bus, Clock* const clock, CPU* const cpu) noexcept
    {
        this->cpu = cpu;
        this->clock = clock;
        dmc.connect(clock, bus, cpu);
    }
    void APUImpl::setSampleBuffer(SampleBuffer* const sampleBuffer) noexcept
    {
        this->sampleBuffer = sampleBuffer;
        sampleTimer.period = clock->getCPUFrequency() / sampleBuffer->getSampleRate() - 1.0;
    }
    template<typename Accessor>
    inline void APUImpl::access(Accessor& accessor) noexcept
    {
        pulse1.access(accessor);
        pulse2.access(accessor);
        triangle.access(accessor);
        noise.access(accessor);
        dmc.access(accessor);
        accessor.access(frameCounter.stepModeFlag);
        accessor.access(frameCounter.interruptInhibitFlag);
        accessor.access(frameCounter.counter);
        accessor.access(interruptFlag);
    }
    inline void APUImpl::clear() noexcept
    {
        pulse1 = {};
        pulse2 = {};
        triangle = {};
        noise = {};
        frameCounter = {};
        sampleTimer.counter = 0.0;
        interruptFlag = false;
    }
    inline void APUImpl::exec() noexcept
    {
        step<Timer>();
        step<FrameCounter>();
        if (sampleTimer.step()) sampleBuffer->sendSample(output());
    }
    template<> inline std::uint8_t APUImpl::get<0x15>() noexcept
    {
        std::uint8_t ret =
            !pulse1.getLengthCounter().zero() << 0 |
            !pulse2.getLengthCounter().zero() << 1 |
            !triangle.getLengthCounter().zero() << 2 |
            !noise.getLengthCounter().zero() << 3 |
            !dmc.zeroBytesRemaining() << 4 |
            interruptFlag << 6 |
            dmc.checkInterrupt() << 7;

        cpu->requestIRQ<CPU::IRQType::APU>(interruptFlag = false);
        return ret;
    }
    template<> inline void APUImpl::set<0x00>(const std::uint8_t v) noexcept
    {
        pulse1.set<0x00>(v);
    }
    template<> inline void APUImpl::set<0x01>(const std::uint8_t v) noexcept
    {
        pulse1.set<0x01>(v);
    }
    template<> inline void APUImpl::set<0x02>(const std::uint8_t v) noexcept
    {
        pulse1.set<0x02>(v);
    }
    template<> inline void APUImpl::set<0x03>(const std::uint8_t v) noexcept
    {
        pulse1.set<0x03>(v);
    }
    template<> inline void APUImpl::set<0x04>(const std::uint8_t v) noexcept
    {
        pulse2.set<0x00>(v);
    }
    template<> inline void APUImpl::set<0x05>(const std::uint8_t v) noexcept
    {
        pulse2.set<0x01>(v);
    }
    template<> inline void APUImpl::set<0x06>(const std::uint8_t v) noexcept
    {
        pulse2.set<0x02>(v);
    }
    template<> inline void APUImpl::set<0x07>(const std::uint8_t v) noexcept
    {
        pulse2.set<0x03>(v);
    }
    template<> inline void APUImpl::set<0x08>(const std::uint8_t v) noexcept
    {
        triangle.set<0x08>(v);
    }
    template<> inline void APUImpl::set<0x0a>(const std::uint8_t v) noexcept
    {
        triangle.set<0x0a>(v);
    }
    template<> inline void APUImpl::set<0x0b>(const std::uint8_t v) noexcept
    {
        triangle.set<0x0b>(v);
    }
    template<> inline void APUImpl::set<0x0c>(const std::uint8_t v) noexcept
    {
        noise.set<0x0c>(v);
    }
    template<> inline void APUImpl::set<0x0e>(const std::uint8_t v) noexcept
    {
        noise.set<0x0e>(v);
    }
    template<> inline void APUImpl::set<0x0f>(const std::uint8_t v) noexcept
    {
        noise.set<0x0f>(v);
    }
    template<> inline void APUImpl::set<0x10>(const std::uint8_t v) noexcept
    {
        dmc.set<0x10>(v);
    }
    template<> inline void APUImpl::set<0x11>(const std::uint8_t v) noexcept
    {
        dmc.set<0x11>(v);
    }
    template<> inline void APUImpl::set<0x12>(const std::uint8_t v) noexcept
    {
        dmc.set<0x12>(v);
    }
    template<> inline void APUImpl::set<0x13>(const std::uint8_t v) noexcept
    {
        dmc.set<0x13>(v);
    }
    template<> inline void APUImpl::set<0x15>(const std::uint8_t v) noexcept
    {
        if (!pulse1.enable(v & 0x01)) pulse1.getLengthCounter().reset();
        if (!pulse2.enable(v & 0x02)) pulse2.getLengthCounter().reset();
        if (!triangle.enable(v & 0x04)) triangle.getLengthCounter().reset();
        if (!noise.enable(v & 0x08)) noise.getLengthCounter().reset();

        dmc.clearInterrupt();
        if (!(v & 0x10)) dmc.resetBytesRemaining();
        else if (dmc.zeroBytesRemaining())
        {
            dmc.start();
            if(dmc.getBuffer().empty()) dmc.load();
        }
    }
    template<> inline void APUImpl::set<0x17>(const std::uint8_t v) noexcept
    {
        frameCounter.counter = (clock->getCPUCycles() & 1) ? -3 : -4; //After 3 or 4 CPU clock cycles, the timer is reset.
        frameCounter.stepModeFlag = v & (1 << 7);
        //Interrupt inhibit flag. If set, the frame interrupt flag is cleared, otherwise it is unaffected.
        if ((frameCounter.interruptInhibitFlag = v & (1 << 6))) cpu->requestIRQ<CPU::IRQType::APU>(interruptFlag = false);
        if (frameCounter.stepModeFlag)
        {
            step<Envelope>();
            step<LengthCounter>();
            step<Sweep>();
        }
    }
}

struct fcpp::core::APU::APUData
{
    detail::APUImpl impl{};
};

fcpp::core::APU::APU() : dptr(std::make_unique<APUData>()) {}
fcpp::core::APU::~APU() noexcept = default;

void fcpp::core::APU::connect(void* const p) noexcept
{
    auto fptr = static_cast<FC*>(p);
    dptr->impl.connect(fptr->getBus(), fptr->getClock(), fptr->getCPU());
}
void fcpp::core::APU::save(void* const p) noexcept
{
    dptr->impl.access(static_cast<Snapshot*>(p)->getWriter());
}
void fcpp::core::APU::load(void* const p) noexcept
{
    dptr->impl.access(static_cast<Snapshot*>(p)->getReader());
}
void fcpp::core::APU::reset() noexcept
{
    dptr->impl.clear();
}

void fcpp::core::APU::exec() noexcept
{
    dptr->impl.exec();
}

template<int reg>
std::uint8_t fcpp::core::APU::get() noexcept
{
    return dptr->impl.get<reg>();
}
template<int reg>
void fcpp::core::APU::set(const std::uint8_t v) noexcept
{
    dptr->impl.set<reg>(v);
}
void fcpp::core::APU::set(SampleBuffer* const sampleBuffer) noexcept
{
    dptr->impl.setSampleBuffer(sampleBuffer);
}

template std::uint8_t fcpp::core::APU::get<0x15>() noexcept;
template void fcpp::core::APU::set<0x00>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x01>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x02>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x03>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x04>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x05>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x06>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x07>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x08>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x0a>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x0b>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x0c>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x0e>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x0f>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x10>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x11>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x12>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x13>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x15>(const std::uint8_t) noexcept;
template void fcpp::core::APU::set<0x17>(const std::uint8_t) noexcept;
