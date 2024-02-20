#ifndef FCPP_QFCPP_EMULATOR_HPP
#define FCPP_QFCPP_EMULATOR_HPP

#include <memory>
#include <functional>

#include <QObject>

#include "FCPP/Core.hpp"
#include "FCPP/IO.hpp"

#define gEmulator (Emulator::instance())

class Emulator : public QObject
{
    Q_OBJECT

public:
    struct Config
    {
        bool fullScreen = false;
        bool vsync = true;
        int sampleRate = 44100;
        int engineIdx = 0;
        int renderDriverIdx = 0;
        int spriteLimit = 16;
        unsigned int tapeLength = 128;
        float scale = 2.0f;
        float volume = 100.0f;
        double fpsLimit = 60.0;
        fcpp::io::PaletteTable paletteTable{};
        fcpp::core::JoypadType joypadType[2] = { fcpp::core::JoypadType::Standard, fcpp::core::JoypadType::Standard };
        int joystickPort[2] = { 0, 0 };
        int turboButtonSpeed[2] = { 6, 6 };
        fcpp::io::Keyboard keyboardMap[2][10] = {
            {
                fcpp::io::Keyboard::A,
                fcpp::io::Keyboard::S,
                fcpp::io::Keyboard::X,
                fcpp::io::Keyboard::Z,
                fcpp::io::Keyboard::Up,
                fcpp::io::Keyboard::Down,
                fcpp::io::Keyboard::Left,
                fcpp::io::Keyboard::Right,
                fcpp::io::Keyboard::Q,
                fcpp::io::Keyboard::W
            },
            {
                fcpp::io::Keyboard::A,
                fcpp::io::Keyboard::S,
                fcpp::io::Keyboard::X,
                fcpp::io::Keyboard::Z,
                fcpp::io::Keyboard::Up,
                fcpp::io::Keyboard::Down,
                fcpp::io::Keyboard::Left,
                fcpp::io::Keyboard::Right,
                fcpp::io::Keyboard::Q,
                fcpp::io::Keyboard::W
            },
        };
        fcpp::io::Joystick joystickMap[2][10] = {
            {
                fcpp::io::Joystick::Button0,
                fcpp::io::Joystick::Button1,
                fcpp::io::Joystick::Button2,
                fcpp::io::Joystick::Button7,
                fcpp::io::Joystick::Up,
                fcpp::io::Joystick::Down,
                fcpp::io::Joystick::Left,
                fcpp::io::Joystick::Right,
                fcpp::io::Joystick::Button4,
                fcpp::io::Joystick::Button5
            },
            {
                fcpp::io::Joystick::Button0,
                fcpp::io::Joystick::Button1,
                fcpp::io::Joystick::Button2,
                fcpp::io::Joystick::Button7,
                fcpp::io::Joystick::Up,
                fcpp::io::Joystick::Down,
                fcpp::io::Joystick::Left,
                fcpp::io::Joystick::Right,
                fcpp::io::Joystick::Button4,
                fcpp::io::Joystick::Button5
            }
        };
    };
private:
    struct EmulatorData;
public:
    static Emulator& instance() noexcept;
private:
    Emulator();
    ~Emulator() noexcept override;
public:
    Config& getConfig() noexcept;
    std::uint64_t getTotalFrame() noexcept;
    void setQuickSnapshot(const fcpp::core::Snapshot& other) noexcept;
    fcpp::core::Snapshot getQuickSnapshot() noexcept;

    bool run(const char* filePath) noexcept;
    bool running() noexcept;
public slots:
    void stop() noexcept;
    void pause();

    void pushTask(const std::function<void(fcpp::io::Controller*)>& task);
    void pushRewind();
    void pushReset();
    void pushQuickSave();
    void pushQuickLoad();
signals:
    void started();
    void stopped();
private:
    const std::unique_ptr<EmulatorData> dptr;
};

#endif
