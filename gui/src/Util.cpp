#include <QHash>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QKeySequence>

#include "FCPP/Util/Archive.hpp"
#include "Config.hpp"
#include "Util.hpp"

#define KeyToString(key) QKeySequence{ key }.toString()
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
#define StringToKey(str) QKeySequence::fromString(str)[0]
#else
#define StringToKey(str) QKeySequence::fromString(str)[0].toCombined()
#endif

const QStringList& util::joypadTypeList()
{
    static const QStringList list{ "Standard" };
    return list;
}

fcpp::io::Keyboard util::keyMap(const int key)
{
    switch (key)
    {
    case Qt::Key::Key_A:            return fcpp::io::Keyboard::A;
    case Qt::Key::Key_B:            return fcpp::io::Keyboard::B;
    case Qt::Key::Key_C:            return fcpp::io::Keyboard::C;
    case Qt::Key::Key_D:            return fcpp::io::Keyboard::D;
    case Qt::Key::Key_E:            return fcpp::io::Keyboard::E;
    case Qt::Key::Key_F:            return fcpp::io::Keyboard::F;
    case Qt::Key::Key_G:            return fcpp::io::Keyboard::G;
    case Qt::Key::Key_H:            return fcpp::io::Keyboard::H;
    case Qt::Key::Key_I:            return fcpp::io::Keyboard::I;
    case Qt::Key::Key_J:            return fcpp::io::Keyboard::J;
    case Qt::Key::Key_K:            return fcpp::io::Keyboard::K;
    case Qt::Key::Key_L:            return fcpp::io::Keyboard::L;
    case Qt::Key::Key_M:            return fcpp::io::Keyboard::M;
    case Qt::Key::Key_N:            return fcpp::io::Keyboard::N;
    case Qt::Key::Key_O:            return fcpp::io::Keyboard::O;
    case Qt::Key::Key_P:            return fcpp::io::Keyboard::P;
    case Qt::Key::Key_Q:            return fcpp::io::Keyboard::Q;
    case Qt::Key::Key_R:            return fcpp::io::Keyboard::R;
    case Qt::Key::Key_S:            return fcpp::io::Keyboard::S;
    case Qt::Key::Key_T:            return fcpp::io::Keyboard::T;
    case Qt::Key::Key_U:            return fcpp::io::Keyboard::U;
    case Qt::Key::Key_V:            return fcpp::io::Keyboard::V;
    case Qt::Key::Key_W:            return fcpp::io::Keyboard::W;
    case Qt::Key::Key_X:            return fcpp::io::Keyboard::X;
    case Qt::Key::Key_Y:            return fcpp::io::Keyboard::Y;
    case Qt::Key::Key_Z:            return fcpp::io::Keyboard::Z;
    case Qt::Key::Key_0:            return fcpp::io::Keyboard::Num0;
    case Qt::Key::Key_1:            return fcpp::io::Keyboard::Num1;
    case Qt::Key::Key_2:            return fcpp::io::Keyboard::Num2;
    case Qt::Key::Key_3:            return fcpp::io::Keyboard::Num3;
    case Qt::Key::Key_4:            return fcpp::io::Keyboard::Num4;
    case Qt::Key::Key_5:            return fcpp::io::Keyboard::Num5;
    case Qt::Key::Key_6:            return fcpp::io::Keyboard::Num6;
    case Qt::Key::Key_7:            return fcpp::io::Keyboard::Num7;
    case Qt::Key::Key_8:            return fcpp::io::Keyboard::Num8;
    case Qt::Key::Key_9:            return fcpp::io::Keyboard::Num9;
    case Qt::Key::Key_Escape:       return fcpp::io::Keyboard::Escape;
    case Qt::Key::Key_Control:      return fcpp::io::Keyboard::LControl;
    case Qt::Key::Key_Shift:        return fcpp::io::Keyboard::LShift;
    case Qt::Key::Key_Alt:          return fcpp::io::Keyboard::LAlt;
    case Qt::Key::Key_Meta:         return fcpp::io::Keyboard::System;
    case Qt::Key::Key_AltGr:        return fcpp::io::Keyboard::RAlt;
    case Qt::Key::Key_Menu:         return fcpp::io::Keyboard::Menu;
    case Qt::Key::Key_BracketLeft:  return fcpp::io::Keyboard::LBracket;
    case Qt::Key::Key_BracketRight: return fcpp::io::Keyboard::RBracket;
    case Qt::Key::Key_Semicolon:    return fcpp::io::Keyboard::Semicolon;
    case Qt::Key::Key_Comma:        return fcpp::io::Keyboard::Comma;
    case Qt::Key::Key_Period:       return fcpp::io::Keyboard::Period;
    case Qt::Key::Key_QuoteDbl:     return fcpp::io::Keyboard::Quote;
    case Qt::Key::Key_Slash:        return fcpp::io::Keyboard::Slash;
    case Qt::Key::Key_Backslash:    return fcpp::io::Keyboard::Backslash;
    case Qt::Key::Key_AsciiTilde:   return fcpp::io::Keyboard::Tilde;
    case Qt::Key::Key_Equal:        return fcpp::io::Keyboard::Equal;
    case Qt::Key::Key_hyphen:       return fcpp::io::Keyboard::Hyphen;
    case Qt::Key::Key_Space:        return fcpp::io::Keyboard::Space;
    case Qt::Key::Key_Return:
    case Qt::Key::Key_Enter:        return fcpp::io::Keyboard::Enter;
    case Qt::Key::Key_Backspace:    return fcpp::io::Keyboard::Backspace;
    case Qt::Key::Key_Tab:          return fcpp::io::Keyboard::Tab;
    case Qt::Key::Key_PageUp:       return fcpp::io::Keyboard::PageUp;
    case Qt::Key::Key_PageDown:     return fcpp::io::Keyboard::PageDown;
    case Qt::Key::Key_End:          return fcpp::io::Keyboard::End;
    case Qt::Key::Key_Home:         return fcpp::io::Keyboard::Home;
    case Qt::Key::Key_Insert:       return fcpp::io::Keyboard::Insert;
    case Qt::Key::Key_Delete:       return fcpp::io::Keyboard::Delete;
    case Qt::Key::Key_Plus:         return fcpp::io::Keyboard::Add;
    case Qt::Key::Key_Minus:        return fcpp::io::Keyboard::Subtract;
    case Qt::Key::Key_Asterisk:     return fcpp::io::Keyboard::Multiply;
    case Qt::Key::Key_Percent:      return fcpp::io::Keyboard::Divide;
    case Qt::Key::Key_Left:         return fcpp::io::Keyboard::Left;
    case Qt::Key::Key_Right:        return fcpp::io::Keyboard::Right;
    case Qt::Key::Key_Up:           return fcpp::io::Keyboard::Up;
    case Qt::Key::Key_Down:         return fcpp::io::Keyboard::Down;
    case Qt::Key::Key_F1:           return fcpp::io::Keyboard::F1;
    case Qt::Key::Key_F2:           return fcpp::io::Keyboard::F2;
    case Qt::Key::Key_F3:           return fcpp::io::Keyboard::F3;
    case Qt::Key::Key_F4:           return fcpp::io::Keyboard::F4;
    case Qt::Key::Key_F5:           return fcpp::io::Keyboard::F5;
    case Qt::Key::Key_F6:           return fcpp::io::Keyboard::F6;
    case Qt::Key::Key_F7:           return fcpp::io::Keyboard::F7;
    case Qt::Key::Key_F8:           return fcpp::io::Keyboard::F8;
    case Qt::Key::Key_F9:           return fcpp::io::Keyboard::F9;
    case Qt::Key::Key_F10:          return fcpp::io::Keyboard::F10;
    case Qt::Key::Key_F11:          return fcpp::io::Keyboard::F11;
    case Qt::Key::Key_F12:          return fcpp::io::Keyboard::F12;
    case Qt::Key::Key_Pause:        return fcpp::io::Keyboard::Pause;
    default:                        return fcpp::io::Keyboard::Unknown;
    }
}
int util::keyMap(const fcpp::io::Keyboard key)
{
    switch (key)
    {
    case fcpp::io::Keyboard::A:         return Qt::Key::Key_A;
    case fcpp::io::Keyboard::B:         return Qt::Key::Key_B;
    case fcpp::io::Keyboard::C:         return Qt::Key::Key_C;
    case fcpp::io::Keyboard::D:         return Qt::Key::Key_D;
    case fcpp::io::Keyboard::E:         return Qt::Key::Key_E;
    case fcpp::io::Keyboard::F:         return Qt::Key::Key_F;
    case fcpp::io::Keyboard::G:         return Qt::Key::Key_G;
    case fcpp::io::Keyboard::H:         return Qt::Key::Key_H;
    case fcpp::io::Keyboard::I:         return Qt::Key::Key_I;
    case fcpp::io::Keyboard::J:         return Qt::Key::Key_J;
    case fcpp::io::Keyboard::K:         return Qt::Key::Key_K;
    case fcpp::io::Keyboard::L:         return Qt::Key::Key_L;
    case fcpp::io::Keyboard::M:         return Qt::Key::Key_M;
    case fcpp::io::Keyboard::N:         return Qt::Key::Key_N;
    case fcpp::io::Keyboard::O:         return Qt::Key::Key_O;
    case fcpp::io::Keyboard::P:         return Qt::Key::Key_P;
    case fcpp::io::Keyboard::Q:         return Qt::Key::Key_Q;
    case fcpp::io::Keyboard::R:         return Qt::Key::Key_R;
    case fcpp::io::Keyboard::S:         return Qt::Key::Key_S;
    case fcpp::io::Keyboard::T:         return Qt::Key::Key_T;
    case fcpp::io::Keyboard::U:         return Qt::Key::Key_U;
    case fcpp::io::Keyboard::V:         return Qt::Key::Key_V;
    case fcpp::io::Keyboard::W:         return Qt::Key::Key_W;
    case fcpp::io::Keyboard::X:         return Qt::Key::Key_X;
    case fcpp::io::Keyboard::Y:         return Qt::Key::Key_Y;
    case fcpp::io::Keyboard::Z:         return Qt::Key::Key_Z;
    case fcpp::io::Keyboard::Num0:      return Qt::Key::Key_0;
    case fcpp::io::Keyboard::Num1:      return Qt::Key::Key_1;
    case fcpp::io::Keyboard::Num2:      return Qt::Key::Key_2;
    case fcpp::io::Keyboard::Num3:      return Qt::Key::Key_3;
    case fcpp::io::Keyboard::Num4:      return Qt::Key::Key_4;
    case fcpp::io::Keyboard::Num5:      return Qt::Key::Key_5;
    case fcpp::io::Keyboard::Num6:      return Qt::Key::Key_6;
    case fcpp::io::Keyboard::Num7:      return Qt::Key::Key_7;
    case fcpp::io::Keyboard::Num8:      return Qt::Key::Key_8;
    case fcpp::io::Keyboard::Num9:      return Qt::Key::Key_9;
    case fcpp::io::Keyboard::Escape:    return Qt::Key::Key_Escape;
    case fcpp::io::Keyboard::LControl:  return Qt::Key::Key_Control;
    case fcpp::io::Keyboard::LShift:    return Qt::Key::Key_Shift;
    case fcpp::io::Keyboard::LAlt:      return Qt::Key::Key_Alt;
    case fcpp::io::Keyboard::System:    return Qt::Key::Key_Meta;
    case fcpp::io::Keyboard::RControl:  return Qt::Key::Key_Control;
    case fcpp::io::Keyboard::RShift:    return Qt::Key::Key_Shift;
    case fcpp::io::Keyboard::RAlt:      return Qt::Key::Key_AltGr;
    case fcpp::io::Keyboard::Menu:      return Qt::Key::Key_Menu;
    case fcpp::io::Keyboard::LBracket:  return Qt::Key::Key_BracketLeft;
    case fcpp::io::Keyboard::RBracket:  return Qt::Key::Key_BracketRight;
    case fcpp::io::Keyboard::Semicolon: return Qt::Key::Key_Semicolon;
    case fcpp::io::Keyboard::Comma:     return Qt::Key::Key_Comma;
    case fcpp::io::Keyboard::Period:    return Qt::Key::Key_Period;
    case fcpp::io::Keyboard::Quote:     return Qt::Key::Key_QuoteDbl;
    case fcpp::io::Keyboard::Slash:     return Qt::Key::Key_Slash;
    case fcpp::io::Keyboard::Backslash: return Qt::Key::Key_Backslash;
    case fcpp::io::Keyboard::Tilde:     return Qt::Key::Key_AsciiTilde;
    case fcpp::io::Keyboard::Equal:     return Qt::Key::Key_Equal;
    case fcpp::io::Keyboard::Hyphen:    return Qt::Key::Key_hyphen;
    case fcpp::io::Keyboard::Space:     return Qt::Key::Key_Space;
    case fcpp::io::Keyboard::Enter:     return Qt::Key::Key_Return;
    case fcpp::io::Keyboard::Backspace: return Qt::Key::Key_Backspace;
    case fcpp::io::Keyboard::Tab:       return Qt::Key::Key_Tab;
    case fcpp::io::Keyboard::PageUp:    return Qt::Key::Key_PageUp;
    case fcpp::io::Keyboard::PageDown:  return Qt::Key::Key_PageDown;
    case fcpp::io::Keyboard::End:       return Qt::Key::Key_End;
    case fcpp::io::Keyboard::Home:      return Qt::Key::Key_Home;
    case fcpp::io::Keyboard::Insert:    return Qt::Key::Key_Insert;
    case fcpp::io::Keyboard::Delete:    return Qt::Key::Key_Delete;
    case fcpp::io::Keyboard::Add:       return Qt::Key::Key_Plus;
    case fcpp::io::Keyboard::Subtract:  return Qt::Key::Key_Minus;
    case fcpp::io::Keyboard::Multiply:  return Qt::Key::Key_Asterisk;
    case fcpp::io::Keyboard::Divide:    return Qt::Key::Key_Percent;
    case fcpp::io::Keyboard::Left:      return Qt::Key::Key_Left;
    case fcpp::io::Keyboard::Right:     return Qt::Key::Key_Right;
    case fcpp::io::Keyboard::Up:        return Qt::Key::Key_Up;
    case fcpp::io::Keyboard::Down:      return Qt::Key::Key_Down;
    case fcpp::io::Keyboard::Numpad0:   return Qt::Key::Key_0;
    case fcpp::io::Keyboard::Numpad1:   return Qt::Key::Key_1;
    case fcpp::io::Keyboard::Numpad2:   return Qt::Key::Key_2;
    case fcpp::io::Keyboard::Numpad3:   return Qt::Key::Key_3;
    case fcpp::io::Keyboard::Numpad4:   return Qt::Key::Key_4;
    case fcpp::io::Keyboard::Numpad5:   return Qt::Key::Key_5;
    case fcpp::io::Keyboard::Numpad6:   return Qt::Key::Key_6;
    case fcpp::io::Keyboard::Numpad7:   return Qt::Key::Key_7;
    case fcpp::io::Keyboard::Numpad8:   return Qt::Key::Key_8;
    case fcpp::io::Keyboard::Numpad9:   return Qt::Key::Key_9;
    case fcpp::io::Keyboard::F1:        return Qt::Key::Key_F1;
    case fcpp::io::Keyboard::F2:        return Qt::Key::Key_F2;
    case fcpp::io::Keyboard::F3:        return Qt::Key::Key_F3;
    case fcpp::io::Keyboard::F4:        return Qt::Key::Key_F4;
    case fcpp::io::Keyboard::F5:        return Qt::Key::Key_F5;
    case fcpp::io::Keyboard::F6:        return Qt::Key::Key_F6;
    case fcpp::io::Keyboard::F7:        return Qt::Key::Key_F7;
    case fcpp::io::Keyboard::F8:        return Qt::Key::Key_F8;
    case fcpp::io::Keyboard::F9:        return Qt::Key::Key_F9;
    case fcpp::io::Keyboard::F10:       return Qt::Key::Key_F10;
    case fcpp::io::Keyboard::F11:       return Qt::Key::Key_F11;
    case fcpp::io::Keyboard::F12:       return Qt::Key::Key_F12;
    case fcpp::io::Keyboard::Pause:     return Qt::Key::Key_Pause;
    default:                            return Qt::Key::Key_unknown;
    }
}

QString util::joystickToString(const fcpp::io::Joystick button)
{
    switch (button)
    {
    case fcpp::io::Joystick::Button0:  return "Button0";
    case fcpp::io::Joystick::Button1:  return "Button1";
    case fcpp::io::Joystick::Button2:  return "Button2";
    case fcpp::io::Joystick::Button3:  return "Button3";
    case fcpp::io::Joystick::Button4:  return "Button4";
    case fcpp::io::Joystick::Button5:  return "Button5";
    case fcpp::io::Joystick::Button6:  return "Button6";
    case fcpp::io::Joystick::Button7:  return "Button7";
    case fcpp::io::Joystick::Button8:  return "Button8";
    case fcpp::io::Joystick::Button9:  return "Button9";
    case fcpp::io::Joystick::Button10: return "Button10";
    case fcpp::io::Joystick::Up:       return "Up";
    case fcpp::io::Joystick::Down:     return "Down";
    case fcpp::io::Joystick::Left:     return "Left";
    case fcpp::io::Joystick::Right:    return "Right";
    case fcpp::io::Joystick::Axis0P:   return "Axis0P";
    case fcpp::io::Joystick::Axis0N:   return "Axis0N";
    case fcpp::io::Joystick::Axis1P:   return "Axis1P";
    case fcpp::io::Joystick::Axis1N:   return "Axis1N";
    case fcpp::io::Joystick::Axis2P:   return "Axis2P";
    case fcpp::io::Joystick::Axis2N:   return "Axis2N";
    case fcpp::io::Joystick::Axis3P:   return "Axis3P";
    case fcpp::io::Joystick::Axis3N:   return "Axis3N";
    case fcpp::io::Joystick::Axis4P:   return "Axis4P";
    case fcpp::io::Joystick::Axis5P:   return "Axis5P";
    default:                           return "Unknown";
    }
}
fcpp::io::Joystick util::stringToJoystick(const QString& string)
{
    static const QHash<QString, fcpp::io::Joystick> map =
    {
        {"Button0", fcpp::io::Joystick::Button0},
        {"Button1", fcpp::io::Joystick::Button1},
        {"Button2", fcpp::io::Joystick::Button2},
        {"Button3", fcpp::io::Joystick::Button3},
        {"Button4", fcpp::io::Joystick::Button4},
        {"Button5", fcpp::io::Joystick::Button5},
        {"Button6", fcpp::io::Joystick::Button6},
        {"Button7", fcpp::io::Joystick::Button7},
        {"Button8", fcpp::io::Joystick::Button8},
        {"Button9", fcpp::io::Joystick::Button9},
        {"Button10", fcpp::io::Joystick::Button10},
        {"Up", fcpp::io::Joystick::Up},
        {"Down", fcpp::io::Joystick::Down},
        {"Left", fcpp::io::Joystick::Left},
        {"Right", fcpp::io::Joystick::Right},
        {"Axis0P", fcpp::io::Joystick::Axis0P},
        {"Axis0N", fcpp::io::Joystick::Axis0N},
        {"Axis1P", fcpp::io::Joystick::Axis1P},
        {"Axis1N", fcpp::io::Joystick::Axis1N},
        {"Axis2P", fcpp::io::Joystick::Axis2P},
        {"Axis2N", fcpp::io::Joystick::Axis2N},
        {"Axis3P", fcpp::io::Joystick::Axis3P},
        {"Axis3N", fcpp::io::Joystick::Axis3N},
        {"Axis4P", fcpp::io::Joystick::Axis4P},
        {"Axis5P", fcpp::io::Joystick::Axis5P}
    };
    return map[string];
}

void util::saveInputBind(const QString& path)
{
    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup("KeyboardPort1");
    settings.setValue("A", KeyToString(keyMap(gConfig.emu.keyboardMap[0][0])));
    settings.setValue("B", KeyToString(keyMap(gConfig.emu.keyboardMap[0][1])));
    settings.setValue("Select", KeyToString(keyMap(gConfig.emu.keyboardMap[0][2])));
    settings.setValue("Start", KeyToString(keyMap(gConfig.emu.keyboardMap[0][3])));
    settings.setValue("Up", KeyToString(keyMap(gConfig.emu.keyboardMap[0][4])));
    settings.setValue("Down", KeyToString(keyMap(gConfig.emu.keyboardMap[0][5])));
    settings.setValue("Left", KeyToString(keyMap(gConfig.emu.keyboardMap[0][6])));
    settings.setValue("Right", KeyToString(keyMap(gConfig.emu.keyboardMap[0][7])));
    settings.setValue("TurboA", KeyToString(keyMap(gConfig.emu.keyboardMap[0][8])));
    settings.setValue("TurboB", KeyToString(keyMap(gConfig.emu.keyboardMap[0][9])));
    settings.endGroup();

    settings.beginGroup("KeyboardPort2");
    settings.setValue("A", KeyToString(keyMap(gConfig.emu.keyboardMap[1][0])));
    settings.setValue("B", KeyToString(keyMap(gConfig.emu.keyboardMap[1][1])));
    settings.setValue("Select", KeyToString(keyMap(gConfig.emu.keyboardMap[1][2])));
    settings.setValue("Start", KeyToString(keyMap(gConfig.emu.keyboardMap[1][3])));
    settings.setValue("Up", KeyToString(keyMap(gConfig.emu.keyboardMap[1][4])));
    settings.setValue("Down", KeyToString(keyMap(gConfig.emu.keyboardMap[1][5])));
    settings.setValue("Left", KeyToString(keyMap(gConfig.emu.keyboardMap[1][6])));
    settings.setValue("Right", KeyToString(keyMap(gConfig.emu.keyboardMap[1][7])));
    settings.setValue("TurboA", KeyToString(keyMap(gConfig.emu.keyboardMap[1][8])));
    settings.setValue("TurboB", KeyToString(keyMap(gConfig.emu.keyboardMap[1][9])));
    settings.endGroup();

    settings.beginGroup("JoystickPort1");
    settings.setValue("Port", gConfig.emu.joystickPort[0]);
    settings.setValue("A", joystickToString(gConfig.emu.joystickMap[0][0]));
    settings.setValue("B", joystickToString(gConfig.emu.joystickMap[0][1]));
    settings.setValue("Select", joystickToString(gConfig.emu.joystickMap[0][2]));
    settings.setValue("Start", joystickToString(gConfig.emu.joystickMap[0][3]));
    settings.setValue("Up", joystickToString(gConfig.emu.joystickMap[0][4]));
    settings.setValue("Down", joystickToString(gConfig.emu.joystickMap[0][5]));
    settings.setValue("Left", joystickToString(gConfig.emu.joystickMap[0][6]));
    settings.setValue("Right", joystickToString(gConfig.emu.joystickMap[0][7]));
    settings.setValue("TurboA", joystickToString(gConfig.emu.joystickMap[0][8]));
    settings.setValue("TurboB", joystickToString(gConfig.emu.joystickMap[0][9]));
    settings.endGroup();

    settings.beginGroup("JoystickPort2");
    settings.setValue("Port", gConfig.emu.joystickPort[1]);
    settings.setValue("A", joystickToString(gConfig.emu.joystickMap[1][0]));
    settings.setValue("B", joystickToString(gConfig.emu.joystickMap[1][1]));
    settings.setValue("Select", joystickToString(gConfig.emu.joystickMap[1][2]));
    settings.setValue("Start", joystickToString(gConfig.emu.joystickMap[1][3]));
    settings.setValue("Up", joystickToString(gConfig.emu.joystickMap[1][4]));
    settings.setValue("Down", joystickToString(gConfig.emu.joystickMap[1][5]));
    settings.setValue("Left", joystickToString(gConfig.emu.joystickMap[1][6]));
    settings.setValue("Right", joystickToString(gConfig.emu.joystickMap[1][7]));
    settings.setValue("TurboA", joystickToString(gConfig.emu.joystickMap[1][8]));
    settings.setValue("TurboB", joystickToString(gConfig.emu.joystickMap[1][9]));
    settings.endGroup();
}
void util::loadInputBind(const QString& path)
{
    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup("KeyboardPort1");
    gConfig.emu.keyboardMap[0][0] =
        keyMap(StringToKey(settings.value("A", KeyToString(keyMap(gConfig.emu.keyboardMap[0][0]))).toString()));
    gConfig.emu.keyboardMap[0][1] =
        keyMap(StringToKey(settings.value("B", KeyToString(keyMap(gConfig.emu.keyboardMap[0][1]))).toString()));
    gConfig.emu.keyboardMap[0][2] =
        keyMap(StringToKey(settings.value("Select", KeyToString(keyMap(gConfig.emu.keyboardMap[0][2]))).toString()));
    gConfig.emu.keyboardMap[0][3] =
        keyMap(StringToKey(settings.value("Start", KeyToString(keyMap(gConfig.emu.keyboardMap[0][3]))).toString()));
    gConfig.emu.keyboardMap[0][4] =
        keyMap(StringToKey(settings.value("Up", KeyToString(keyMap(gConfig.emu.keyboardMap[0][4]))).toString()));
    gConfig.emu.keyboardMap[0][5] =
        keyMap(StringToKey(settings.value("Down", KeyToString(keyMap(gConfig.emu.keyboardMap[0][5]))).toString()));
    gConfig.emu.keyboardMap[0][6] =
        keyMap(StringToKey(settings.value("Left", KeyToString(keyMap(gConfig.emu.keyboardMap[0][6]))).toString()));
    gConfig.emu.keyboardMap[0][7] =
        keyMap(StringToKey(settings.value("Right", KeyToString(keyMap(gConfig.emu.keyboardMap[0][7]))).toString()));
    gConfig.emu.keyboardMap[0][8] =
        keyMap(StringToKey(settings.value("TurboA", KeyToString(keyMap(gConfig.emu.keyboardMap[0][8]))).toString()));
    gConfig.emu.keyboardMap[0][9] =
        keyMap(StringToKey(settings.value("TurboB", KeyToString(keyMap(gConfig.emu.keyboardMap[0][9]))).toString()));
    settings.endGroup();

    settings.beginGroup("KeyboardPort2");
    gConfig.emu.keyboardMap[1][0] =
        keyMap(StringToKey(settings.value("A", KeyToString(keyMap(gConfig.emu.keyboardMap[1][0]))).toString()));
    gConfig.emu.keyboardMap[1][1] =
        keyMap(StringToKey(settings.value("B", KeyToString(keyMap(gConfig.emu.keyboardMap[1][1]))).toString()));
    gConfig.emu.keyboardMap[1][2] =
        keyMap(StringToKey(settings.value("Select", KeyToString(keyMap(gConfig.emu.keyboardMap[1][2]))).toString()));
    gConfig.emu.keyboardMap[1][3] =
        keyMap(StringToKey(settings.value("Start", KeyToString(keyMap(gConfig.emu.keyboardMap[1][3]))).toString()));
    gConfig.emu.keyboardMap[1][4] =
        keyMap(StringToKey(settings.value("Up", KeyToString(keyMap(gConfig.emu.keyboardMap[1][4]))).toString()));
    gConfig.emu.keyboardMap[1][5] =
        keyMap(StringToKey(settings.value("Down", KeyToString(keyMap(gConfig.emu.keyboardMap[1][5]))).toString()));
    gConfig.emu.keyboardMap[1][6] =
        keyMap(StringToKey(settings.value("Left", KeyToString(keyMap(gConfig.emu.keyboardMap[1][6]))).toString()));
    gConfig.emu.keyboardMap[1][7] =
        keyMap(StringToKey(settings.value("Right", KeyToString(keyMap(gConfig.emu.keyboardMap[1][7]))).toString()));
    gConfig.emu.keyboardMap[1][8] =
        keyMap(StringToKey(settings.value("TurboA", KeyToString(keyMap(gConfig.emu.keyboardMap[1][8]))).toString()));
    gConfig.emu.keyboardMap[1][9] =
        keyMap(StringToKey(settings.value("TurboB", KeyToString(keyMap(gConfig.emu.keyboardMap[1][9]))).toString()));
    settings.endGroup();

    settings.beginGroup("JoystickPort1");
    gConfig.emu.joystickPort[0] = settings.value("Port", gConfig.emu.joystickPort[0]).toInt();
    gConfig.emu.joystickMap[0][0] =
        stringToJoystick(settings.value("A", joystickToString(gConfig.emu.joystickMap[0][0])).toString());
    gConfig.emu.joystickMap[0][1] =
        stringToJoystick(settings.value("B", joystickToString(gConfig.emu.joystickMap[0][1])).toString());
    gConfig.emu.joystickMap[0][2] =
        stringToJoystick(settings.value("Select", joystickToString(gConfig.emu.joystickMap[0][2])).toString());
    gConfig.emu.joystickMap[0][3] =
        stringToJoystick(settings.value("Start", joystickToString(gConfig.emu.joystickMap[0][3])).toString());
    gConfig.emu.joystickMap[0][4] =
        stringToJoystick(settings.value("Up", joystickToString(gConfig.emu.joystickMap[0][4])).toString());
    gConfig.emu.joystickMap[0][5] =
        stringToJoystick(settings.value("Down", joystickToString(gConfig.emu.joystickMap[0][5])).toString());
    gConfig.emu.joystickMap[0][6] =
        stringToJoystick(settings.value("Left", joystickToString(gConfig.emu.joystickMap[0][6])).toString());
    gConfig.emu.joystickMap[0][7] =
        stringToJoystick(settings.value("Right", joystickToString(gConfig.emu.joystickMap[0][7])).toString());
    gConfig.emu.joystickMap[0][8] =
        stringToJoystick(settings.value("TurboA", joystickToString(gConfig.emu.joystickMap[0][8])).toString());
    gConfig.emu.joystickMap[0][9] =
        stringToJoystick(settings.value("TurboB", joystickToString(gConfig.emu.joystickMap[0][9])).toString());
    settings.endGroup();

    settings.beginGroup("JoystickPort2");
    gConfig.emu.joystickPort[1] = settings.value("Port", gConfig.emu.joystickPort[1]).toInt();
    gConfig.emu.joystickMap[1][0] =
        stringToJoystick(settings.value("A", joystickToString(gConfig.emu.joystickMap[1][0])).toString());
    gConfig.emu.joystickMap[1][1] =
        stringToJoystick(settings.value("B", joystickToString(gConfig.emu.joystickMap[1][1])).toString());
    gConfig.emu.joystickMap[1][2] =
        stringToJoystick(settings.value("Select", joystickToString(gConfig.emu.joystickMap[1][2])).toString());
    gConfig.emu.joystickMap[1][3] =
        stringToJoystick(settings.value("Start", joystickToString(gConfig.emu.joystickMap[1][3])).toString());
    gConfig.emu.joystickMap[1][4] =
        stringToJoystick(settings.value("Up", joystickToString(gConfig.emu.joystickMap[1][4])).toString());
    gConfig.emu.joystickMap[1][5] =
        stringToJoystick(settings.value("Down", joystickToString(gConfig.emu.joystickMap[1][5])).toString());
    gConfig.emu.joystickMap[1][6] =
        stringToJoystick(settings.value("Left", joystickToString(gConfig.emu.joystickMap[1][6])).toString());
    gConfig.emu.joystickMap[1][7] =
        stringToJoystick(settings.value("Right", joystickToString(gConfig.emu.joystickMap[1][7])).toString());
    gConfig.emu.joystickMap[1][8] =
        stringToJoystick(settings.value("TurboA", joystickToString(gConfig.emu.joystickMap[1][8])).toString());
    gConfig.emu.joystickMap[1][9] =
        stringToJoystick(settings.value("TurboB", joystickToString(gConfig.emu.joystickMap[1][9])).toString());
    settings.endGroup();
}

bool util::saveState(const QString& path)
{
    if (gConfig.gui.lastPlay.name.isEmpty()) return false;
    fcpp::core::Snapshot state = gEmulator.getQuickSnapshot();
    std::uint64_t size = state.size();
    if (!size) return false;
    return fcpp::util::archive::save(path.toStdString(), gConfig.gui.lastPlay.name.toStdString(), reinterpret_cast<char*>(state.data()), size);
}
bool util::loadState(const QString& path)
{
    std::uint64_t size = 0;
    fcpp::core::Snapshot state{};
    if (fcpp::util::archive::load(path.toStdString(), gConfig.gui.lastPlay.name.toStdString(), reinterpret_cast<char*>(state.data()), size))
    {
        state.setSize(static_cast<std::size_t>(size));
        gEmulator.setQuickSnapshot(state);
        return true;
    }
    return false;
}
