#include <cstring>
#include <string>

#include <raylib.h>

#include "FCPP/Util/Semaphore.hpp"
#include "FCPP/IO/Audio.hpp"
#include "FCPP/IO/Input.hpp"
#include "FCPP/IO/Video.hpp"
#include "FCPP/IO/RayLib/RayLibController.hpp"

namespace fcpp::io::detail
{
    static constexpr int keyMap(const Keyboard key) noexcept
    {
        switch (key)
        {
        case Keyboard::A:         return KEY_A;
        case Keyboard::B:         return KEY_B;
        case Keyboard::C:         return KEY_C;
        case Keyboard::D:         return KEY_D;
        case Keyboard::E:         return KEY_E;
        case Keyboard::F:         return KEY_F;
        case Keyboard::G:         return KEY_G;
        case Keyboard::H:         return KEY_H;
        case Keyboard::I:         return KEY_I;
        case Keyboard::J:         return KEY_J;
        case Keyboard::K:         return KEY_K;
        case Keyboard::L:         return KEY_L;
        case Keyboard::M:         return KEY_M;
        case Keyboard::N:         return KEY_N;
        case Keyboard::O:         return KEY_O;
        case Keyboard::P:         return KEY_P;
        case Keyboard::Q:         return KEY_Q;
        case Keyboard::R:         return KEY_R;
        case Keyboard::S:         return KEY_S;
        case Keyboard::T:         return KEY_T;
        case Keyboard::U:         return KEY_U;
        case Keyboard::V:         return KEY_V;
        case Keyboard::W:         return KEY_W;
        case Keyboard::X:         return KEY_X;
        case Keyboard::Y:         return KEY_Y;
        case Keyboard::Z:         return KEY_Z;
        case Keyboard::Num0:      return KEY_ZERO;
        case Keyboard::Num1:      return KEY_ONE;
        case Keyboard::Num2:      return KEY_TWO;
        case Keyboard::Num3:      return KEY_THREE;
        case Keyboard::Num4:      return KEY_FOUR;
        case Keyboard::Num5:      return KEY_FIVE;
        case Keyboard::Num6:      return KEY_SIX;
        case Keyboard::Num7:      return KEY_SEVEN;
        case Keyboard::Num8:      return KEY_EIGHT;
        case Keyboard::Num9:      return KEY_NINE;
        case Keyboard::Escape:    return KEY_ESCAPE;
        case Keyboard::LControl:  return KEY_LEFT_CONTROL;
        case Keyboard::LShift:    return KEY_LEFT_SHIFT;
        case Keyboard::LAlt:      return KEY_LEFT_ALT;
        case Keyboard::System:    return KEY_LEFT_SUPER;
        case Keyboard::RControl:  return KEY_RIGHT_CONTROL;
        case Keyboard::RShift:    return KEY_RIGHT_SHIFT;
        case Keyboard::RAlt:      return KEY_RIGHT_ALT;
        case Keyboard::Menu:      return KEY_KB_MENU;
        case Keyboard::LBracket:  return KEY_LEFT_BRACKET;
        case Keyboard::RBracket:  return KEY_RIGHT_BRACKET;
        case Keyboard::Semicolon: return KEY_SEMICOLON;
        case Keyboard::Comma:     return KEY_COMMA;
        case Keyboard::Period:    return KEY_PERIOD;
        case Keyboard::Quote:     return KEY_APOSTROPHE;
        case Keyboard::Slash:     return KEY_SLASH;
        case Keyboard::Backslash: return KEY_BACKSLASH;
        case Keyboard::Tilde:     return KEY_GRAVE;
        case Keyboard::Equal:     return KEY_EQUAL;
        case Keyboard::Hyphen:    return KEY_MINUS;
        case Keyboard::Space:     return KEY_SPACE;
        case Keyboard::Enter:     return KEY_ENTER;
        case Keyboard::Backspace: return KEY_BACKSPACE;
        case Keyboard::Tab:       return KEY_TAB;
        case Keyboard::PageUp:    return KEY_PAGE_UP;
        case Keyboard::PageDown:  return KEY_PAGE_DOWN;
        case Keyboard::End:       return KEY_END;
        case Keyboard::Home:      return KEY_HOME;
        case Keyboard::Insert:    return KEY_INSERT;
        case Keyboard::Delete:    return KEY_DELETE;
        case Keyboard::Add:       return KEY_KP_ADD;
        case Keyboard::Subtract:  return KEY_KP_SUBTRACT;
        case Keyboard::Multiply:  return KEY_KP_MULTIPLY;
        case Keyboard::Divide:    return KEY_KP_DIVIDE;
        case Keyboard::Left:      return KEY_LEFT;
        case Keyboard::Right:     return KEY_RIGHT;
        case Keyboard::Up:        return KEY_UP;
        case Keyboard::Down:      return KEY_DOWN;
        case Keyboard::Numpad0:   return KEY_KP_0;
        case Keyboard::Numpad1:   return KEY_KP_1;
        case Keyboard::Numpad2:   return KEY_KP_2;
        case Keyboard::Numpad3:   return KEY_KP_3;
        case Keyboard::Numpad4:   return KEY_KP_4;
        case Keyboard::Numpad5:   return KEY_KP_5;
        case Keyboard::Numpad6:   return KEY_KP_6;
        case Keyboard::Numpad7:   return KEY_KP_7;
        case Keyboard::Numpad8:   return KEY_KP_8;
        case Keyboard::Numpad9:   return KEY_KP_9;
        case Keyboard::F1:        return KEY_F1;
        case Keyboard::F2:        return KEY_F2;
        case Keyboard::F3:        return KEY_F3;
        case Keyboard::F4:        return KEY_F4;
        case Keyboard::F5:        return KEY_F5;
        case Keyboard::F6:        return KEY_F6;
        case Keyboard::F7:        return KEY_F7;
        case Keyboard::F8:        return KEY_F8;
        case Keyboard::F9:        return KEY_F9;
        case Keyboard::F10:       return KEY_F10;
        case Keyboard::F11:       return KEY_F11;
        case Keyboard::F12:       return KEY_F12;
        case Keyboard::Pause:     return KEY_PAUSE;
        default:                  return KEY_NULL;
        }
    }
    static constexpr Keyboard keyMap(const int key) noexcept
    {
        switch (key)
        {
        case KEY_A:             return Keyboard::A;
        case KEY_B:             return Keyboard::B;
        case KEY_C:             return Keyboard::C;
        case KEY_D:             return Keyboard::D;
        case KEY_E:             return Keyboard::E;
        case KEY_F:             return Keyboard::F;
        case KEY_G:             return Keyboard::G;
        case KEY_H:             return Keyboard::H;
        case KEY_I:             return Keyboard::I;
        case KEY_J:             return Keyboard::J;
        case KEY_K:             return Keyboard::K;
        case KEY_L:             return Keyboard::L;
        case KEY_M:             return Keyboard::M;
        case KEY_N:             return Keyboard::N;
        case KEY_O:             return Keyboard::O;
        case KEY_P:             return Keyboard::P;
        case KEY_Q:             return Keyboard::Q;
        case KEY_R:             return Keyboard::R;
        case KEY_S:             return Keyboard::S;
        case KEY_T:             return Keyboard::T;
        case KEY_U:             return Keyboard::U;
        case KEY_V:             return Keyboard::V;
        case KEY_W:             return Keyboard::W;
        case KEY_X:             return Keyboard::X;
        case KEY_Y:             return Keyboard::Y;
        case KEY_Z:             return Keyboard::Z;
        case KEY_ZERO:          return Keyboard::Num0;
        case KEY_ONE:           return Keyboard::Num1;
        case KEY_TWO:           return Keyboard::Num2;
        case KEY_THREE:         return Keyboard::Num3;
        case KEY_FOUR:          return Keyboard::Num4;
        case KEY_FIVE:          return Keyboard::Num5;
        case KEY_SIX:           return Keyboard::Num6;
        case KEY_SEVEN:         return Keyboard::Num7;
        case KEY_EIGHT:         return Keyboard::Num8;
        case KEY_NINE:          return Keyboard::Num9;
        case KEY_ESCAPE:        return Keyboard::Escape;
        case KEY_LEFT_CONTROL:  return Keyboard::LControl;
        case KEY_LEFT_SHIFT:    return Keyboard::LShift;
        case KEY_LEFT_ALT:      return Keyboard::LAlt;
        case KEY_LEFT_SUPER:    return Keyboard::System;
        case KEY_RIGHT_CONTROL: return Keyboard::RControl;
        case KEY_RIGHT_SHIFT:   return Keyboard::RShift;
        case KEY_RIGHT_ALT:     return Keyboard::RAlt;
        case KEY_KB_MENU:       return Keyboard::Menu;
        case KEY_LEFT_BRACKET:  return Keyboard::LBracket;
        case KEY_RIGHT_BRACKET: return Keyboard::RBracket;
        case KEY_SEMICOLON:     return Keyboard::Semicolon;
        case KEY_COMMA:         return Keyboard::Comma;
        case KEY_PERIOD:        return Keyboard::Period;
        case KEY_APOSTROPHE:    return Keyboard::Quote;
        case KEY_SLASH:         return Keyboard::Slash;
        case KEY_BACKSLASH:     return Keyboard::Backslash;
        case KEY_GRAVE:         return Keyboard::Tilde;
        case KEY_EQUAL:         return Keyboard::Equal;
        case KEY_MINUS:         return Keyboard::Hyphen;
        case KEY_SPACE:         return Keyboard::Space;
        case KEY_ENTER:         return Keyboard::Enter;
        case KEY_BACKSPACE:     return Keyboard::Backspace;
        case KEY_TAB:           return Keyboard::Tab;
        case KEY_PAGE_UP:       return Keyboard::PageUp;
        case KEY_PAGE_DOWN:     return Keyboard::PageDown;
        case KEY_END:           return Keyboard::End;
        case KEY_HOME:          return Keyboard::Home;
        case KEY_INSERT:        return Keyboard::Insert;
        case KEY_DELETE:        return Keyboard::Delete;
        case KEY_KP_ADD:        return Keyboard::Add;
        case KEY_KP_SUBTRACT:   return Keyboard::Subtract;
        case KEY_KP_MULTIPLY:   return Keyboard::Multiply;
        case KEY_KP_DIVIDE:     return Keyboard::Divide;
        case KEY_LEFT:          return Keyboard::Left;
        case KEY_RIGHT:         return Keyboard::Right;
        case KEY_UP:            return Keyboard::Up;
        case KEY_DOWN:          return Keyboard::Down;
        case KEY_KP_0:          return Keyboard::Numpad0;
        case KEY_KP_1:          return Keyboard::Numpad1;
        case KEY_KP_2:          return Keyboard::Numpad2;
        case KEY_KP_3:          return Keyboard::Numpad3;
        case KEY_KP_4:          return Keyboard::Numpad4;
        case KEY_KP_5:          return Keyboard::Numpad5;
        case KEY_KP_6:          return Keyboard::Numpad6;
        case KEY_KP_7:          return Keyboard::Numpad7;
        case KEY_KP_8:          return Keyboard::Numpad8;
        case KEY_KP_9:          return Keyboard::Numpad9;
        case KEY_F1:            return Keyboard::F1;
        case KEY_F2:            return Keyboard::F2;
        case KEY_F3:            return Keyboard::F3;
        case KEY_F4:            return Keyboard::F4;
        case KEY_F5:            return Keyboard::F5;
        case KEY_F6:            return Keyboard::F6;
        case KEY_F7:            return Keyboard::F7;
        case KEY_F8:            return Keyboard::F8;
        case KEY_F9:            return Keyboard::F9;
        case KEY_F10:           return Keyboard::F10;
        case KEY_F11:           return Keyboard::F11;
        case KEY_F12:           return Keyboard::F12;
        case KEY_PAUSE:         return Keyboard::Pause;
        default:                return Keyboard::Unknown;
        }
    }

    class RayLibVideo : public Video
    {
    public:
        RayLibVideo() = default;
        ~RayLibVideo() noexcept override;

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
        Texture2D texture{};

        bool vsync = false;
        int width = 256, height = 240;
        unsigned int windowsMode = 0;
        std::string title{ "FCPP RayLib Renderer" };
        Color frameBuffer[256 * 240]{};
    };

    RayLibVideo::~RayLibVideo() noexcept
    {
        if (IsWindowReady())
        {
            UnloadTexture(texture);
            CloseWindow();
        }
    }
    bool RayLibVideo::create() noexcept
    {
        if (!IsWindowReady())
        {
            if (windowsMode)
            {
                SetConfigFlags(windowsMode);
                InitWindow(0, 0, title.c_str());
            }
            else
            {
                InitWindow(width, height, title.c_str());
                if (IsWindowFullscreen()) ToggleFullscreen();
                else if (IsWindowState(FLAG_WINDOW_UNDECORATED)) ClearWindowState(FLAG_WINDOW_UNDECORATED);
            }
            if (vsync) SetWindowState(FLAG_VSYNC_HINT);
            else ClearWindowState(FLAG_VSYNC_HINT);
            SetWindowState(FLAG_WINDOW_RESIZABLE);
            SetExitKey(KEY_NULL);
            auto image = GenImageColor(256, 240, WHITE);
            texture = LoadTextureFromImage(image);
            UnloadImage(image);

            return IsWindowReady();
        }
        return false;
    }
    void RayLibVideo::render() noexcept
    {
        if (!WindowShouldClose())
        {
            if (IsWindowResized())
            {
                width = GetRenderWidth();
                height = GetRenderHeight();
            }

            if (keyPressCallback)
                if (auto key = GetKeyPressed()) keyPressCallback(keyMap(key));

            if (renderCallback) renderCallback();

            UpdateTexture(texture, frameBuffer);

            BeginDrawing();
            {
                DrawTexturePro(texture,
                    { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) },
                    { 0.0f, 0.0f, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight()) },
                    { 0, 0 }, 0, WHITE);
            }
            EndDrawing();

            fpsLimiter.wait();
        }
        else if (closeCallback) closeCallback();
    }
    bool RayLibVideo::setFullScreen(const bool enable) noexcept
    {
        windowsMode = enable ? FLAG_FULLSCREEN_MODE : 0;
        if (IsWindowReady())
        {
            if (enable && !IsWindowFullscreen())
            {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                ToggleFullscreen();
            }
            else if (!enable && IsWindowFullscreen())
            {
                ToggleFullscreen();
                SetWindowSize(width, height);
            }
            return enable == IsWindowFullscreen();
        }
        return true;
    }
    bool RayLibVideo::setBorderless(const bool enable) noexcept
    {
        windowsMode = enable ? FLAG_WINDOW_UNDECORATED : 0;
        if (IsWindowReady())
        {
            if (enable && !IsWindowState(FLAG_WINDOW_UNDECORATED))
            {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                SetWindowState(FLAG_WINDOW_UNDECORATED);
            }
            else if (!enable && IsWindowState(FLAG_WINDOW_UNDECORATED))
            {
                ClearWindowState(FLAG_WINDOW_UNDECORATED);
                SetWindowSize(width, height);
            }
            return enable == IsWindowState(FLAG_WINDOW_UNDECORATED);
        }
        return true;
    }
    bool RayLibVideo::setVerticalSync(const bool enable) noexcept
    {
        vsync = enable;
        if (IsWindowReady())
        {
            if (enable && !IsWindowState(FLAG_VSYNC_HINT)) SetWindowState(FLAG_VSYNC_HINT);
            else if (!enable && IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
            return enable == IsWindowState(FLAG_VSYNC_HINT);
        }
        return true;
    }
    bool RayLibVideo::setRenderDriver(const int idx) noexcept
    {
        return true;
    }
    void RayLibVideo::setScale(const double factor) noexcept
    {
        width = static_cast<int>(256.0 * factor);
        height = static_cast<int>(240.0 * factor);
        if (IsWindowReady()) SetWindowSize(width, height);
    }
    void RayLibVideo::setTitle(const char* const text) noexcept
    {
        title = text;
        if (IsWindowReady()) SetWindowTitle(text);
    }
    void RayLibVideo::setFrameBufferData(const std::uint8_t* const data) noexcept
    {
        if (data != nullptr) std::memcpy(frameBuffer, data, Controller::FrameBufferSize);
    }
    void RayLibVideo::getFrameBufferData(std::uint8_t* const data) const noexcept
    {
        if (data != nullptr) std::memcpy(data, frameBuffer, Controller::FrameBufferSize);
    }
    void RayLibVideo::setPixel(const int x, const int y, const std::uint32_t color) noexcept
    {
        frameBuffer[static_cast<std::size_t>(256) * y + x].r = (color >> 16) & 0xff;
        frameBuffer[static_cast<std::size_t>(256) * y + x].g = (color >> 8) & 0xff;
        frameBuffer[static_cast<std::size_t>(256) * y + x].b = color & 0xff;
        frameBuffer[static_cast<std::size_t>(256) * y + x].a = 0xff;
    }
    void RayLibVideo::completedSignal() noexcept
    {
        if (frameCompletedCallback) frameCompletedCallback();
        else render();
    }

    class RayLibInput : public Input
    {
    public:
        RayLibInput() = default;
        ~RayLibInput() override = default;

        bool setJoystickPort(int port) noexcept;
        void bind(int standardButtonIdx, Keyboard key) noexcept;
        void bind(int standardButtonIdx, Joystick button) noexcept;
    private:
        std::uint8_t scan() noexcept override;
    private:
        std::uint8_t joystickState(Joystick button) noexcept;
    private:
        int joystickIdx = 0;
        int keys[10] = {
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
    bool RayLibInput::setJoystickPort(const int port) noexcept
    {
        joystickIdx = port;
        return true;
    }
    void RayLibInput::bind(const int standardButtonIdx, const Keyboard key) noexcept
    {
        keys[standardButtonIdx] = keyMap(key);
    }
    void RayLibInput::bind(const int standardButtonIdx, const Joystick button) noexcept
    {
        buttons[standardButtonIdx] = button;
    }
    std::uint8_t RayLibInput::scan() noexcept
    {
        std::uint8_t mask = 0;

        switch (joypadType)
        {
        case fcpp::core::JoypadType::Standard:
            turbo++;

            mask |=
                IsKeyDown(keys[7]) << 7 |
                IsKeyDown(keys[6]) << 6 |
                IsKeyDown(keys[5]) << 5 |
                IsKeyDown(keys[4]) << 4 |
                IsKeyDown(keys[3]) << 3 |
                IsKeyDown(keys[2]) << 2 |
                (IsKeyDown(keys[9]) ? ((turbo >> turboShift) & 1) : IsKeyDown(keys[1])) << 1 | //B
                (IsKeyDown(keys[8]) ? ((turbo >> turboShift) & 1) : IsKeyDown(keys[0])) << 0;  //A

            if (IsGamepadAvailable(joystickIdx))
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
    std::uint8_t RayLibInput::joystickState(const Joystick button) noexcept
    {
        switch (button)
        {
        case Joystick::Button0:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        case Joystick::Button1:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
        case Joystick::Button2:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        case Joystick::Button3:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_FACE_UP);
        case Joystick::Button4:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
        case Joystick::Button5:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
        case Joystick::Button6:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_MIDDLE_LEFT);
        case Joystick::Button7:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_MIDDLE_RIGHT);
        case Joystick::Button8:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_THUMB);
        case Joystick::Button9:  return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_RIGHT_THUMB);
        case Joystick::Button10: return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_MIDDLE);
        case Joystick::Up:       return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_FACE_UP);
        case Joystick::Down:     return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        case Joystick::Left:     return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        case Joystick::Right:    return IsGamepadButtonDown(joystickIdx, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        case Joystick::Axis0P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_LEFT_X) > +0.5f;
        case Joystick::Axis0N:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_LEFT_X) < -0.5f;
        case Joystick::Axis1P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_LEFT_Y) > +0.5f;
        case Joystick::Axis1N:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_LEFT_Y) < -0.5f;
        case Joystick::Axis2P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_RIGHT_X) > +0.5f;
        case Joystick::Axis2N:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_RIGHT_X) < -0.5f;
        case Joystick::Axis3P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_RIGHT_Y) > +0.5f;
        case Joystick::Axis3N:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_RIGHT_Y) < -0.5f;
        case Joystick::Axis4P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_LEFT_TRIGGER) > +0.5f;
        case Joystick::Axis5P:   return GetGamepadAxisMovement(joystickIdx, GAMEPAD_AXIS_RIGHT_TRIGGER) > +0.5f;
        default:                 return 0;
        }
    }

    class RayLibAudio : public Audio
    {
    public:
        RayLibAudio() = default;
        ~RayLibAudio() noexcept override;

        bool create() noexcept;
        void setVolume(float v) noexcept;
    private:
        void sendSample(double sample) noexcept override;
    private:
        void fillBuffer(void* buffer, unsigned int len) noexcept;
        static void callback(void* buffer, unsigned int count) noexcept;
    private:
        static constexpr std::size_t buffSize = 1024;
        static constexpr std::size_t buffNum = 6;

        AudioStream stream{};
        std::size_t writeCount = 0, readCount = 0, writeIdx = 0, readIdx = 0;
        fcpp::util::Semaphore sem{ buffNum - 1 };
        std::int16_t samples[buffSize * buffNum]{};

        static std::function<void(void*, unsigned int)> callbackFunc;
    };
    RayLibAudio::~RayLibAudio() noexcept
    {
        if (IsAudioDeviceReady())
        {
            StopAudioStream(stream);
            UnloadAudioStream(stream);
            CloseAudioDevice();
        }
    }
    bool RayLibAudio::create() noexcept
    {
        if (!IsAudioDeviceReady())
        {
            InitAudioDevice();
            stream = LoadAudioStream(sampleRate, 16, 1);
            callbackFunc = [this](void* const buffer, const unsigned int len) { fillBuffer(buffer, len); };
            SetAudioStreamCallback(stream, callback);
            PlayAudioStream(stream);
            return IsAudioDeviceReady();
        }
        return false;
    }
    void RayLibAudio::setVolume(const float v) noexcept
    {
        SetMasterVolume(v < 0.0f ? 0.0f : (100.0f < v ? 1.0f : v / 100.0f));
    }
    void RayLibAudio::sendSample(const double sample) noexcept
    {
        samples[writeIdx * buffSize + writeCount++] = static_cast<std::int16_t>(sample * 32767);
        if (writeCount >= buffSize)
        {
            writeCount = 0;
            writeIdx = (writeIdx < (buffNum - 1)) ? writeIdx + 1 : 0;
            sem.acquire();
        }
    }
    void RayLibAudio::fillBuffer(void* const buffer, const unsigned int len) noexcept
    {
        if (sem.value() < buffNum - 1)
        {
            if ((readIdx == (buffNum - 1)) && (buffSize - readCount < len))
            {
                auto rest = buffSize - readCount;
                std::memcpy(buffer, samples + readIdx * buffSize + readCount, rest * sizeof(std::int16_t));
                std::memcpy(static_cast<std::int16_t*>(buffer) + rest, samples, (len - rest) * sizeof(std::int16_t));
            }
            else std::memcpy(buffer, samples + readIdx * buffSize + readCount, len * sizeof(std::int16_t));
            if ((readCount += len) >= buffSize)
            {
                readCount -= buffSize;
                readIdx = (readIdx < (buffNum - 1)) ? readIdx + 1 : 0;
                sem.release();
            }
        }
        else std::memset(buffer, 0, len * sizeof(std::int16_t));
    }
    void RayLibAudio::callback(void* const buffer, const unsigned int len) noexcept
    {
        return callbackFunc(buffer, len);
    }
    std::function<void(void*, unsigned int)> RayLibAudio::callbackFunc{};
}

struct fcpp::io::RayLibController::RayLibControllerData
{
    detail::RayLibVideo video{};
    detail::RayLibInput input[2]{};
    detail::RayLibAudio audio{};
};

fcpp::io::RayLibController::RayLibController() : dptr(std::make_unique<RayLibControllerData>()) {}
fcpp::io::RayLibController::~RayLibController() noexcept = default;

bool fcpp::io::RayLibController::create() noexcept
{
    return dptr->audio.create() && dptr->video.create();
}
fcpp::core::FrameBuffer* fcpp::io::RayLibController::getFrameBuffer() noexcept
{
    return &dptr->video;
}
fcpp::core::InputScanner* fcpp::io::RayLibController::getInputScanner(const int idx) noexcept
{
    return &dptr->input[idx];
}
fcpp::core::SampleBuffer* fcpp::io::RayLibController::getSampleBuffer() noexcept
{
    return &dptr->audio;
}
void fcpp::io::RayLibController::render() noexcept
{
    dptr->video.render();
}
bool fcpp::io::RayLibController::setFullScreen(const bool enable) noexcept
{
    return dptr->video.setFullScreen(enable);
}
bool fcpp::io::RayLibController::setBorderless(const bool enable) noexcept
{
    return dptr->video.setBorderless(enable);
}
bool fcpp::io::RayLibController::setVerticalSync(const bool enable) noexcept
{
    return dptr->video.setVerticalSync(enable);
}
bool fcpp::io::RayLibController::setRenderDriver(const int idx) noexcept
{
    return true;
}
bool fcpp::io::RayLibController::setJoystickPort(const int idx, const int port) noexcept
{
    return dptr->input[idx].setJoystickPort(idx);
}
void fcpp::io::RayLibController::setScale(const float factor) noexcept
{
    dptr->video.setScale(factor);
}
void fcpp::io::RayLibController::setTitle(const char* const text) noexcept
{
    dptr->video.setTitle(text);
}
void fcpp::io::RayLibController::setFPSLimit(const double fps) noexcept
{
    dptr->video.setFPSLimit(fps);
}
void fcpp::io::RayLibController::setVolume(const float volume) noexcept
{
    dptr->audio.setVolume(volume);
}
void fcpp::io::RayLibController::setSampleRate(const int rate) noexcept
{
    dptr->audio.setSampleRate(rate);
}
void fcpp::io::RayLibController::setJoypadType(const int idx, const fcpp::core::JoypadType type) noexcept
{
    dptr->input[idx].setJoypadType(type);
}
void fcpp::io::RayLibController::setPaletteTable(const PaletteTable& paletteTable) noexcept
{
    dptr->video.setPaletteTable(paletteTable);
}
void fcpp::io::RayLibController::setPaletteTable(PaletteTable&& paletteTable) noexcept
{
    dptr->video.setPaletteTable(std::move(paletteTable));
}
void fcpp::io::RayLibController::setTurboButtonSpeed(const int idx, const std::uint8_t v) noexcept
{
    dptr->input[idx].setTurboSpeed(v);
}
void fcpp::io::RayLibController::setFrameBufferData(const std::uint8_t* const argb) noexcept
{
    dptr->video.setFrameBufferData(argb);
}
void fcpp::io::RayLibController::getFrameBufferData(std::uint8_t* const argb) const noexcept
{
    dptr->video.getFrameBufferData(argb);
}
void fcpp::io::RayLibController::bind(const int idx, const int standardButtonIdx, const Keyboard key) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, key);
}
void fcpp::io::RayLibController::bind(const int idx, const int standardButtonIdx, const Joystick button) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, button);
}
void fcpp::io::RayLibController::setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept
{
    dptr->video.setKeyPressCallback(callback);
}
void fcpp::io::RayLibController::setFrameCompleteCallback(std::function<void()> callback) noexcept
{
    dptr->video.setFrameCompleteCallback(callback);
}
void fcpp::io::RayLibController::setRenderCallback(std::function<void()> callback) noexcept
{
    dptr->video.setRenderCallback(callback);
}
void fcpp::io::RayLibController::setCloseCallback(std::function<void()> callback) noexcept
{
    dptr->video.setCloseCallback(callback);
}

int fcpp::io::RayLibController::Info::getRenderDriverCount() noexcept
{
    return 1;
}
const char* fcpp::io::RayLibController::Info::getRenderDriverName(const int idx) noexcept
{
    return "opengl";
}

bool fcpp::io::RayLibController::JoystickHelper::joystickPressed(const int port, fcpp::io::Joystick& buttons) noexcept
{
    bool ret = false;
    buttons = Joystick::Unknown;
    if (port >= 0 && port < joystickCount())
    {
        switch (GetGamepadButtonPressed())
        {
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN:  buttons = Joystick::Button0; break;
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: buttons = Joystick::Button1; break;
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT:  buttons = Joystick::Button2; break;
        case GAMEPAD_BUTTON_RIGHT_FACE_UP:    buttons = Joystick::Button3; break;
        case GAMEPAD_BUTTON_LEFT_TRIGGER_1:   buttons = Joystick::Button4; break;
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1:  buttons = Joystick::Button5; break;
        case GAMEPAD_BUTTON_MIDDLE_LEFT:      buttons = Joystick::Button6; break;
        case GAMEPAD_BUTTON_MIDDLE_RIGHT:     buttons = Joystick::Button7; break;
        case GAMEPAD_BUTTON_LEFT_THUMB:       buttons = Joystick::Button8; break;
        case GAMEPAD_BUTTON_RIGHT_THUMB:      buttons = Joystick::Button9; break;
        case GAMEPAD_BUTTON_MIDDLE:           buttons = Joystick::Button10; break;
        case GAMEPAD_BUTTON_LEFT_FACE_UP:     buttons = Joystick::Up; break;
        case GAMEPAD_BUTTON_LEFT_FACE_DOWN:   buttons = Joystick::Down; break;
        case GAMEPAD_BUTTON_LEFT_FACE_LEFT:   buttons = Joystick::Left; break;
        case GAMEPAD_BUTTON_LEFT_FACE_RIGHT:  buttons = Joystick::Right; break;
        }

        if (buttons == Joystick::Unknown)
        {
            if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_LEFT_X) > +0.5f) buttons = Joystick::Axis0P;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_LEFT_X) < -0.5f) buttons = Joystick::Axis0N;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_LEFT_Y) > +0.5f) buttons = Joystick::Axis1P;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_LEFT_Y) < -0.5f) buttons = Joystick::Axis1N;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_RIGHT_X) > +0.5f) buttons = Joystick::Axis2P;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_RIGHT_X) < -0.5f) buttons = Joystick::Axis2N;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_RIGHT_Y) > +0.5f) buttons = Joystick::Axis3P;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_RIGHT_Y) < -0.5f) buttons = Joystick::Axis3N;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_LEFT_TRIGGER) > +0.5f) buttons = Joystick::Axis4P;
            else if (GetGamepadAxisMovement(port, GAMEPAD_AXIS_RIGHT_TRIGGER) > +0.5f) buttons = Joystick::Axis5P;
        }
        ret = buttons != Joystick::Unknown;
    }
    return ret;
}
int fcpp::io::RayLibController::JoystickHelper::joystickCount() noexcept
{
    int count = 0;
    for (int i = 0; i < 4; i++)
        if (IsGamepadAvailable(i)) count++;
        else break;
    return count;
}
