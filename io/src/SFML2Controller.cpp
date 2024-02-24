#include <cstring>
#include <utility>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "FCPP/IO/Audio.hpp"
#include "FCPP/IO/Input.hpp"
#include "FCPP/IO/Video.hpp"
#include "FCPP/IO/SFML2/SFML2Controller.hpp"
#include "FCPP/Util/LoopCounter.hpp"
#include "FCPP/Util/Semaphore.hpp"

namespace fcpp::io::detail
{
    static constexpr sf::Keyboard::Key keyMap(const Keyboard key) noexcept
    {
        switch (key)
        {
        case Keyboard::A:         return sf::Keyboard::Key::A;
        case Keyboard::B:         return sf::Keyboard::Key::B;
        case Keyboard::C:         return sf::Keyboard::Key::C;
        case Keyboard::D:         return sf::Keyboard::Key::D;
        case Keyboard::E:         return sf::Keyboard::Key::E;
        case Keyboard::F:         return sf::Keyboard::Key::F;
        case Keyboard::G:         return sf::Keyboard::Key::G;
        case Keyboard::H:         return sf::Keyboard::Key::H;
        case Keyboard::I:         return sf::Keyboard::Key::I;
        case Keyboard::J:         return sf::Keyboard::Key::J;
        case Keyboard::K:         return sf::Keyboard::Key::K;
        case Keyboard::L:         return sf::Keyboard::Key::L;
        case Keyboard::M:         return sf::Keyboard::Key::M;
        case Keyboard::N:         return sf::Keyboard::Key::N;
        case Keyboard::O:         return sf::Keyboard::Key::O;
        case Keyboard::P:         return sf::Keyboard::Key::P;
        case Keyboard::Q:         return sf::Keyboard::Key::Q;
        case Keyboard::R:         return sf::Keyboard::Key::R;
        case Keyboard::S:         return sf::Keyboard::Key::S;
        case Keyboard::T:         return sf::Keyboard::Key::T;
        case Keyboard::U:         return sf::Keyboard::Key::U;
        case Keyboard::V:         return sf::Keyboard::Key::V;
        case Keyboard::W:         return sf::Keyboard::Key::W;
        case Keyboard::X:         return sf::Keyboard::Key::X;
        case Keyboard::Y:         return sf::Keyboard::Key::Y;
        case Keyboard::Z:         return sf::Keyboard::Key::Z;
        case Keyboard::Num0:      return sf::Keyboard::Key::Num0;
        case Keyboard::Num1:      return sf::Keyboard::Key::Num1;
        case Keyboard::Num2:      return sf::Keyboard::Key::Num2;
        case Keyboard::Num3:      return sf::Keyboard::Key::Num3;
        case Keyboard::Num4:      return sf::Keyboard::Key::Num4;
        case Keyboard::Num5:      return sf::Keyboard::Key::Num5;
        case Keyboard::Num6:      return sf::Keyboard::Key::Num6;
        case Keyboard::Num7:      return sf::Keyboard::Key::Num7;
        case Keyboard::Num8:      return sf::Keyboard::Key::Num8;
        case Keyboard::Num9:      return sf::Keyboard::Key::Num9;
        case Keyboard::Escape:    return sf::Keyboard::Key::Escape;
        case Keyboard::LControl:  return sf::Keyboard::Key::LControl;
        case Keyboard::LShift:    return sf::Keyboard::Key::LShift;
        case Keyboard::LAlt:      return sf::Keyboard::Key::LAlt;
        case Keyboard::System:    return sf::Keyboard::Key::LSystem;
        case Keyboard::RControl:  return sf::Keyboard::Key::RControl;
        case Keyboard::RShift:    return sf::Keyboard::Key::RShift;
        case Keyboard::RAlt:      return sf::Keyboard::Key::RAlt;
        case Keyboard::Menu:      return sf::Keyboard::Key::Menu;
        case Keyboard::LBracket:  return sf::Keyboard::Key::LBracket;
        case Keyboard::RBracket:  return sf::Keyboard::Key::RBracket;
        case Keyboard::Semicolon: return sf::Keyboard::Key::Semicolon;
        case Keyboard::Comma:     return sf::Keyboard::Key::Comma;
        case Keyboard::Period:    return sf::Keyboard::Key::Period;
        case Keyboard::Quote:     return sf::Keyboard::Key::Quote;
        case Keyboard::Slash:     return sf::Keyboard::Key::Slash;
        case Keyboard::Backslash: return sf::Keyboard::Key::Backslash;
        case Keyboard::Tilde:     return sf::Keyboard::Key::Tilde;
        case Keyboard::Equal:     return sf::Keyboard::Key::Equal;
        case Keyboard::Hyphen:    return sf::Keyboard::Key::Hyphen;
        case Keyboard::Space:     return sf::Keyboard::Key::Space;
        case Keyboard::Enter:     return sf::Keyboard::Key::Enter;
        case Keyboard::Backspace: return sf::Keyboard::Key::Backspace;
        case Keyboard::Tab:       return sf::Keyboard::Key::Tab;
        case Keyboard::PageUp:    return sf::Keyboard::Key::PageUp;
        case Keyboard::PageDown:  return sf::Keyboard::Key::PageDown;
        case Keyboard::End:       return sf::Keyboard::Key::End;
        case Keyboard::Home:      return sf::Keyboard::Key::Home;
        case Keyboard::Insert:    return sf::Keyboard::Key::Insert;
        case Keyboard::Delete:    return sf::Keyboard::Key::Delete;
        case Keyboard::Add:       return sf::Keyboard::Key::Add;
        case Keyboard::Subtract:  return sf::Keyboard::Key::Subtract;
        case Keyboard::Multiply:  return sf::Keyboard::Key::Multiply;
        case Keyboard::Divide:    return sf::Keyboard::Key::Divide;
        case Keyboard::Left:      return sf::Keyboard::Key::Left;
        case Keyboard::Right:     return sf::Keyboard::Key::Right;
        case Keyboard::Up:        return sf::Keyboard::Key::Up;
        case Keyboard::Down:      return sf::Keyboard::Key::Down;
        case Keyboard::Numpad0:   return sf::Keyboard::Key::Numpad0;
        case Keyboard::Numpad1:   return sf::Keyboard::Key::Numpad1;
        case Keyboard::Numpad2:   return sf::Keyboard::Key::Numpad2;
        case Keyboard::Numpad3:   return sf::Keyboard::Key::Numpad3;
        case Keyboard::Numpad4:   return sf::Keyboard::Key::Numpad4;
        case Keyboard::Numpad5:   return sf::Keyboard::Key::Numpad5;
        case Keyboard::Numpad6:   return sf::Keyboard::Key::Numpad6;
        case Keyboard::Numpad7:   return sf::Keyboard::Key::Numpad7;
        case Keyboard::Numpad8:   return sf::Keyboard::Key::Numpad8;
        case Keyboard::Numpad9:   return sf::Keyboard::Key::Numpad9;
        case Keyboard::F1:        return sf::Keyboard::Key::F1;
        case Keyboard::F2:        return sf::Keyboard::Key::F2;
        case Keyboard::F3:        return sf::Keyboard::Key::F3;
        case Keyboard::F4:        return sf::Keyboard::Key::F4;
        case Keyboard::F5:        return sf::Keyboard::Key::F5;
        case Keyboard::F6:        return sf::Keyboard::Key::F6;
        case Keyboard::F7:        return sf::Keyboard::Key::F7;
        case Keyboard::F8:        return sf::Keyboard::Key::F8;
        case Keyboard::F9:        return sf::Keyboard::Key::F9;
        case Keyboard::F10:       return sf::Keyboard::Key::F10;
        case Keyboard::F11:       return sf::Keyboard::Key::F11;
        case Keyboard::F12:       return sf::Keyboard::Key::F12;
        case Keyboard::Pause:     return sf::Keyboard::Key::Pause;
        default:                  return sf::Keyboard::Key::Unknown;
        }
    }
    static constexpr Keyboard keyMap(const sf::Keyboard::Key key) noexcept
    {
        switch (key)
        {
        case sf::Keyboard::Key::A:         return Keyboard::A;
        case sf::Keyboard::Key::B:         return Keyboard::B;
        case sf::Keyboard::Key::C:         return Keyboard::C;
        case sf::Keyboard::Key::D:         return Keyboard::D;
        case sf::Keyboard::Key::E:         return Keyboard::E;
        case sf::Keyboard::Key::F:         return Keyboard::F;
        case sf::Keyboard::Key::G:         return Keyboard::G;
        case sf::Keyboard::Key::H:         return Keyboard::H;
        case sf::Keyboard::Key::I:         return Keyboard::I;
        case sf::Keyboard::Key::J:         return Keyboard::J;
        case sf::Keyboard::Key::K:         return Keyboard::K;
        case sf::Keyboard::Key::L:         return Keyboard::L;
        case sf::Keyboard::Key::M:         return Keyboard::M;
        case sf::Keyboard::Key::N:         return Keyboard::N;
        case sf::Keyboard::Key::O:         return Keyboard::O;
        case sf::Keyboard::Key::P:         return Keyboard::P;
        case sf::Keyboard::Key::Q:         return Keyboard::Q;
        case sf::Keyboard::Key::R:         return Keyboard::R;
        case sf::Keyboard::Key::S:         return Keyboard::S;
        case sf::Keyboard::Key::T:         return Keyboard::T;
        case sf::Keyboard::Key::U:         return Keyboard::U;
        case sf::Keyboard::Key::V:         return Keyboard::V;
        case sf::Keyboard::Key::W:         return Keyboard::W;
        case sf::Keyboard::Key::X:         return Keyboard::X;
        case sf::Keyboard::Key::Y:         return Keyboard::Y;
        case sf::Keyboard::Key::Z:         return Keyboard::Z;
        case sf::Keyboard::Key::Num0:      return Keyboard::Num0;
        case sf::Keyboard::Key::Num1:      return Keyboard::Num1;
        case sf::Keyboard::Key::Num2:      return Keyboard::Num2;
        case sf::Keyboard::Key::Num3:      return Keyboard::Num3;
        case sf::Keyboard::Key::Num4:      return Keyboard::Num4;
        case sf::Keyboard::Key::Num5:      return Keyboard::Num5;
        case sf::Keyboard::Key::Num6:      return Keyboard::Num6;
        case sf::Keyboard::Key::Num7:      return Keyboard::Num7;
        case sf::Keyboard::Key::Num8:      return Keyboard::Num8;
        case sf::Keyboard::Key::Num9:      return Keyboard::Num9;
        case sf::Keyboard::Key::Escape:    return Keyboard::Escape;
        case sf::Keyboard::Key::LControl:  return Keyboard::LControl;
        case sf::Keyboard::Key::LShift:    return Keyboard::LShift;
        case sf::Keyboard::Key::LAlt:      return Keyboard::LAlt;
        case sf::Keyboard::Key::LSystem:   return Keyboard::System;
        case sf::Keyboard::Key::RControl:  return Keyboard::RControl;
        case sf::Keyboard::Key::RShift:    return Keyboard::RShift;
        case sf::Keyboard::Key::RSystem:   return Keyboard::System;
        case sf::Keyboard::Key::RAlt:      return Keyboard::RAlt;
        case sf::Keyboard::Key::Menu:      return Keyboard::Menu;
        case sf::Keyboard::Key::LBracket:  return Keyboard::LBracket;
        case sf::Keyboard::Key::RBracket:  return Keyboard::RBracket;
        case sf::Keyboard::Key::Semicolon: return Keyboard::Semicolon;
        case sf::Keyboard::Key::Comma:     return Keyboard::Comma;
        case sf::Keyboard::Key::Period:    return Keyboard::Period;
        case sf::Keyboard::Key::Quote:     return Keyboard::Quote;
        case sf::Keyboard::Key::Slash:     return Keyboard::Slash;
        case sf::Keyboard::Key::Backslash: return Keyboard::Backslash;
        case sf::Keyboard::Key::Tilde:     return Keyboard::Tilde;
        case sf::Keyboard::Key::Equal:     return Keyboard::Equal;
        case sf::Keyboard::Key::Hyphen:    return Keyboard::Hyphen;
        case sf::Keyboard::Key::Space:     return Keyboard::Space;
        case sf::Keyboard::Key::Enter:     return Keyboard::Enter;
        case sf::Keyboard::Key::Backspace: return Keyboard::Backspace;
        case sf::Keyboard::Key::Tab:       return Keyboard::Tab;
        case sf::Keyboard::Key::PageUp:    return Keyboard::PageUp;
        case sf::Keyboard::Key::PageDown:  return Keyboard::PageDown;
        case sf::Keyboard::Key::End:       return Keyboard::End;
        case sf::Keyboard::Key::Home:      return Keyboard::Home;
        case sf::Keyboard::Key::Insert:    return Keyboard::Insert;
        case sf::Keyboard::Key::Delete:    return Keyboard::Delete;
        case sf::Keyboard::Key::Add:       return Keyboard::Add;
        case sf::Keyboard::Key::Subtract:  return Keyboard::Subtract;
        case sf::Keyboard::Key::Multiply:  return Keyboard::Multiply;
        case sf::Keyboard::Key::Divide:    return Keyboard::Divide;
        case sf::Keyboard::Key::Left:      return Keyboard::Left;
        case sf::Keyboard::Key::Right:     return Keyboard::Right;
        case sf::Keyboard::Key::Up:        return Keyboard::Up;
        case sf::Keyboard::Key::Down:      return Keyboard::Down;
        case sf::Keyboard::Key::Numpad0:   return Keyboard::Numpad0;
        case sf::Keyboard::Key::Numpad1:   return Keyboard::Numpad1;
        case sf::Keyboard::Key::Numpad2:   return Keyboard::Numpad2;
        case sf::Keyboard::Key::Numpad3:   return Keyboard::Numpad3;
        case sf::Keyboard::Key::Numpad4:   return Keyboard::Numpad4;
        case sf::Keyboard::Key::Numpad5:   return Keyboard::Numpad5;
        case sf::Keyboard::Key::Numpad6:   return Keyboard::Numpad6;
        case sf::Keyboard::Key::Numpad7:   return Keyboard::Numpad7;
        case sf::Keyboard::Key::Numpad8:   return Keyboard::Numpad8;
        case sf::Keyboard::Key::Numpad9:   return Keyboard::Numpad9;
        case sf::Keyboard::Key::F1:        return Keyboard::F1;
        case sf::Keyboard::Key::F2:        return Keyboard::F2;
        case sf::Keyboard::Key::F3:        return Keyboard::F3;
        case sf::Keyboard::Key::F4:        return Keyboard::F4;
        case sf::Keyboard::Key::F5:        return Keyboard::F5;
        case sf::Keyboard::Key::F6:        return Keyboard::F6;
        case sf::Keyboard::Key::F7:        return Keyboard::F7;
        case sf::Keyboard::Key::F8:        return Keyboard::F8;
        case sf::Keyboard::Key::F9:        return Keyboard::F9;
        case sf::Keyboard::Key::F10:       return Keyboard::F10;
        case sf::Keyboard::Key::F11:       return Keyboard::F11;
        case sf::Keyboard::Key::F12:       return Keyboard::F12;
        case sf::Keyboard::Key::Pause:     return Keyboard::Pause;
        default:                           return Keyboard::Unknown;
        }
    }
    static std::uint8_t keyState[sf::Keyboard::KeyCount]{};

    class SFML2Video : public Video
    {
    public:
        SFML2Video() = default;
        ~SFML2Video() override = default;

        bool create() noexcept;
        void render() noexcept;
        bool setFullScreen(bool enable) noexcept;
        bool setBorderless(bool enable) noexcept;
        bool setVerticalSync(bool enable) noexcept;
        void setScale(float factor) noexcept;
        void setTitle(const char* text) noexcept;
        void setFrameBufferData(const std::uint8_t* data) noexcept;
        void getFrameBufferData(std::uint8_t* data) const noexcept;
    private:
        void setPixel(int x, int y, std::uint32_t color) noexcept override;
        void completedSignal() noexcept override;
    private:
        bool vsync = false;
        unsigned int width = 256, height = 240;

        sf::Uint32 style = sf::Style::Default;
        sf::String title{ "FCPP SFML2 Renderer" };
        sf::VideoMode videoMode{ width, height };
        sf::RenderWindow window{};
        sf::Image image{};
        sf::Texture texture{};
        sf::Sprite sprite{};
    };
    bool SFML2Video::create() noexcept
    {
        window.create(videoMode, title, style);
        window.setVerticalSyncEnabled(vsync);
        window.setView(sf::View{ sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f) });
        image.create(256, 240);
        if (!texture.create(256, 240)) return false;
        sprite.setTexture(texture);
        return true;
    }
    void SFML2Video::render() noexcept
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                if (closeCallback) closeCallback();
                break;
            case sf::Event::KeyPressed:
                keyState[event.key.code] = 1;
                if (keyPressCallback) keyPressCallback(keyMap(event.key.code));
                break;
            case sf::Event::KeyReleased:
                keyState[event.key.code] = 0;
                break;
            case sf::Event::LostFocus:
                std::memset(keyState, 0, sizeof(keyState));
                break;
            case sf::Event::Resized:
                width = event.size.width;
                height = event.size.height;
                break;
            default:
                break;
            }
        }

        if (renderCallback) renderCallback();

        texture.update(image);
        window.draw(sprite);
        window.display();
    }
    bool SFML2Video::setFullScreen(const bool enable) noexcept
    {
        videoMode = enable ? sf::VideoMode::getDesktopMode() : sf::VideoMode(width, height);
        style = enable ? sf::Style::Fullscreen : sf::Style::Default;
        if (window.isOpen())
        {
            window.create(videoMode, title, style);
            window.setVerticalSyncEnabled(vsync);
            window.setView(sf::View{ sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f) });
        }
        return true;
    }
    bool SFML2Video::setBorderless(const bool enable) noexcept
    {
        videoMode = enable ? sf::VideoMode::getDesktopMode() : sf::VideoMode(width, height);
        style = enable ? sf::Style::None : sf::Style::Default;
        if (window.isOpen())
        {
            window.create(videoMode, title, style);
            window.setVerticalSyncEnabled(vsync);
            window.setView(sf::View{ sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f) });
        }
        return true;
    }
    bool SFML2Video::setVerticalSync(const bool enable) noexcept
    {
        if (vsync != enable) window.setVerticalSyncEnabled(vsync = enable);
        return true;
    }
    void SFML2Video::setScale(const float factor) noexcept
    {
        width = static_cast<unsigned int>(256.0 * factor);
        height = static_cast<unsigned int>(240.0 * factor);
        videoMode = sf::VideoMode(width, height);
        if (window.isOpen()) window.setSize(sf::Vector2u(width, height));
    }
    void SFML2Video::setTitle(const char* const text) noexcept
    {
        window.setTitle(title = text);
    }
    void SFML2Video::setFrameBufferData(const std::uint8_t* const data) noexcept
    {
        if (data != nullptr) std::memcpy(const_cast<std::uint8_t*>(image.getPixelsPtr()), data, Controller::FrameBufferSize);
    }
    void SFML2Video::getFrameBufferData(std::uint8_t* const data) const noexcept
    {
        if (data != nullptr) std::memcpy(data, image.getPixelsPtr(), Controller::FrameBufferSize);
    }
    void SFML2Video::setPixel(const int x, const int y, const std::uint32_t color) noexcept
    {
        image.setPixel(x, y, sf::Color((color << 8) | 0xff));
    }
    void SFML2Video::completedSignal() noexcept
    {
        if (frameCompletedCallback) frameCompletedCallback();
        else
        {
            render();
            fpsLimiter.wait();
        }
    }

    class SFML2Input : public Input
    {
    public:
        SFML2Input() = default;
        ~SFML2Input() override = default;

        bool setJoystickPort(int port) noexcept;
        void bind(int standardButtonIdx, Keyboard key) noexcept;
        void bind(int standardButtonIdx, Joystick button) noexcept;
    private:
        std::uint8_t scan() noexcept override;
    private:
        std::uint8_t joystickState(Joystick button) noexcept;
    private:
        unsigned int joystickIdx = 0;
        sf::Keyboard::Key keys[10] = {
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
    bool SFML2Input::setJoystickPort(const int port) noexcept
    {
        joystickIdx = port;
        return true;
    }
    void SFML2Input::bind(const int standardButtonIdx, const Keyboard key) noexcept
    {
        keys[standardButtonIdx] = keyMap(key);
    }
    void SFML2Input::bind(const int standardButtonIdx, const Joystick button) noexcept
    {
        buttons[standardButtonIdx] = button;
    }
    std::uint8_t SFML2Input::scan() noexcept
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

            if (sf::Joystick::isConnected(joystickIdx))
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
    std::uint8_t SFML2Input::joystickState(const Joystick button) noexcept
    {
        switch (button)
        {
        case Joystick::Button0:  return sf::Joystick::isButtonPressed(joystickIdx, 0);
        case Joystick::Button1:  return sf::Joystick::isButtonPressed(joystickIdx, 1);
        case Joystick::Button2:  return sf::Joystick::isButtonPressed(joystickIdx, 2);
        case Joystick::Button3:  return sf::Joystick::isButtonPressed(joystickIdx, 3);
        case Joystick::Button4:  return sf::Joystick::isButtonPressed(joystickIdx, 4);
        case Joystick::Button5:  return sf::Joystick::isButtonPressed(joystickIdx, 5);
        case Joystick::Button6:  return sf::Joystick::isButtonPressed(joystickIdx, 6);
        case Joystick::Button7:  return sf::Joystick::isButtonPressed(joystickIdx, 7);
        case Joystick::Button8:  return sf::Joystick::isButtonPressed(joystickIdx, 8);
        case Joystick::Button9:  return sf::Joystick::isButtonPressed(joystickIdx, 9);
        case Joystick::Button10: return sf::Joystick::isButtonPressed(joystickIdx, 10);
        case Joystick::Up:       return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::PovY) > +50.0f;
        case Joystick::Down:     return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::PovY) < -50.0f;
        case Joystick::Left:     return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::PovX) < -50.0f;
        case Joystick::Right:    return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::PovX) > +50.0f;
        case Joystick::Axis0P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::X) > +50.0f;
        case Joystick::Axis0N:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::X) < -50.0f;
        case Joystick::Axis1P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::Y) > +50.0f;
        case Joystick::Axis1N:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::Y) < -50.0f;
        case Joystick::Axis2P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::U) > +50.0f;
        case Joystick::Axis2N:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::U) < -50.0f;
        case Joystick::Axis3P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::V) > +50.0f;
        case Joystick::Axis3N:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::V) < -50.0f;
        case Joystick::Axis4P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::Z) > +50.0f;
        case Joystick::Axis5P:   return sf::Joystick::getAxisPosition(joystickIdx, sf::Joystick::Axis::R) > +50.0f;
        default:                 return 0;
        }
    }

    class SFML2Audio :
        public Audio,
        private sf::SoundStream
    {
    public:
        SFML2Audio() = default;
        ~SFML2Audio() override = default;

        void create() noexcept;
        void setVolume(float v) noexcept;
    private:
        void sendSample(double sample) noexcept override;
    private:
        bool onGetData(Chunk& data) noexcept override;
        void onSeek(sf::Time timeOffset) noexcept override;
    private:
        static constexpr std::size_t buffSize = 1024;
        static constexpr std::size_t buffNum = 6;

        std::size_t count = 0;
        fcpp::util::LoopCounter<std::size_t> readIdx{ buffNum - 1 }, writeIdx{ buffNum - 1 };
        fcpp::util::Semaphore sem{ buffNum - 1 };
        std::int16_t samples[buffSize * buffNum]{};
    };
    void SFML2Audio::create() noexcept
    {
        initialize(1, sampleRate);
        play();
    }
    void SFML2Audio::setVolume(const float v) noexcept
    {
        sf::SoundStream::setVolume(v);
    }
    void SFML2Audio::sendSample(const double sample) noexcept
    {
        samples[writeIdx * buffSize + count++] = static_cast<std::int16_t>(sample * 32767);
        if (count >= buffSize)
        {
            count = 0;
            ++writeIdx;
            sem.acquire();
        }
    }
    bool SFML2Audio::onGetData(Chunk& data) noexcept
    {
        if (sem.value() < buffNum - 1)
        {
            data.samples = samples + readIdx * buffSize;
            data.sampleCount = buffSize;
            ++readIdx;
            sem.release();
        }
        else
        {
            auto buffer = samples + readIdx.previous() * buffSize;
            std::memset(buffer, 0, buffSize * sizeof(std::int16_t));
            data.samples = buffer;
            data.sampleCount = buffSize;
        }
        return true;
    }
    void SFML2Audio::onSeek(const sf::Time timeOffset) noexcept
    {
        return;
    }
}

struct fcpp::io::SFML2Controller::SFML2ControllerData
{
    detail::SFML2Video video{};
    detail::SFML2Input input[2]{};
    detail::SFML2Audio audio{};
};

fcpp::io::SFML2Controller::SFML2Controller() : dptr(std::make_unique<SFML2ControllerData>()) {}
fcpp::io::SFML2Controller::~SFML2Controller() noexcept = default;

bool fcpp::io::SFML2Controller::create() noexcept
{
    dptr->audio.create();
    return dptr->video.create();
}
fcpp::core::FrameBuffer* fcpp::io::SFML2Controller::getFrameBuffer() noexcept
{
    return &dptr->video;
}
fcpp::core::InputScanner* fcpp::io::SFML2Controller::getInputScanner(const int idx) noexcept
{
    return &dptr->input[idx];
}
fcpp::core::SampleBuffer* fcpp::io::SFML2Controller::getSampleBuffer() noexcept
{
    return &dptr->audio;
}
void fcpp::io::SFML2Controller::render() noexcept
{
    dptr->video.render();
}
bool fcpp::io::SFML2Controller::setFullScreen(const bool enable) noexcept
{
    return dptr->video.setFullScreen(enable);
}
bool fcpp::io::SFML2Controller::setBorderless(const bool enable) noexcept
{
    return dptr->video.setBorderless(enable);
}
bool fcpp::io::SFML2Controller::setVerticalSync(const bool enable) noexcept
{
    return dptr->video.setVerticalSync(enable);
}
bool fcpp::io::SFML2Controller::setRenderDriver(const int idx) noexcept
{
    return true;
}
bool fcpp::io::SFML2Controller::setJoystickPort(const int idx, const int port) noexcept
{
    return dptr->input[idx].setJoystickPort(idx);
}
void fcpp::io::SFML2Controller::setScale(const float factor) noexcept
{
    dptr->video.setScale(factor);
}
void fcpp::io::SFML2Controller::setTitle(const char* const text) noexcept
{
    dptr->video.setTitle(text);
}
void fcpp::io::SFML2Controller::setFPSLimit(const double fps) noexcept
{
    dptr->video.setFPSLimit(fps);
}
void fcpp::io::SFML2Controller::setVolume(const float volume) noexcept
{
    dptr->audio.setVolume(volume);
}
void fcpp::io::SFML2Controller::setSampleRate(const int rate) noexcept
{
    dptr->audio.setSampleRate(rate);
}
void fcpp::io::SFML2Controller::setJoypadType(const int idx, const fcpp::core::JoypadType type) noexcept
{
    dptr->input[idx].setJoypadType(type);
}
void fcpp::io::SFML2Controller::setPaletteTable(const PaletteTable& paletteTable) noexcept
{
    dptr->video.setPaletteTable(paletteTable);
}
void fcpp::io::SFML2Controller::setPaletteTable(PaletteTable&& paletteTable) noexcept
{
    dptr->video.setPaletteTable(std::move(paletteTable));
}
void fcpp::io::SFML2Controller::setTurboButtonSpeed(const int idx, const std::uint8_t v) noexcept
{
    dptr->input[idx].setTurboSpeed(v);
}
void fcpp::io::SFML2Controller::setFrameBufferData(const std::uint8_t* const argb) noexcept
{
    dptr->video.setFrameBufferData(argb);
}
void fcpp::io::SFML2Controller::getFrameBufferData(std::uint8_t* const argb) const noexcept
{
    dptr->video.getFrameBufferData(argb);
}
void fcpp::io::SFML2Controller::bind(const int idx, const int standardButtonIdx, const Keyboard key) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, key);
}
void fcpp::io::SFML2Controller::bind(const int idx, const int standardButtonIdx, const Joystick button) noexcept
{
    dptr->input[idx].bind(standardButtonIdx, button);
}
void fcpp::io::SFML2Controller::setKeyPressCallback(std::function<void(Keyboard)> callback) noexcept
{
    dptr->video.setKeyPressCallback(std::move(callback));
}
void fcpp::io::SFML2Controller::setFrameCompleteCallback(std::function<void()> callback) noexcept
{
    dptr->video.setFrameCompleteCallback(std::move(callback));
}
void fcpp::io::SFML2Controller::setRenderCallback(std::function<void()> callback) noexcept
{
    dptr->video.setRenderCallback(std::move(callback));
}
void fcpp::io::SFML2Controller::setCloseCallback(std::function<void()> callback) noexcept
{
    dptr->video.setCloseCallback(std::move(callback));
}

int fcpp::io::SFML2Controller::Info::getRenderDriverCount() noexcept
{
    return 1;
}
const char* fcpp::io::SFML2Controller::Info::getRenderDriverName(const int idx) noexcept
{
    return "opengl";
}

bool fcpp::io::SFML2Controller::JoystickHelper::joystickPressed(const int port, fcpp::io::Joystick& buttons) noexcept
{
    bool ret = false;
    buttons = Joystick::Unknown;
    if (port >= 0 && port < joystickCount())
    {
        if (sf::Joystick::isButtonPressed(port, 0)) buttons = Joystick::Button0;
        else if (sf::Joystick::isButtonPressed(port, 1)) buttons = Joystick::Button1;
        else if (sf::Joystick::isButtonPressed(port, 2)) buttons = Joystick::Button2;
        else if (sf::Joystick::isButtonPressed(port, 3)) buttons = Joystick::Button3;
        else if (sf::Joystick::isButtonPressed(port, 4)) buttons = Joystick::Button4;
        else if (sf::Joystick::isButtonPressed(port, 5)) buttons = Joystick::Button5;
        else if (sf::Joystick::isButtonPressed(port, 6)) buttons = Joystick::Button6;
        else if (sf::Joystick::isButtonPressed(port, 7)) buttons = Joystick::Button7;
        else if (sf::Joystick::isButtonPressed(port, 8)) buttons = Joystick::Button8;
        else if (sf::Joystick::isButtonPressed(port, 9)) buttons = Joystick::Button9;
        else if (sf::Joystick::isButtonPressed(port, 10)) buttons = Joystick::Button10;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::PovY) > +50.0f) buttons = Joystick::Up;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::PovY) < -50.0f) buttons = Joystick::Down;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::PovX) < -50.0f) buttons = Joystick::Left;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::PovX) > +50.0f) buttons = Joystick::Right;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::X) > +50.0f) buttons = Joystick::Axis0P;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::X) < -50.0f) buttons = Joystick::Axis0N;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::Y) > +50.0f) buttons = Joystick::Axis1P;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::Y) < -50.0f) buttons = Joystick::Axis1N;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::U) > +50.0f) buttons = Joystick::Axis2P;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::U) < -50.0f) buttons = Joystick::Axis2N;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::V) > +50.0f) buttons = Joystick::Axis3P;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::V) < -50.0f) buttons = Joystick::Axis3N;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::Z) > +50.0f) buttons = Joystick::Axis4P;
        else if (sf::Joystick::getAxisPosition(port, sf::Joystick::Axis::R) > +50.0f) buttons = Joystick::Axis5P;
        ret = buttons != Joystick::Unknown;
    }
    return ret;
}
int fcpp::io::SFML2Controller::JoystickHelper::joystickCount() noexcept
{
    int count = 0;
    sf::Joystick::update();
    for (int i = 0; i < 4; i++)
        if (sf::Joystick::isConnected(i)) count++;
        else break;
    return count;
}
