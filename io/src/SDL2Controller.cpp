#include <cstring>
#include <string>

#include <SDL.h>

#include "FCPP/IO/Audio.hpp"
#include "FCPP/IO/Input.hpp"
#include "FCPP/IO/Video.hpp"
#include "FCPP/IO/SDL2/SDL2Controller.hpp"

namespace fcpp::io::detail
{
    static constexpr SDL_Scancode keyMap(const Keyboard key) noexcept
    {
        switch (key)
        {
        case Keyboard::A:         return SDL_SCANCODE_A;
        case Keyboard::B:         return SDL_SCANCODE_B;
        case Keyboard::C:         return SDL_SCANCODE_C;
        case Keyboard::D:         return SDL_SCANCODE_D;
        case Keyboard::E:         return SDL_SCANCODE_E;
        case Keyboard::F:         return SDL_SCANCODE_F;
        case Keyboard::G:         return SDL_SCANCODE_G;
        case Keyboard::H:         return SDL_SCANCODE_H;
        case Keyboard::I:         return SDL_SCANCODE_I;
        case Keyboard::J:         return SDL_SCANCODE_J;
        case Keyboard::K:         return SDL_SCANCODE_K;
        case Keyboard::L:         return SDL_SCANCODE_L;
        case Keyboard::M:         return SDL_SCANCODE_M;
        case Keyboard::N:         return SDL_SCANCODE_N;
        case Keyboard::O:         return SDL_SCANCODE_O;
        case Keyboard::P:         return SDL_SCANCODE_P;
        case Keyboard::Q:         return SDL_SCANCODE_Q;
        case Keyboard::R:         return SDL_SCANCODE_R;
        case Keyboard::S:         return SDL_SCANCODE_S;
        case Keyboard::T:         return SDL_SCANCODE_T;
        case Keyboard::U:         return SDL_SCANCODE_U;
        case Keyboard::V:         return SDL_SCANCODE_V;
        case Keyboard::W:         return SDL_SCANCODE_W;
        case Keyboard::X:         return SDL_SCANCODE_X;
        case Keyboard::Y:         return SDL_SCANCODE_Y;
        case Keyboard::Z:         return SDL_SCANCODE_Z;
        case Keyboard::Num0:      return SDL_SCANCODE_0;
        case Keyboard::Num1:      return SDL_SCANCODE_1;
        case Keyboard::Num2:      return SDL_SCANCODE_2;
        case Keyboard::Num3:      return SDL_SCANCODE_3;
        case Keyboard::Num4:      return SDL_SCANCODE_4;
        case Keyboard::Num5:      return SDL_SCANCODE_5;
        case Keyboard::Num6:      return SDL_SCANCODE_6;
        case Keyboard::Num7:      return SDL_SCANCODE_7;
        case Keyboard::Num8:      return SDL_SCANCODE_8;
        case Keyboard::Num9:      return SDL_SCANCODE_9;
        case Keyboard::Escape:    return SDL_SCANCODE_ESCAPE;
        case Keyboard::LControl:  return SDL_SCANCODE_LCTRL;
        case Keyboard::LShift:    return SDL_SCANCODE_LSHIFT;
        case Keyboard::LAlt:      return SDL_SCANCODE_LALT;
        case Keyboard::System:    return SDL_SCANCODE_APPLICATION;
        case Keyboard::RControl:  return SDL_SCANCODE_RCTRL;
        case Keyboard::RShift:    return SDL_SCANCODE_RSHIFT;
        case Keyboard::RAlt:      return SDL_SCANCODE_RALT;
        case Keyboard::Menu:      return SDL_SCANCODE_MENU;
        case Keyboard::LBracket:  return SDL_SCANCODE_LEFTBRACKET;
        case Keyboard::RBracket:  return SDL_SCANCODE_RIGHTBRACKET;
        case Keyboard::Semicolon: return SDL_SCANCODE_SEMICOLON;
        case Keyboard::Comma:     return SDL_SCANCODE_COMMA;
        case Keyboard::Period:    return SDL_SCANCODE_PERIOD;
        case Keyboard::Quote:     return SDL_SCANCODE_APOSTROPHE;
        case Keyboard::Slash:     return SDL_SCANCODE_SLASH;
        case Keyboard::Backslash: return SDL_SCANCODE_BACKSLASH;
        case Keyboard::Tilde:     return SDL_SCANCODE_GRAVE;
        case Keyboard::Equal:     return SDL_SCANCODE_EQUALS;
        case Keyboard::Hyphen:    return SDL_SCANCODE_MINUS;
        case Keyboard::Space:     return SDL_SCANCODE_SPACE;
        case Keyboard::Enter:     return SDL_SCANCODE_RETURN;
        case Keyboard::Backspace: return SDL_SCANCODE_BACKSPACE;
        case Keyboard::Tab:       return SDL_SCANCODE_TAB;
        case Keyboard::PageUp:    return SDL_SCANCODE_PAGEUP;
        case Keyboard::PageDown:  return SDL_SCANCODE_PAGEDOWN;
        case Keyboard::End:       return SDL_SCANCODE_END;
        case Keyboard::Home:      return SDL_SCANCODE_HOME;
        case Keyboard::Insert:    return SDL_SCANCODE_INSERT;
        case Keyboard::Delete:    return SDL_SCANCODE_DELETE;
        case Keyboard::Add:       return SDL_SCANCODE_KP_PLUS;
        case Keyboard::Subtract:  return SDL_SCANCODE_KP_MINUS;
        case Keyboard::Multiply:  return SDL_SCANCODE_KP_MULTIPLY;
        case Keyboard::Divide:    return SDL_SCANCODE_KP_DIVIDE;
        case Keyboard::Left:      return SDL_SCANCODE_LEFT;
        case Keyboard::Right:     return SDL_SCANCODE_RIGHT;
        case Keyboard::Up:        return SDL_SCANCODE_UP;
        case Keyboard::Down:      return SDL_SCANCODE_DOWN;
        case Keyboard::Numpad0:   return SDL_SCANCODE_KP_0;
        case Keyboard::Numpad1:   return SDL_SCANCODE_KP_1;
        case Keyboard::Numpad2:   return SDL_SCANCODE_KP_2;
        case Keyboard::Numpad3:   return SDL_SCANCODE_KP_3;
        case Keyboard::Numpad4:   return SDL_SCANCODE_KP_4;
        case Keyboard::Numpad5:   return SDL_SCANCODE_KP_5;
        case Keyboard::Numpad6:   return SDL_SCANCODE_KP_6;
        case Keyboard::Numpad7:   return SDL_SCANCODE_KP_7;
        case Keyboard::Numpad8:   return SDL_SCANCODE_KP_8;
        case Keyboard::Numpad9:   return SDL_SCANCODE_KP_9;
        case Keyboard::F1:        return SDL_SCANCODE_F1;
        case Keyboard::F2:        return SDL_SCANCODE_F2;
        case Keyboard::F3:        return SDL_SCANCODE_F3;
        case Keyboard::F4:        return SDL_SCANCODE_F4;
        case Keyboard::F5:        return SDL_SCANCODE_F5;
        case Keyboard::F6:        return SDL_SCANCODE_F6;
        case Keyboard::F7:        return SDL_SCANCODE_F7;
        case Keyboard::F8:        return SDL_SCANCODE_F8;
        case Keyboard::F9:        return SDL_SCANCODE_F9;
        case Keyboard::F10:       return SDL_SCANCODE_F10;
        case Keyboard::F11:       return SDL_SCANCODE_F11;
        case Keyboard::F12:       return SDL_SCANCODE_F12;
        case Keyboard::Pause:     return SDL_SCANCODE_PAUSE;
        default:                  return SDL_SCANCODE_UNKNOWN;
        }
    }
    static constexpr Keyboard keyMap(const SDL_Scancode key) noexcept
    {
        switch (key)
        {
        case SDL_SCANCODE_A:            return Keyboard::A;
        case SDL_SCANCODE_B:            return Keyboard::B;
        case SDL_SCANCODE_C:            return Keyboard::C;
        case SDL_SCANCODE_D:            return Keyboard::D;
        case SDL_SCANCODE_E:            return Keyboard::E;
        case SDL_SCANCODE_F:            return Keyboard::F;
        case SDL_SCANCODE_G:            return Keyboard::G;
        case SDL_SCANCODE_H:            return Keyboard::H;
        case SDL_SCANCODE_I:            return Keyboard::I;
        case SDL_SCANCODE_J:            return Keyboard::J;
        case SDL_SCANCODE_K:            return Keyboard::K;
        case SDL_SCANCODE_L:            return Keyboard::L;
        case SDL_SCANCODE_M:            return Keyboard::M;
        case SDL_SCANCODE_N:            return Keyboard::N;
        case SDL_SCANCODE_O:            return Keyboard::O;
        case SDL_SCANCODE_P:            return Keyboard::P;
        case SDL_SCANCODE_Q:            return Keyboard::Q;
        case SDL_SCANCODE_R:            return Keyboard::R;
        case SDL_SCANCODE_S:            return Keyboard::S;
        case SDL_SCANCODE_T:            return Keyboard::T;
        case SDL_SCANCODE_U:            return Keyboard::U;
        case SDL_SCANCODE_V:            return Keyboard::V;
        case SDL_SCANCODE_W:            return Keyboard::W;
        case SDL_SCANCODE_X:            return Keyboard::X;
        case SDL_SCANCODE_Y:            return Keyboard::Y;
        case SDL_SCANCODE_Z:            return Keyboard::Z;
        case SDL_SCANCODE_0:            return Keyboard::Num0;
        case SDL_SCANCODE_1:            return Keyboard::Num1;
        case SDL_SCANCODE_2:            return Keyboard::Num2;
        case SDL_SCANCODE_3:            return Keyboard::Num3;
        case SDL_SCANCODE_4:            return Keyboard::Num4;
        case SDL_SCANCODE_5:            return Keyboard::Num5;
        case SDL_SCANCODE_6:            return Keyboard::Num6;
        case SDL_SCANCODE_7:            return Keyboard::Num7;
        case SDL_SCANCODE_8:            return Keyboard::Num8;
        case SDL_SCANCODE_9:            return Keyboard::Num9;
        case SDL_SCANCODE_ESCAPE:       return Keyboard::Escape;
        case SDL_SCANCODE_LCTRL:        return Keyboard::LControl;
        case SDL_SCANCODE_LSHIFT:       return Keyboard::LShift;
        case SDL_SCANCODE_LALT:         return Keyboard::LAlt;
        case SDL_SCANCODE_APPLICATION:  return Keyboard::System;
        case SDL_SCANCODE_RCTRL:        return Keyboard::RControl;
        case SDL_SCANCODE_RSHIFT:       return Keyboard::RShift;
        case SDL_SCANCODE_RALT:         return Keyboard::RAlt;
        case SDL_SCANCODE_MENU:         return Keyboard::Menu;
        case SDL_SCANCODE_LEFTBRACKET:  return Keyboard::LBracket;
        case SDL_SCANCODE_RIGHTBRACKET: return Keyboard::RBracket;
        case SDL_SCANCODE_SEMICOLON:    return Keyboard::Semicolon;
        case SDL_SCANCODE_COMMA:        return Keyboard::Comma;
        case SDL_SCANCODE_PERIOD:       return Keyboard::Period;
        case SDL_SCANCODE_APOSTROPHE:   return Keyboard::Quote;
        case SDL_SCANCODE_SLASH:        return Keyboard::Slash;
        case SDL_SCANCODE_BACKSLASH:    return Keyboard::Backslash;
        case SDL_SCANCODE_GRAVE:        return Keyboard::Tilde;
        case SDL_SCANCODE_EQUALS:       return Keyboard::Equal;
        case SDL_SCANCODE_MINUS:        return Keyboard::Hyphen;
        case SDL_SCANCODE_SPACE:        return Keyboard::Space;
        case SDL_SCANCODE_RETURN:       return Keyboard::Enter;
        case SDL_SCANCODE_BACKSPACE:    return Keyboard::Backspace;
        case SDL_SCANCODE_TAB:          return Keyboard::Tab;
        case SDL_SCANCODE_PAGEUP:       return Keyboard::PageUp;
        case SDL_SCANCODE_PAGEDOWN:     return Keyboard::PageDown;
        case SDL_SCANCODE_END:          return Keyboard::End;
        case SDL_SCANCODE_HOME:         return Keyboard::Home;
        case SDL_SCANCODE_INSERT:       return Keyboard::Insert;
        case SDL_SCANCODE_DELETE:       return Keyboard::Delete;
        case SDL_SCANCODE_KP_PLUS:      return Keyboard::Add;
        case SDL_SCANCODE_KP_MINUS:     return Keyboard::Subtract;
        case SDL_SCANCODE_KP_MULTIPLY:  return Keyboard::Multiply;
        case SDL_SCANCODE_KP_DIVIDE:    return Keyboard::Divide;
        case SDL_SCANCODE_LEFT:         return Keyboard::Left;
        case SDL_SCANCODE_RIGHT:        return Keyboard::Right;
        case SDL_SCANCODE_UP:           return Keyboard::Up;
        case SDL_SCANCODE_DOWN:         return Keyboard::Down;
        case SDL_SCANCODE_KP_0:         return Keyboard::Numpad0;
        case SDL_SCANCODE_KP_1:         return Keyboard::Numpad1;
        case SDL_SCANCODE_KP_2:         return Keyboard::Numpad2;
        case SDL_SCANCODE_KP_3:         return Keyboard::Numpad3;
        case SDL_SCANCODE_KP_4:         return Keyboard::Numpad4;
        case SDL_SCANCODE_KP_5:         return Keyboard::Numpad5;
        case SDL_SCANCODE_KP_6:         return Keyboard::Numpad6;
        case SDL_SCANCODE_KP_7:         return Keyboard::Numpad7;
        case SDL_SCANCODE_KP_8:         return Keyboard::Numpad8;
        case SDL_SCANCODE_KP_9:         return Keyboard::Numpad9;
        case SDL_SCANCODE_F1:           return Keyboard::F1;
        case SDL_SCANCODE_F2:           return Keyboard::F2;
        case SDL_SCANCODE_F3:           return Keyboard::F3;
        case SDL_SCANCODE_F4:           return Keyboard::F4;
        case SDL_SCANCODE_F5:           return Keyboard::F5;
        case SDL_SCANCODE_F6:           return Keyboard::F6;
        case SDL_SCANCODE_F7:           return Keyboard::F7;
        case SDL_SCANCODE_F8:           return Keyboard::F8;
        case SDL_SCANCODE_F9:           return Keyboard::F9;
        case SDL_SCANCODE_F10:          return Keyboard::F10;
        case SDL_SCANCODE_F11:          return Keyboard::F11;
        case SDL_SCANCODE_F12:          return Keyboard::F12;
        case SDL_SCANCODE_PAUSE:        return Keyboard::Pause;
        default:                        return Keyboard::Unknown;
        }
    }

    class SDL2Manager
    {
    public:
        static void ref();
        static void unref();
    protected:
        SDL2Manager() noexcept;
        SDL2Manager(const SDL2Manager&) noexcept;
        SDL2Manager(SDL2Manager&&) noexcept;
        ~SDL2Manager() noexcept;
    private:
        static unsigned long long int SDL2ObjectCount;
    };
    unsigned long long int SDL2Manager::SDL2ObjectCount = 0;
    void SDL2Manager::ref()
    {
        if (SDL2ObjectCount++ == 0)
        {
            if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
            SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
        }
    }
    void SDL2Manager::unref()
    {
        if (--SDL2ObjectCount == 0) SDL_Quit();
    }
    SDL2Manager::SDL2Manager() noexcept
    {
        ref();
    }
    SDL2Manager::SDL2Manager(const SDL2Manager&) noexcept : SDL2Manager() {}
    SDL2Manager::SDL2Manager(SDL2Manager&&) noexcept : SDL2Manager() {}
    SDL2Manager::~SDL2Manager() noexcept
    {
        unref();
    }

    class SDL2Video :
        public Video,
        private SDL2Manager
    {
    public:
        SDL2Video() = default;
        ~SDL2Video() noexcept override;

        bool create() noexcept;
        void render() noexcept;
        bool setFullScreen(bool enable) noexcept;
        bool setBorderless(bool enable) noexcept;
        bool setVerticalSync(bool enable) noexcept;
        bool setRenderDriver(int idx) noexcept;
        void setScale(double factor) noexcept;
        void setTitle(const char* text) noexcept;
        void setFrameBufferData(const std::uint8_t* data) noexcept;
        void getFrameBufferData(std::uint8_t* data) const noexcept;
    private:
        void setPixel(int x, int y, std::uint32_t color) noexcept override;
        void completedSignal() noexcept override;
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;

        bool vsync = false;
        int width = 256, height = 240;
        int renderDriverIdx = -1;
        std::uint32_t windowsMode = 0;
        std::string title{ "FCPP SDL2 Renderer" };
        std::uint32_t frameBuffer[256 * 240]{};
    };
    SDL2Video::~SDL2Video() noexcept
    {
        if (texture != nullptr) SDL_DestroyTexture(texture);
        if (renderer != nullptr) SDL_DestroyRenderer(renderer);
        if (window != nullptr) SDL_DestroyWindow(window);
    }
    bool SDL2Video::create() noexcept
    {
        if (window == nullptr)
        {
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | windowsMode);
            if (window == nullptr)
            {
                SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
                return false;
            }
        }
        if (renderer == nullptr)
        {
            renderer = SDL_CreateRenderer(window, renderDriverIdx, SDL_RENDERER_ACCELERATED | (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
            if (renderer == nullptr)
            {
                SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
                return false;
            }
        }
        if (texture == nullptr)
        {
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
            if (texture == nullptr)
            {
                SDL_Log("SDL_CreateTexture Error: %s\n", SDL_GetError());
                return false;
            }
        }

        return true;
    }
    void SDL2Video::render() noexcept
    {
        SDL_Event event{};
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                if (closeCallback) closeCallback();
                break;
            case SDL_KEYDOWN:
                if (keyPressCallback) keyPressCallback(keyMap(event.key.keysym.scancode));
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    width = event.window.data1;
                    height = event.window.data2;
                }
                break;
            }
        }

        if (renderCallback) renderCallback();

        if (SDL_UpdateTexture(texture, nullptr, frameBuffer, 256 * sizeof(std::uint32_t)) != 0)
            SDL_Log("SDL_UpdateTexture Error: %s\n", SDL_GetError());

        if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) != 0)
            SDL_Log("SDL_RenderCopy Error: %s\n", SDL_GetError());

        SDL_RenderPresent(renderer);

        fpsLimiter.wait();
    }
    bool SDL2Video::setFullScreen(const bool enable) noexcept
    {
        windowsMode = enable ? SDL_WINDOW_FULLSCREEN : 0;
        return window != nullptr ? (SDL_ShowCursor(enable ? SDL_DISABLE : SDL_ENABLE),
            SDL_SetWindowFullscreen(window, windowsMode) == 0) : true;
    }
    bool SDL2Video::setBorderless(const bool enable) noexcept
    {
        windowsMode = enable ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
        return window != nullptr ? SDL_SetWindowFullscreen(window, windowsMode) == 0 : true;
    }
    bool SDL2Video::setVerticalSync(const bool enable) noexcept
    {
        if (vsync != enable)
        {
            vsync = enable;
            if (renderer != nullptr && texture != nullptr)
            {
#if SDL_PATCHLEVEL >= 18
                return SDL_RenderSetVSync(renderer, enable) == 0;
#else
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                renderer = SDL_CreateRenderer(window, renderDriverIdx, SDL_RENDERER_ACCELERATED | (enable ? SDL_RENDERER_PRESENTVSYNC : 0));
                if (renderer == nullptr)
                {
                    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
                    return false;
                }
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
                if (texture == nullptr)
                {
                    SDL_Log("SDL_CreateTexture Error: %s\n", SDL_GetError());
                    return false;
                }
#endif
            }
        }
        return true;
    }
    bool SDL2Video::setRenderDriver(int idx) noexcept
    {
        renderDriverIdx = idx;
        if (renderer != nullptr && texture != nullptr)
        {
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            renderer = SDL_CreateRenderer(window, idx, SDL_RENDERER_ACCELERATED | (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
            if (renderer == nullptr)
            {
                SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
                return false;
            }
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);
            if (texture == nullptr)
            {
                SDL_Log("SDL_CreateTexture Error: %s\n", SDL_GetError());
                return false;
            }
        }
        return true;
    }
    void SDL2Video::setScale(const double factor) noexcept
    {
        width = static_cast<int>(256.0 * factor);
        height = static_cast<int>(240.0 * factor);
        if (window != nullptr)
        {
            SDL_SetWindowSize(window, width, height);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        }
    }
    void SDL2Video::setTitle(const char* const text) noexcept
    {
        title = text;
        SDL_SetWindowTitle(window, text);
    }
    void SDL2Video::setFrameBufferData(const std::uint8_t* const data) noexcept
    {
        if (data != nullptr) std::memcpy(frameBuffer, data, Controller::FrameBufferSize);
    }
    void SDL2Video::getFrameBufferData(std::uint8_t* const data) const noexcept
    {
        if (data != nullptr) std::memcpy(data, frameBuffer, Controller::FrameBufferSize);
    }
    void SDL2Video::setPixel(const int x, const int y, const std::uint32_t color) noexcept
    {
        frameBuffer[static_cast<std::size_t>(256) * y + x] = color;
    }
    void SDL2Video::completedSignal() noexcept
    {
        if (frameCompletedCallback) frameCompletedCallback();
        else render();
    }

    class SDL2Input :
        public Input,
        private SDL2Manager
    {
    public:
        SDL2Input() = default;
        ~SDL2Input() noexcept override;

        void create() noexcept;
        bool setJoystickPort(int port) noexcept;
        void bind(int standardButtonIdx, Keyboard key) noexcept;
        void bind(int standardButtonIdx, Joystick button) noexcept;
    private:
        std::uint8_t scan() noexcept override;
    private:
        std::uint8_t joystickState(Joystick button) noexcept;
    private:
        SDL_GameController* gameController = nullptr;
        const std::uint8_t* keyState = nullptr;

        int joystickIdx = 0;
        SDL_Scancode keys[10] = {
            keyMap(Keyboard::A),
            keyMap(Keyboard::S),
            keyMap(Keyboard::X),
            keyMap(Keyboard::Z) ,
            keyMap(Keyboard::Up),
            keyMap(Keyboard::Down),
            keyMap(Keyboard::Left),
            keyMap(Keyboard::Right),
            keyMap(Keyboard::Q),
            keyMap(Keyboard::W)
        };
        Joystick buttons[10] = {
            Joystick::Button0,
            Joystick::Button1,
            Joystick::Button2,
            Joystick::Button7,
            Joystick::Up,
            Joystick::Down,
            Joystick::Left,
            Joystick::Right,
            Joystick::Button4,
            Joystick::Button5
        };
    };
    SDL2Input::~SDL2Input() noexcept
    {
        if (gameController != nullptr) SDL_GameControllerClose(gameController);
    }
    void SDL2Input::create() noexcept
    {
        if (gameController == nullptr)
        {
            gameController = SDL_GameControllerOpen(joystickIdx);
            if (gameController == nullptr) SDL_Log("SDL_GameControllerOpen: %s\n", SDL_GetError());
        }

        keyState = SDL_GetKeyboardState(nullptr);
    }
    bool SDL2Input::setJoystickPort(const int port) noexcept
    {
        joystickIdx = port;
        if (gameController != nullptr)
        {
            SDL_GameControllerClose(gameController);
            gameController = SDL_GameControllerOpen(port);
            if (gameController == nullptr)
            {
                SDL_Log("SDL_GameControllerOpen: %s\n", SDL_GetError());
                return false;
            }
        }
        return true;
    }
    void SDL2Input::bind(const int standardButtonIdx, const Keyboard key) noexcept
    {
        keys[standardButtonIdx] = keyMap(key);
    }
    void SDL2Input::bind(const int standardButtonIdx, const Joystick button) noexcept
    {
        buttons[standardButtonIdx] = button;
    }
    std::uint8_t SDL2Input::scan() noexcept
    {
        std::uint8_t mask = 0;

        switch (joypadType)
        {
        case fcpp::core::JoypadType::Standard:
            turbo++;

            mask |=
                keyState[keys[7]] << 7 |
                keyState[keys[6]] << 6 |
                keyState[keys[5]] << 5 |
                keyState[keys[4]] << 4 |
                keyState[keys[3]] << 3 |
                keyState[keys[2]] << 2 |
                (keyState[keys[9]] ? ((turbo >> turboShift) & 1) : keyState[keys[1]]) << 1 | //B
                (keyState[keys[8]] ? ((turbo >> turboShift) & 1) : keyState[keys[0]]) << 0;  //A

            if (gameController)
            {
                mask |=
                    joystickState(buttons[7]) << 7 |
                    joystickState(buttons[6]) << 6 |
                    joystickState(buttons[5]) << 5 |
                    joystickState(buttons[4]) << 4 |
                    joystickState(buttons[3]) << 3 |
                    joystickState(buttons[2]) << 2 |
                    (joystickState(buttons[9]) ? ((turbo >> turboShift) & 1) : joystickState(buttons[1])) << 1 |
                    (joystickState(buttons[8]) ? ((turbo >> turboShift) & 1) : joystickState(buttons[0])) << 0;
            }
            break;
        }

        return mask;
    }
    std::uint8_t SDL2Input::joystickState(const Joystick button) noexcept
    {
        switch (button)
        {
        case Joystick::Button0:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A);
        case Joystick::Button1:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_B);
        case Joystick::Button2:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_X);
        case Joystick::Button3:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_Y);
        case Joystick::Button4:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        case Joystick::Button5:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        case Joystick::Button6:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_BACK);
        case Joystick::Button7:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_START);
        case Joystick::Button8:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
        case Joystick::Button9:  return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
        case Joystick::Button10: return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_GUIDE);
        case Joystick::Up:       return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
        case Joystick::Down:     return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        case Joystick::Left:     return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        case Joystick::Right:    return SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        case Joystick::Axis0P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX) > +16383;
        case Joystick::Axis0N:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX) < -16383;
        case Joystick::Axis1P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY) > +16383;
        case Joystick::Axis1N:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY) < -16383;
        case Joystick::Axis2P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX) > +16383;
        case Joystick::Axis2N:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX) < -16383;
        case Joystick::Axis3P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY) > +16383;
        case Joystick::Axis3N:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY) < -16383;
        case Joystick::Axis4P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > +16383;
        case Joystick::Axis5P:   return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > +16383;
        default:                 return 0;
        }
    }

    class SDL2Audio :
        public Audio,
        private SDL2Manager
    {
    public:
        SDL2Audio() = default;
        ~SDL2Audio() noexcept override;

        bool create() noexcept;
        void setVolume(double v) noexcept;
    private:
        void sendSample(double sample) noexcept override;
    private:
        void fillBuffer(std::uint8_t* buffer, int len) noexcept;
        static void callback(void* data, std::uint8_t* buffer, int len) noexcept;
    private:
        static constexpr std::size_t buffSize = 512;
        static constexpr std::size_t buffNum = 12;
        SDL_semaphore* sem = nullptr;
        SDL_AudioDeviceID devid = 0;

        double volume = 1.0;
        std::size_t count = 0, writeIdx = 0, readIdx = 0;
        std::int16_t samples[buffSize * buffNum]{};
    };
    SDL2Audio::~SDL2Audio() noexcept
    {
        if (devid != 0)
        {
            SDL_PauseAudioDevice(devid, 1);
            SDL_CloseAudioDevice(devid);
        }
        if (sem != nullptr) SDL_DestroySemaphore(sem);
    }
    bool SDL2Audio::create() noexcept
    {
        if (sem == nullptr)
        {
            sem = SDL_CreateSemaphore(buffNum - 1);
            if (sem == nullptr)
            {
                SDL_Log("SDL_CreateSemaphore Error: %s\n", SDL_GetError());
                return false;
            }
        }

        if (devid == 0)
        {
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
                return false;
            }

            SDL_PauseAudioDevice(devid, 0);
        }

        return true;
    }
    void SDL2Audio::setVolume(const double v) noexcept
    {
        volume = v < 0.0 ? 0.0 : (100.0f < v ? 1.0 : v / 100.0);
    }
    void SDL2Audio::sendSample(const double sample) noexcept
    {
        samples[writeIdx * buffSize + count++] = static_cast<std::int16_t>(sample * 32767 * volume);
        if (count >= buffSize)
        {
            count = 0;
            writeIdx = (writeIdx < (buffNum - 1)) ? writeIdx + 1 : 0;
            SDL_SemWait(sem);
        }
    }
    void SDL2Audio::fillBuffer(std::uint8_t* const buffer, const int len) noexcept
    {
        if (SDL_SemValue(sem) < buffNum - 1)
        {
            SDL_memcpy(buffer, samples + readIdx * buffSize, len);
            readIdx = (readIdx < (buffNum - 1)) ? readIdx + 1 : 0;
            SDL_SemPost(sem);
        }
        else SDL_memset(buffer, 0, len);
    }
    void SDL2Audio::callback(void* const data, std::uint8_t* const buffer, const int len) noexcept
    {
        static_cast<SDL2Audio*>(data)->fillBuffer(buffer, len);
    }
}

struct fcpp::io::SDL2Controller::SDL2ControllerData
{
    detail::SDL2Video video{};
    detail::SDL2Input input[2]{};
    detail::SDL2Audio audio{};
};

fcpp::io::SDL2Controller::SDL2Controller() : dptr(std::make_unique<SDL2ControllerData>()) {}
fcpp::io::SDL2Controller::~SDL2Controller() noexcept = default;

bool fcpp::io::SDL2Controller::create() noexcept
{
    for (auto&& input : dptr->input) input.create();
    return dptr->audio.create() && dptr->video.create();
}
fcpp::core::FrameBuffer* fcpp::io::SDL2Controller::getFrameBuffer() noexcept
{
    return &dptr->video;
}
fcpp::core::InputScanner* fcpp::io::SDL2Controller::getInputScanner(const int idx) noexcept
{
    return &dptr->input[idx];
}
fcpp::core::SampleBuffer* fcpp::io::SDL2Controller::getSampleBuffer() noexcept
{
    return &dptr->audio;
}
void fcpp::io::SDL2Controller::render() noexcept
{
    dptr->video.render();
}
bool fcpp::io::SDL2Controller::setFullScreen(const bool enable) noexcept
{
    return dptr->video.setFullScreen(enable);
}
bool fcpp::io::SDL2Controller::setBorderless(const bool enable) noexcept
{
    return dptr->video.setBorderless(enable);
}
bool fcpp::io::SDL2Controller::setVerticalSync(const bool enable) noexcept
{
    return dptr->video.setVerticalSync(enable);
}
bool fcpp::io::SDL2Controller::setRenderDriver(const int idx) noexcept
{
    return dptr->video.setRenderDriver(idx);
}
bool fcpp::io::SDL2Controller::setJoystickPort(const int idx, const int port) noexcept
{
    return dptr->input[idx].setJoystickPort(port);
}
void fcpp::io::SDL2Controller::setScale(const float factor) noexcept
{
    dptr->video.setScale(factor);
}
void fcpp::io::SDL2Controller::setTitle(const char* const text) noexcept
{
    dptr->video.setTitle(text);
}
void fcpp::io::SDL2Controller::setFPSLimit(const double fps) noexcept
{
    dptr->video.setFPSLimit(fps);
}
void fcpp::io::SDL2Controller::setVolume(const float volume) noexcept
{
    dptr->audio.setVolume(volume);
}
void fcpp::io::SDL2Controller::setSampleRate(const int rate) noexcept
{
    dptr->audio.setSampleRate(rate);
}
void fcpp::io::SDL2Controller::setJoypadType(const int idx, const fcpp::core::JoypadType type) noexcept
{
    dptr->input[idx].setJoypadType(type);
}
void fcpp::io::SDL2Controller::setPaletteTable(const PaletteTable & paletteTable) noexcept
{
    dptr->video.setPaletteTable(paletteTable);
}
void fcpp::io::SDL2Controller::setPaletteTable(PaletteTable && paletteTable) noexcept
{
    dptr->video.setPaletteTable(std::move(paletteTable));
}
void fcpp::io::SDL2Controller::setTurboButtonSpeed(const int idx, const std::uint8_t v) noexcept
{
    dptr->input[idx].setTurboSpeed(v);
}
void fcpp::io::SDL2Controller::setFrameBufferData(const std::uint8_t* const argb) noexcept
{
    dptr->video.setFrameBufferData(argb);
}
void fcpp::io::SDL2Controller::getFrameBufferData(std::uint8_t* const argb) const noexcept
{
    dptr->video.getFrameBufferData(argb);
}
void fcpp::io::SDL2Controller::bind(const int idx, const int standardButtonIdx, const Keyboard key) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, key);
}
void fcpp::io::SDL2Controller::bind(const int idx, const int standardButtonIdx, const Joystick button) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, button);
}
void fcpp::io::SDL2Controller::setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept
{
    dptr->video.setKeyPressCallback(callback);
}
void fcpp::io::SDL2Controller::setFrameCompleteCallback(std::function<void()> callback) noexcept
{
    dptr->video.setFrameCompleteCallback(callback);
}
void fcpp::io::SDL2Controller::setRenderCallback(std::function<void()> callback) noexcept
{
    dptr->video.setRenderCallback(callback);
}
void fcpp::io::SDL2Controller::setCloseCallback(std::function<void()> callback) noexcept
{
    dptr->video.setCloseCallback(callback);
}

int fcpp::io::SDL2Controller::Info::getRenderDriverCount() noexcept
{
    return SDL_GetNumRenderDrivers();
}
const char* fcpp::io::SDL2Controller::Info::getRenderDriverName(const int idx) noexcept
{
    SDL_RendererInfo rendererInfo = {};
    if (SDL_GetRenderDriverInfo(idx, &rendererInfo) != 0)
    {
        SDL_Log("SDL_GetRenderDriverInfo Error: %s\n", SDL_GetError());
        return "Unknown Driver";
    }
    return rendererInfo.name;
}

fcpp::io::SDL2Controller::JoystickHelper::JoystickHelper() noexcept
{
    detail::SDL2Manager::ref();
    auto len = SDL2Controller::JoystickHelper::joystickCount();
    data = SDL_malloc(len * sizeof(SDL_GameController*));
    for (int i = 0; i < len; i++) static_cast<SDL_GameController**>(data)[i] = SDL_GameControllerOpen(i);
}
fcpp::io::SDL2Controller::JoystickHelper::~JoystickHelper() noexcept
{
    auto len = SDL2Controller::JoystickHelper::joystickCount();
    for (int i = 0; i < len; i++)
    {
        auto gameController = static_cast<SDL_GameController**>(data)[i];
        if (gameController != nullptr) SDL_GameControllerClose(gameController);
    }
    SDL_free(data);
    detail::SDL2Manager::unref();
}
bool fcpp::io::SDL2Controller::JoystickHelper::joystickPressed(const int port, fcpp::io::Joystick& buttons) noexcept
{
    bool ret = false;
    buttons = Joystick::Unknown;
    if (port >= 0 && port < joystickCount())
    {
        auto gameController = static_cast<SDL_GameController**>(data)[port];
        if (gameController != nullptr)
        {
            auto joystick = SDL_GameControllerGetJoystick(gameController);
            int id = SDL_JoystickInstanceID(joystick);
            SDL_Event event{};
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_CONTROLLERBUTTONDOWN:
                    if (id == event.cbutton.which)
                        switch (event.cbutton.button)
                        {
                        case SDL_CONTROLLER_BUTTON_A:            buttons = Joystick::Button0;  break;
                        case SDL_CONTROLLER_BUTTON_B:            buttons = Joystick::Button1;  break;
                        case SDL_CONTROLLER_BUTTON_X:            buttons = Joystick::Button2;  break;
                        case SDL_CONTROLLER_BUTTON_Y:            buttons = Joystick::Button3;  break;
                        case SDL_CONTROLLER_BUTTON_BACK:         buttons = Joystick::Button6;  break;
                        case SDL_CONTROLLER_BUTTON_GUIDE:        buttons = Joystick::Button10; break;
                        case SDL_CONTROLLER_BUTTON_START:        buttons = Joystick::Button7;  break;
                        case SDL_CONTROLLER_BUTTON_LEFTSTICK:    buttons = Joystick::Button8;  break;
                        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:   buttons = Joystick::Button9;  break;
                        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: buttons = Joystick::Button4;  break;
                        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:buttons = Joystick::Button5;  break;
                        case SDL_CONTROLLER_BUTTON_DPAD_UP:      buttons = Joystick::Up;       break;
                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:    buttons = Joystick::Down;     break;
                        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:    buttons = Joystick::Left;     break;
                        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:   buttons = Joystick::Right;    break;
                        }
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    if (id == event.caxis.which)
                        switch (event.caxis.axis)
                        {
                        case SDL_CONTROLLER_AXIS_LEFTX:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis0P;
                            else if (event.caxis.value < -16383)  buttons = Joystick::Axis0N;
                            break;
                        case SDL_CONTROLLER_AXIS_LEFTY:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis1P;
                            else if (event.caxis.value < -16383)  buttons = Joystick::Axis1N;
                            break;
                        case SDL_CONTROLLER_AXIS_RIGHTX:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis2P;
                            else if (event.caxis.value < -16383)  buttons = Joystick::Axis2N;
                            break;
                        case SDL_CONTROLLER_AXIS_RIGHTY:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis3P;
                            else if (event.caxis.value < -16383)  buttons = Joystick::Axis3N;
                            break;
                        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis4P;
                            break;
                        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                            if (event.caxis.value > +16383) buttons = Joystick::Axis5P;
                            break;
                        }
                    break;
                }
            }
            ret = buttons != Joystick::Unknown;
        }
    }
    return ret;
}
int fcpp::io::SDL2Controller::JoystickHelper::joystickCount() noexcept
{
    auto len = SDL_NumJoysticks();
    return len > 4 ? 4 : len;
}
