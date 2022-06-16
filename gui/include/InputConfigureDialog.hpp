#ifndef FCPP_QFCPP_INPUT_CONFIGURE_DIALOG_HPP
#define FCPP_QFCPP_INPUT_CONFIGURE_DIALOG_HPP

#include <memory>

#include <QDialog>

#include "FCPP/IO.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class InputConfigureDialog; }
QT_END_NAMESPACE

class InputConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputConfigureDialog(QWidget* parent = nullptr);
    ~InputConfigureDialog() noexcept override;

    void setKeyboard(fcpp::io::Keyboard(&keys)[10]);
    void getKeyboard(fcpp::io::Keyboard(&keys)[10]);
    void setJoystick(fcpp::io::Joystick(&buttons)[10]);
    void getJoystick(fcpp::io::Joystick(&buttons)[10]);
    void setJoystickPort(int port);
    int getJoystickPort();
    void setTurboSpeed(int v);
    int getTurboSpeed();
    void setJoystickHelper(std::unique_ptr<fcpp::io::Controller::JoystickHelper> helper);
    void showKeyboardMap();
    void showJoystickMap();
private:
    void updateJoystick();
    void keyPressEvent(QKeyEvent* e) override;
private:
    bool joystickMapFlag = false;
    int waitingKey = -1;
    int joystickPort = 0;
    std::unique_ptr<fcpp::io::Controller::JoystickHelper> joystickHelper;
    fcpp::io::Joystick joystickMap[10]{};
    int keyboardMap[10]{};
    QPushButton* buttonList[10]{};
private:
    std::unique_ptr<Ui::InputConfigureDialog> ui;
};

#endif
