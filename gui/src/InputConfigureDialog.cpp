#include <QKeyEvent>
#include <QCloseEvent>
#include <QKeySequence>
#include <QTimer>

#include "Util.hpp"
#include "InputConfigureDialog.hpp"
#include "ui_InputConfigureDialog.h"

InputConfigureDialog::InputConfigureDialog(QWidget* const parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::InputConfigureDialog>())
{
    ui->setupUi(this);

    buttonList[0] = ui->push_button_button_a;
    buttonList[1] = ui->push_button_button_b;
    buttonList[2] = ui->push_button_button_select;
    buttonList[3] = ui->push_button_button_start;
    buttonList[4] = ui->push_button_button_up;
    buttonList[5] = ui->push_button_button_down;
    buttonList[6] = ui->push_button_button_left;
    buttonList[7] = ui->push_button_button_right;
    buttonList[8] = ui->push_button_button_turbo_a;
    buttonList[9] = ui->push_button_button_turbo_b;

    for (int i = 0; i < 10; i++)
    {
        QObject::connect(buttonList[i], &QPushButton::clicked, this,
            [=]()
            {
                buttonList[i]->clearFocus();
                if (waitingKey >= 0) return;
                buttonList[waitingKey = i]->setText(tr("Waiting..."));
                if (joystickMapFlag) updateJoystick();
            });
    }

    QObject::connect(ui->combo_box_device, qOverload<int>(&QComboBox::currentIndexChanged), this,
        [=](const int index)
        {
            waitingKey = -1;
            joystickMapFlag = index != 0;
            if (joystickMapFlag)
            {
                joystickPort = index - 1;
                showJoystickMap();
            }
            else showKeyboardMap();
        });

    QObject::connect(this, &QDialog::finished, [=]() { waitingKey = -1; });
}
InputConfigureDialog::~InputConfigureDialog() noexcept = default;

void InputConfigureDialog::setKeyboard(fcpp::io::Keyboard(&keys)[10])
{
    for (int i = 0; i < 10; i++) keyboardMap[i] = util::keyMap(keys[i]);
}
void InputConfigureDialog::getKeyboard(fcpp::io::Keyboard(&keys)[10])
{
    for (int i = 0; i < 10; i++) keys[i] = util::keyMap(keyboardMap[i]);
}
void InputConfigureDialog::setJoystick(fcpp::io::Joystick(&buttons)[10])
{
    for (int i = 0; i < 10; i++) joystickMap[i] = buttons[i];
}
void InputConfigureDialog::getJoystick(fcpp::io::Joystick(&buttons)[10])
{
    for (int i = 0; i < 10; i++) buttons[i] = joystickMap[i];
}
void InputConfigureDialog::setJoystickPort(const int port)
{
    joystickPort = port;
}
int InputConfigureDialog::getJoystickPort()
{
    return joystickPort;
}
void InputConfigureDialog::setTurboSpeed(const int v)
{
    ui->horizontal_slider_turbo_speed->setValue(v);
}
int InputConfigureDialog::getTurboSpeed()
{
    return ui->horizontal_slider_turbo_speed->value();
}
void InputConfigureDialog::setJoystickHelper(std::unique_ptr<fcpp::io::Controller::JoystickHelper> helper)
{
    joystickHelper.swap(helper);
    for (int i = 0; i < joystickHelper->joystickCount(); i++)
        ui->combo_box_device->addItem(QStringLiteral("Joystick port %1").arg(i));
}
void InputConfigureDialog::showKeyboardMap()
{
    for (int i = 0; i < 10; i++) buttonList[i]->setText(QKeySequence{ keyboardMap[i] }.toString());
}
void InputConfigureDialog::showJoystickMap()
{
    for (int i = 0; i < 10; i++) buttonList[i]->setText(util::joystickToString(joystickMap[i]));
}

void InputConfigureDialog::updateJoystick()
{
    QTimer timer{};
    QEventLoop loop{};
    fcpp::io::Joystick joystick = fcpp::io::Joystick::Unknown;
    joystickHelper->joystickPressed(joystickPort, joystick);
    QObject::connect(&timer, &QTimer::timeout, this,
        [&]()
        {
            if (waitingKey < 0)
            {
                timer.stop();
                loop.quit();
                return;
            }
            if (joystickHelper->joystickPressed(joystickPort, joystick))
            {
                buttonList[waitingKey]->setText(util::joystickToString(joystickMap[waitingKey] = joystick));
                waitingKey = -1;
            }
        });
    timer.start();
    loop.exec();
}
void InputConfigureDialog::keyPressEvent(QKeyEvent* const e)
{
    if (waitingKey >= 0 && !joystickMapFlag)
    {
        buttonList[waitingKey]->setText(QKeySequence{ keyboardMap[waitingKey] = e->key() }.toString());
        waitingKey = -1;
    }
    QDialog::keyPressEvent(e);
}
