#include <QDir>
#include <QSettings>

#include "Config.hpp"

Config& Config::instance() noexcept
{
    static Config config{ gEmulator.getConfig() };
    return config;
}

Config::Config(Emulator::Config& config) noexcept : emu(config)
{
    QDir dir{};
    dir.mkpath(QFCPP_DATA_PATH);
    dir.mkpath(QFCPP_AUTO_SAVE_PATH);
    load();
}
Config::~Config() noexcept
{
    save();
}

void Config::save() noexcept
{
    QSettings settings(QFCPP_SETTINGS_FILE, QSettings::IniFormat);

    settings.beginGroup("GUI");
    settings.setValue("AutoResume", gui.autoResume);
    settings.setValue("ExitConfirmation", gui.exitConfirmation);
    settings.setValue("StyleName", gui.styleName);
    settings.setValue("PalettePath", gui.palettePath);
    settings.setValue("RomFolders", gui.romFolders);
    settings.endGroup();

    settings.beginGroup("EMU");
    settings.setValue("FullScreen", emu.fullScreen);
    settings.setValue("Vsync", emu.vsync);
    settings.setValue("SampleRate", emu.sampleRate);
    settings.setValue("EngineIndex", emu.engineIdx);
    settings.setValue("RenderDriverIndex", emu.renderDriverIdx);
    settings.setValue("SpriteLimit", emu.spriteLimit);
    settings.setValue("TapeLength", emu.tapeLength);
    settings.setValue("Scale", emu.scale);
    settings.setValue("Volume", emu.volume);
    settings.setValue("FPSLimit", emu.fpsLimit);
    settings.setValue("TurboButtonSpeedPort1", emu.turboButtonSpeed[0]);
    settings.setValue("TurboButtonSpeedPort2", emu.turboButtonSpeed[1]);
    settings.setValue("JoypadTypePort1", static_cast<int>(emu.joypadType[0]));
    settings.setValue("JoypadTypePort2", static_cast<int>(emu.joypadType[1]));
    settings.setValue("JoystickPortPort1", emu.joystickPort[0]);
    settings.setValue("JoystickPortPort2", emu.joystickPort[1]);
    settings.endGroup();

    settings.beginGroup("INPUT");
    settings.beginWriteArray("KeyboardMapPort1");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("Keyboard", static_cast<int>(emu.keyboardMap[0][i]));
    }
    settings.endArray();
    settings.beginWriteArray("KeyboardMapPort2");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("Keyboard", static_cast<int>(emu.keyboardMap[1][i]));
    }
    settings.endArray();
    settings.beginWriteArray("JoystickMapPort1");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("Joystick", static_cast<int>(emu.joystickMap[0][i]));
    }
    settings.endArray();
    settings.beginWriteArray("JoystickMapPort2");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("Joystick", static_cast<int>(emu.joystickMap[1][i]));
    }
    settings.endArray();
    settings.endGroup();
}
void Config::load() noexcept
{
    QSettings settings(QFCPP_SETTINGS_FILE, QSettings::IniFormat);

    settings.beginGroup("GUI");
    gui.autoResume = settings.value("AutoResume", gui.autoResume).toBool();
    gui.exitConfirmation = settings.value("ExitConfirmation", gui.exitConfirmation).toBool();
    gui.styleName = settings.value("StyleName", gui.styleName).toString();
    gui.palettePath = settings.value("PalettePath", gui.palettePath).toString();
    gui.romFolders = settings.value("RomFolders", gui.romFolders).toStringList();
    settings.endGroup();

    settings.beginGroup("EMU");
    emu.fullScreen = settings.value("FullScreen", emu.fullScreen).toBool();
    emu.vsync = settings.value("Vsync", emu.vsync).toBool();
    emu.sampleRate = settings.value("SampleRate", emu.sampleRate).toInt();
    emu.engineIdx = settings.value("EngineIndex", emu.engineIdx).toInt();
    emu.renderDriverIdx = settings.value("RenderDriverIndex", emu.renderDriverIdx).toInt();
    emu.spriteLimit = settings.value("SpriteLimit", emu.spriteLimit).toInt();
    emu.tapeLength = settings.value("TapeLength", emu.tapeLength).toUInt();
    emu.scale = settings.value("Scale", emu.scale).toFloat();
    emu.volume = settings.value("Volume", emu.volume).toFloat();
    emu.fpsLimit = settings.value("FPSLimit", emu.fpsLimit).toDouble();
    emu.turboButtonSpeed[0] = settings.value("TurboButtonSpeedPort1", emu.turboButtonSpeed[0]).toInt();
    emu.turboButtonSpeed[1] = settings.value("TurboButtonSpeedPort2", emu.turboButtonSpeed[1]).toInt();
    emu.joypadType[0] =	static_cast<fcpp::core::JoypadType>(settings.value("JoypadTypePort1", static_cast<int>(emu.joypadType[0])).toInt());
    emu.joypadType[1] = static_cast<fcpp::core::JoypadType>(settings.value("JoypadTypePort2", static_cast<int>(emu.joypadType[1])).toInt());
    emu.joystickPort[0] = settings.value("JoystickPortPort1", emu.joystickPort[0]).toInt();
    emu.joystickPort[1] = settings.value("JoystickPortPort2", emu.joystickPort[1]).toInt();
    settings.endGroup();

    settings.beginGroup("INPUT");
    settings.beginReadArray("KeyboardMapPort1");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        emu.keyboardMap[0][i] = static_cast<fcpp::io::Keyboard>(settings.value("Keyboard", static_cast<int>(emu.keyboardMap[0][i])).toInt());
    }
    settings.endArray();
    settings.beginReadArray("KeyboardMapPort2");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        emu.keyboardMap[1][i] = static_cast<fcpp::io::Keyboard>(settings.value("Keyboard", static_cast<int>(emu.keyboardMap[1][i])).toInt());
    }
    settings.endArray();
    settings.beginReadArray("JoystickMapPort1");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        emu.joystickMap[0][i] = static_cast<fcpp::io::Joystick>(settings.value("Joystick", static_cast<int>(emu.joystickMap[0][i])).toInt());
    }
    settings.endArray();
    settings.beginReadArray("JoystickMapPort2");
    for(int i = 0; i < 10; i++)
    {
        settings.setArrayIndex(i);
        emu.joystickMap[1][i] = static_cast<fcpp::io::Joystick>(settings.value("Joystick", static_cast<int>(emu.joystickMap[1][i])).toInt());
    }
    settings.endArray();
    settings.endGroup();

    emu.paletteTable.load(gui.palettePath.toLocal8Bit().constData());
}
