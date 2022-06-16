#ifndef FCPP_QFCPP_UTIL_HPP
#define FCPP_QFCPP_UTIL_HPP

#include <QString>
#include <QStringList>

#include "FCPP/IO/InputDevice.hpp"

namespace util
{
    const QStringList& joypadTypeList();

    fcpp::io::Keyboard keyMap(int key);
    int keyMap(fcpp::io::Keyboard key);

    QString joystickToString(fcpp::io::Joystick button);
    fcpp::io::Joystick stringToJoystick(const QString& string);
    
    void saveInputBind(const QString& path);
    void loadInputBind(const QString& path);

    bool saveState(const QString& path);
    bool loadState(const QString& path);
}

#endif
