#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include <SDL.h>

#include "FCPP/Core.hpp"
#include "FCPP/Util/LoopCounter.hpp"

#include "Emulator.hpp"

#define JOYPAD_IMAGE_PATH(image) (IMAGES_LOAD_PATH "/joypad/" image)

namespace fcpp::wasm::detail
{
    class VirtualJoypad
    {
    public:
        ~VirtualJoypad() noexcept;
        void init(SDL_Renderer* rendere) noexcept;
        int render(SDL_Renderer* renderer) noexcept;
        void toggle() noexcept;
        void updateLayout(int w, int h) noexcept;
        void updateState(SDL_FingerID id, SDL_Point point, bool v) noexcept;
        std::uint8_t state() const noexcept;
    private:
        static constexpr int A = 0;
        static constexpr int B = 1;
        static constexpr int DPAD = 2;
        static constexpr int SELECT = 3;
        static constexpr int START = 4;

        bool enableFlag = true;
        bool errorFlag = false;
        SDL_Texture* textures[5]{};
        SDL_Rect buttonRect[8]{};
        std::uint8_t stateBuffer[8]{};
        std::unordered_map<SDL_FingerID, std::uint8_t*> stateMap{};
    };

    inline VirtualJoypad::~VirtualJoypad() noexcept
    {
        for (int i = 0; i < 5; i++) if (textures[i] != nullptr) SDL_DestroyTexture(textures[i]);
    }
    inline void VirtualJoypad::init(SDL_Renderer* const renderer) noexcept
    {
        SDL_Surface* a = SDL_LoadBMP(JOYPAD_IMAGE_PATH("a.bmp"));
        SDL_Surface* b = SDL_LoadBMP(JOYPAD_IMAGE_PATH("b.bmp"));
        SDL_Surface* dpad = SDL_LoadBMP(JOYPAD_IMAGE_PATH("dpad.bmp"));
        SDL_Surface* select = SDL_LoadBMP(JOYPAD_IMAGE_PATH("select.bmp"));
        SDL_Surface* start = SDL_LoadBMP(JOYPAD_IMAGE_PATH("start.bmp"));

        if (a == nullptr || b == nullptr || dpad == nullptr || select == nullptr || start == nullptr)
        {
            SDL_Log("Failed to load BMP for VirtualJoypad: %s\n", SDL_GetError());
            errorFlag = true;
            return;
        }

        textures[A] = SDL_CreateTextureFromSurface(renderer, a);
        textures[B] = SDL_CreateTextureFromSurface(renderer, b);
        textures[DPAD] = SDL_CreateTextureFromSurface(renderer, dpad);
        textures[SELECT] = SDL_CreateTextureFromSurface(renderer, select);
        textures[START] = SDL_CreateTextureFromSurface(renderer, start);

        if (textures[A] == nullptr || textures[B] == nullptr || textures[DPAD] == nullptr || textures[SELECT] == nullptr || textures[START] == nullptr)
        {
            SDL_Log("Failed to create texture for VirtualJoypad: %s\n", SDL_GetError());
            errorFlag = true;
            return;
        }

        SDL_FreeSurface(start);
        SDL_FreeSurface(select);
        SDL_FreeSurface(dpad);
        SDL_FreeSurface(b);
        SDL_FreeSurface(a);
    }
    inline int VirtualJoypad::render(SDL_Renderer* const renderer) noexcept
    {
        int result = 0;
        if (!errorFlag && enableFlag)
        {

            result |= SDL_RenderCopyEx(renderer, textures[DPAD], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::UP], 0, nullptr, SDL_FLIP_NONE);
            result |= SDL_RenderCopyEx(renderer, textures[DPAD], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::DOWN], 180, nullptr, SDL_FLIP_NONE);
            result |= SDL_RenderCopyEx(renderer, textures[DPAD], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::LEFT], 270, nullptr, SDL_FLIP_NONE);
            result |= SDL_RenderCopyEx(renderer, textures[DPAD], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::RIGHT], 90, nullptr, SDL_FLIP_NONE);

            result |= SDL_RenderCopy(renderer, textures[A], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::A]);
            result |= SDL_RenderCopy(renderer, textures[B], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::B]);
            result |= SDL_RenderCopy(renderer, textures[SELECT], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::SELECT]);
            result |= SDL_RenderCopy(renderer, textures[START], nullptr, &buttonRect[fcpp::core::StandardJoypad::Buttons::START]);
        }
        return result;
    }
    inline void VirtualJoypad::toggle() noexcept
    {
        enableFlag = !enableFlag;
    }
    inline void VirtualJoypad::updateLayout(const int w, const int h) noexcept
    {
        buttonRect[fcpp::core::StandardJoypad::Buttons::A] = { w - w / 16 - 100, h - 300 + 130, 100, 100 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::B] = { w - w / 16 - 100, h - 300 + 10, 100, 100 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::SELECT] = { w / 2 - 110, h - 75, 100, 35 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::START] = { w / 2 + 10, h - 75, 100, 35 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::UP] = { w / 8 + 25, h - 300 + 25, 50, 50 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::DOWN] = { w / 8 + 25, h - 300 + 135, 50, 50 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::LEFT] = { w / 8 - 30, h - 300 + 80, 50, 50 };
        buttonRect[fcpp::core::StandardJoypad::Buttons::RIGHT] = { w / 8 + 80, h - 300 + 80, 50, 50 };
    }
    inline void VirtualJoypad::updateState(const SDL_FingerID id, const SDL_Point point, const bool v) noexcept
    {
        if (!errorFlag)
        {
            if (v)
            {
                if (enableFlag) for (int i = 0; i < 8; i++) if (SDL_PointInRect(&point, &buttonRect[i]))
                {
                    stateBuffer[i] = 1;
                    stateMap[id] = stateBuffer + i;
                }
            }
            else *stateMap[id] = 0;
        }
    }
    inline std::uint8_t VirtualJoypad::state() const noexcept
    {
        return
            stateBuffer[fcpp::core::StandardJoypad::Buttons::A] << fcpp::core::StandardJoypad::Buttons::A |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::B] << fcpp::core::StandardJoypad::Buttons::B |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::SELECT] << fcpp::core::StandardJoypad::Buttons::SELECT |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::START] << fcpp::core::StandardJoypad::Buttons::START |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::UP] << fcpp::core::StandardJoypad::Buttons::UP |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::DOWN] << fcpp::core::StandardJoypad::Buttons::DOWN |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::LEFT] << fcpp::core::StandardJoypad::Buttons::LEFT |
            stateBuffer[fcpp::core::StandardJoypad::Buttons::RIGHT] << fcpp::core::StandardJoypad::Buttons::RIGHT;;
    }

    class Audio : public fcpp::core::SampleBuffer
    {
    public:
        ~Audio() noexcept override;

        void init() noexcept;
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
        std::size_t count = 0;
        fcpp::util::LoopCounter<std::size_t> readIdx{ buffNum - 1 }, writeIdx{ buffNum - 1 };
        SDL_AudioDeviceID devid = 0;
    };

    Audio::~Audio() noexcept
    {
        if (devid != 0)
        {
            SDL_PauseAudioDevice(devid, 1);
            SDL_CloseAudioDevice(devid);
        }
    }
    void Audio::init() noexcept
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
    void Audio::sendSample(const double sample) noexcept
    {
        if (frames < buffNum)
        {
            samples[writeIdx * buffSize + count++] = static_cast<std::int16_t>(sample * 32767);
            if (count >= buffSize)
            {
                count = 0;
                ++writeIdx;
                ++frames;
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
            ++readIdx;
            --frames;
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
        ~Input() override = default;

        void init(const VirtualJoypad* p) noexcept;
    private:
        std::uint8_t scan() noexcept override;
        fcpp::core::JoypadType getJoypadType() noexcept override;
    private:
        const Uint8* keyState = SDL_GetKeyboardState(nullptr);
        const VirtualJoypad* virtualJoypad = nullptr;
    };

    inline void Input::init(const VirtualJoypad* const p) noexcept
    {
        virtualJoypad = p;
    }
    std::uint8_t Input::scan() noexcept
    {
        std::uint8_t mask = 0;
        mask |=
            keyState[SDL_SCANCODE_A] << fcpp::core::StandardJoypad::Buttons::A |
            keyState[SDL_SCANCODE_S] << fcpp::core::StandardJoypad::Buttons::B |
            keyState[SDL_SCANCODE_X] << fcpp::core::StandardJoypad::Buttons::SELECT |
            keyState[SDL_SCANCODE_Z] << fcpp::core::StandardJoypad::Buttons::START |
            keyState[SDL_SCANCODE_UP] << fcpp::core::StandardJoypad::Buttons::UP |
            keyState[SDL_SCANCODE_DOWN] << fcpp::core::StandardJoypad::Buttons::DOWN |
            keyState[SDL_SCANCODE_LEFT] << fcpp::core::StandardJoypad::Buttons::LEFT |
            keyState[SDL_SCANCODE_RIGHT] << fcpp::core::StandardJoypad::Buttons::RIGHT;

        mask |= virtualJoypad->state();

        return mask;
    }
    fcpp::core::JoypadType Input::getJoypadType() noexcept
    {
        return fcpp::core::JoypadType::Standard;
    }

    class Video : public fcpp::core::FrameBuffer
    {
    public:
        ~Video() noexcept override;

        void init(VirtualJoypad* p) noexcept;
        void render() noexcept;
        bool isReady() const noexcept;
        bool isStop() const noexcept;
    private:
        void setPixel(int x, int y, std::uint32_t color) noexcept override;
        void completedSignal() noexcept override;
        const std::uint32_t* getPaletteTable() noexcept override;
        void updateScreenRect(int w, int h) noexcept;
    private:
        bool ready = false, stop = false;
        int width = 256 * 2, height = 240 * 2 + 300;
        std::uint32_t buffer[256 * 240]{};
        SDL_Rect screenRect{};
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        VirtualJoypad* virtualJoypad = nullptr;
    };

    Video::~Video() noexcept
    {
        if (texture != nullptr) SDL_DestroyTexture(texture);
        if (renderer != nullptr) SDL_DestroyRenderer(renderer);
        if (window != nullptr) SDL_DestroyWindow(window);
    }
    inline void Video::init(VirtualJoypad* const p) noexcept
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            SDL_Log("SDL_INIT_VIDEO Error: %s\n", SDL_GetError());
            return;
        }

        window = SDL_CreateWindow("FCPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr)
        {
            SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

        virtualJoypad = p;
        virtualJoypad->init(renderer);
        virtualJoypad->updateLayout(width, height);
        updateScreenRect(width, height);
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
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_F1:
                    virtualJoypad->toggle();
                    break;
                default: break;
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    width = event.window.data1;
                    height = event.window.data2;
                    updateScreenRect(width, height);
                    virtualJoypad->updateLayout(width, height);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                virtualJoypad->updateState(0, { event.button.x, event.button.y }, true);
                break;
            case SDL_MOUSEBUTTONUP:
                virtualJoypad->updateState(0, { event.button.x, event.button.y }, false);
                break;
            case SDL_FINGERDOWN:
                virtualJoypad->updateState(event.tfinger.fingerId, { static_cast<int>(event.tfinger.x * width), static_cast<int>(event.tfinger.y * height) }, true);
                break;
            case SDL_FINGERUP:
                virtualJoypad->updateState(event.tfinger.fingerId, { static_cast<int>(event.tfinger.x * width), static_cast<int>(event.tfinger.y * height) }, false);
                break;
            }
        }

        if (SDL_UpdateTexture(texture, nullptr, buffer, 256 * sizeof(std::uint32_t)) != 0) SDL_Log("SDL_UpdateTexture Error: %s\n", SDL_GetError());
        if (SDL_RenderClear(renderer) != 0) SDL_Log("SDL_RenderClear Error: %s\n", SDL_GetError());
        if (SDL_RenderCopy(renderer, texture, nullptr, &screenRect) != 0) SDL_Log("SDL_RenderCopy Error: %s\n", SDL_GetError());
        if (virtualJoypad->render(renderer) != 0) SDL_Log("VirtualJoypad Render Error: %s\n", SDL_GetError());
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
    inline void Video::updateScreenRect(const int w, const int h) noexcept
    {
        int targetHeight = 240 * w / 256;
        if (h < targetHeight)
        {
            int actualHeight = h - 300;
            int actualWeight = 256 * actualHeight / 240;
            screenRect = { w / 2 - actualWeight / 2, 0, 256 * actualHeight / 240 , actualHeight };
        }
        else screenRect = { 0, 0, w , 240 * w / 256 };
    }
}

struct fcpp::wasm::Emulator::EmulatorData
{
    fcpp::core::FC fc{};
    fcpp::wasm::detail::VirtualJoypad virtualHoypad{};
    fcpp::wasm::detail::Audio audio{};
    fcpp::wasm::detail::Input input{};
    fcpp::wasm::detail::Video video{};
};

fcpp::wasm::Emulator::Emulator() : dptr(std::make_unique<EmulatorData>())
{
    dptr->audio.init();
    dptr->input.init(&dptr->virtualHoypad);
    dptr->video.init(&dptr->virtualHoypad);

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

const char* fcpp::wasm::Emulator::info() const noexcept
{
    const char* info =
        "Version:\n"
        "FCPP: " FCPP_VERSION_STR "\n"
        "EFCPP: " EFCPP_VERSION_STR "\n"
        "Keybord mapping:\n"
        "A(A) B(B) Select(Z) Start(X)\n"
        "Up(Up) Down(Down) Left(Left) Right(Right)\n"
        "F1(Toggle virtual joypad)";
    return info;
}
