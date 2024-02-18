#include <cstddef>
#include <cstdint>

#include <SDL.h>

#include "FCPP/Core.hpp"

#include "Emulator.hpp"

namespace fcpp::wasm::detail
{
    class Audio : public fcpp::core::SampleBuffer
    {
    public:
        Audio();
        ~Audio() noexcept override;
    private:
        void sendSample(double sample) noexcept override;
        int getSampleRate() noexcept override;
        void fillBuffer(std::uint8_t* buffer, int len) noexcept;
        static void callback(void* data, std::uint8_t* buffer, int len) noexcept;
    private:
        static constexpr int sampleRate = 44100;
        static constexpr std::size_t buffSize = 512;
        static constexpr std::size_t buffNum = 12;

        int frames = 0;
        std::int16_t samples[buffSize * buffNum]{};
        std::size_t count = 0, writeIdx = 0, readIdx = 0;
        SDL_AudioDeviceID devid = 0;
    };

    Audio::Audio()
    {
        if (SDL_Init(SDL_INIT_AUDIO) != 0)
        {
            SDL_Log("SDL_INIT_AUDIO Error: %s\n", SDL_GetError());
            return;
        }

        SDL_AudioSpec want{};
        SDL_zero(want);
        want.freq = sampleRate;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = buffSize;
        want.callback = callback;
        want.userdata = this;
        devid = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
        if (devid == 0)
        {
            SDL_Log("SDL_OpenAudioDevice Error: %s\n", SDL_GetError());
            return;
        }

        SDL_PauseAudioDevice(devid, 0);
    }
    Audio::~Audio() noexcept
    {
        if (devid != 0)
        {
            SDL_PauseAudioDevice(devid, 1);
            SDL_CloseAudioDevice(devid);
        }
    }
    void Audio::sendSample(const double sample) noexcept
    {
        if (frames < buffNum)
        {
            samples[writeIdx * buffSize + count++] = static_cast<std::int16_t>(sample * 32767);
            if (count >= buffSize)
            {
                count = 0;
                writeIdx = (writeIdx < (buffNum - 1)) ? writeIdx + 1 : 0;
                frames++;
            }
        }
    }
    int Audio::getSampleRate() noexcept
    {
        return sampleRate;
    }
    inline void Audio::fillBuffer(std::uint8_t* const buffer, const int len) noexcept
    {
        if (frames)
        {
            SDL_memcpy(buffer, samples + readIdx * buffSize, len);
            readIdx = (readIdx < (buffNum - 1)) ? readIdx + 1 : 0;
            frames--;
        }
        else SDL_memset(buffer, 0, len);
    }
    void Audio::callback(void* const data, std::uint8_t* const buffer, const int len) noexcept
    {
        static_cast<Audio*>(data)->fillBuffer(buffer, len);
    }

    class Input : public fcpp::core::InputScanner
    {
    public:
        Input() noexcept;
        ~Input() override = default;
    private:
        std::uint8_t scan() noexcept override;
        fcpp::core::JoypadType getJoypadType() noexcept override;
    private:
        const Uint8* keyState;
    };

    Input::Input() noexcept : keyState(SDL_GetKeyboardState(nullptr)) {}
    std::uint8_t Input::scan() noexcept
    {
        return
            keyState[SDL_SCANCODE_A] << fcpp::core::StandardJoypad::Buttons::A |
            keyState[SDL_SCANCODE_S] << fcpp::core::StandardJoypad::Buttons::B |
            keyState[SDL_SCANCODE_Z] << fcpp::core::StandardJoypad::Buttons::SELECT |
            keyState[SDL_SCANCODE_X] << fcpp::core::StandardJoypad::Buttons::START |
            keyState[SDL_SCANCODE_UP] << fcpp::core::StandardJoypad::Buttons::UP |
            keyState[SDL_SCANCODE_DOWN] << fcpp::core::StandardJoypad::Buttons::DOWN |
            keyState[SDL_SCANCODE_LEFT] << fcpp::core::StandardJoypad::Buttons::LEFT |
            keyState[SDL_SCANCODE_RIGHT] << fcpp::core::StandardJoypad::Buttons::RIGHT;
    }
    fcpp::core::JoypadType Input::getJoypadType() noexcept
    {
        return fcpp::core::JoypadType::Standard;
    }

    class Video : public fcpp::core::FrameBuffer
    {
    public:
        Video();
        ~Video() noexcept override;

        void render() noexcept;
        bool isReady() const noexcept;
        bool isStop() const noexcept;
    private:
        void setPixel(int x, int y, std::uint32_t color) noexcept override;
        void completedSignal() noexcept override;
        const std::uint32_t* getPaletteTable() noexcept override;
    private:
        bool ready = false, stop = false;
        std::uint32_t buffer[256 * 240]{};
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
    };

    Video::Video()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            SDL_Log("SDL_INIT_VIDEO Error: %s\n", SDL_GetError());
            return;
        }

        window = SDL_CreateWindow("FCPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256 * 2, 240 * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr)
        {
            SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr)
        {
            SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
            return;
        }
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
        if (texture == nullptr)
        {
            SDL_Log("SDL_CreateTexture Error: %s\n", SDL_GetError());
            return;
        }
    }
    Video::~Video() noexcept
    {
        if (texture != nullptr) SDL_DestroyTexture(texture);
        if (renderer != nullptr) SDL_DestroyRenderer(renderer);
        if (window != nullptr) SDL_DestroyWindow(window);
    }
    inline void Video::render() noexcept
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                stop = true;
                break;
            case SDL_KEYDOWN:
                break;
            }
        }

        if (SDL_UpdateTexture(texture, nullptr, buffer, 256 * sizeof(std::uint32_t)) != 0)
            SDL_Log("SDL_UpdateTexture Error: %s\n", SDL_GetError());

        if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) != 0)
            SDL_Log("SDL_RenderCopy Error: %s\n", SDL_GetError());

        SDL_RenderPresent(renderer);

        ready = false;
    }
    inline bool Video::isReady() const noexcept
    {
        return ready;
    }
    inline bool Video::isStop() const noexcept
    {
        return stop;
    }
    void Video::setPixel(const int x, const int y, const std::uint32_t color) noexcept
    {
        buffer[256 * y + x] = color;
    }
    void Video::completedSignal() noexcept
    {
        ready = true;
    }
    const std::uint32_t* Video::getPaletteTable() noexcept
    {
        return nullptr;
    }
}

struct fcpp::wasm::Emulator::EmulatorData
{
    fcpp::core::FC fc{};
    fcpp::wasm::detail::Audio audio{};
    fcpp::wasm::detail::Input input{};
    fcpp::wasm::detail::Video video{};
};

fcpp::wasm::Emulator::Emulator() : dptr(std::make_unique<EmulatorData>())
{
    dptr->fc.connect(0, &dptr->input);
    dptr->fc.connect(&dptr->video);
    dptr->fc.connect(&dptr->audio);
}

fcpp::wasm::Emulator::~Emulator() noexcept
{
    SDL_Quit();
}

bool fcpp::wasm::Emulator::load(const char* const path) noexcept
{
    if (dptr->fc.insertCartridge(path))
    {
        dptr->fc.powerOn();
        return true;
    }
    return false;
}

bool fcpp::wasm::Emulator::run() noexcept
{
    while (!dptr->video.isReady()) dptr->fc.exec();
    dptr->video.render();
    return !dptr->video.isStop();
}
