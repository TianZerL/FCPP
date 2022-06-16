#include <QFileDialog>
#include <QStyleFactory>

#include "Emulator.hpp"
#include "Util.hpp"
#include "ColorSquareLabel.hpp"
#include "InputConfigureDialog.hpp"
#include "SettingDialog.hpp"
#include "ui_SettingDialog.h"

SettingDialog::SettingDialog(QWidget* const parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::SettingDialog>())
{
    ui->setupUi(this);
    init();
    connect();
}

SettingDialog::~SettingDialog() noexcept = default;

void SettingDialog::init()
{
    ui->check_box_exit_confirmation->setChecked(gConfig.gui.exitConfirmation);
    ui->check_box_emu_full_screen->setChecked(gConfig.emu.fullScreen);
    ui->check_box_emu_vsync->setChecked(gConfig.emu.vsync);
    ui->double_spin_box_emu_scale->setValue(gConfig.emu.scale);
    ui->double_spin_box_emu_fps->setValue(gConfig.emu.fpsLimit);
    ui->spin_box_emu_sample_rate->setValue(gConfig.emu.sampleRate);
    ui->spin_box_emu_tape_length->setValue(gConfig.emu.tapeLength);
    ui->spin_box_emu_sprite_limit->setValue(gConfig.emu.spriteLimit);
    ui->horizontal_slider_emu_volume->setValue(gConfig.emu.volume);
    romFoldersModel.setStringList(gConfig.gui.romFolders);
    ui->list_view_rom_folders->setModel(&romFoldersModel);
    ui->line_edit_palette_path->setText(gConfig.gui.palettePath);
    ui->combo_box_gui_style->addItems(QStyleFactory::keys());
    ui->combo_box_gui_style->setCurrentText(gConfig.gui.styleName);
    ui->combo_box_input_port1_joypad_type->addItems(util::joypadTypeList());
    ui->combo_box_input_port1_joypad_type->setCurrentText(util::joypadTypeList().at(static_cast<int>(gConfig.emu.joypadType[0])));
    ui->combo_box_input_port2_joypad_type->addItems(util::joypadTypeList());
    ui->combo_box_input_port2_joypad_type->setCurrentText(util::joypadTypeList().at(static_cast<int>(gConfig.emu.joypadType[1])));
    for (int i = 0; i < fcpp::io::manager::count(); i++) ui->combo_box_emu_engine->addItem(fcpp::io::manager::name(i));
    ui->combo_box_emu_engine->setCurrentIndex(gConfig.emu.engineIdx);
    auto engineInfo = fcpp::io::manager::info(gConfig.emu.engineIdx);
    for (int i = 0; i < engineInfo->getRenderDriverCount(); i++) ui->combo_box_emu_render_driver->addItem(engineInfo->getRenderDriverName(i));
    ui->combo_box_emu_render_driver->setCurrentIndex(gConfig.emu.renderDriverIdx);

    auto gridLayout = new QGridLayout{};
    for (int i = 0; i < fcpp::io::PaletteTable::Size; i++)
    {
        auto argb = gConfig.emu.paletteTable.get(i);
        auto colorSquareLabe = new ColorSquareLabel(argb, ui->group_box_palette_control);
        colorSquareLabe->setText(QString::number(i, 16).rightJustified(2, '0').toUpper());
        colorSquareLabe->setAlignment(Qt::AlignCenter);
        gridLayout->addWidget(colorSquareLabe, i / 16, i % 16);
    }
    ui->group_box_palette_control->setLayout(gridLayout);
}
void SettingDialog::connect()
{
    QObject::connect(ui->combo_box_gui_style, &QComboBox::textActivated, this,
        [](const QString& text)
        {
            qApp->setStyle(QStyleFactory::create(gConfig.gui.styleName = text));
            qApp->setPalette(qApp->style()->standardPalette());
        });
    QObject::connect(ui->combo_box_emu_engine, qOverload<int>(&QComboBox::activated), this,
        [=](const int idx)
        {
            if (gConfig.emu.engineIdx != idx)
            {
                gConfig.emu.engineIdx = idx;
                ui->combo_box_emu_render_driver->clear();
                auto engineInfo = fcpp::io::manager::info(idx);
                for (int i = 0; i < engineInfo->getRenderDriverCount(); i++) ui->combo_box_emu_render_driver->addItem(engineInfo->getRenderDriverName(i));
                ui->combo_box_emu_render_driver->setCurrentIndex(gConfig.emu.renderDriverIdx = 0);
            }
        });
    QObject::connect(ui->combo_box_emu_render_driver, qOverload<int>(&QComboBox::activated), this,
        [](const int idx) {gConfig.emu.renderDriverIdx = idx; });
    QObject::connect(ui->line_edit_palette_path, &QLineEdit::textChanged, this,
        [=](const QString& text)
        {
            if (text != gConfig.gui.palettePath)
            {
                gConfig.emu.paletteTable.load((gConfig.gui.palettePath = text).toLocal8Bit().constData());
                auto gridLayout = qobject_cast<QGridLayout*>(ui->group_box_palette_control->layout());
                for (int i = 0; i < fcpp::io::PaletteTable::Size; i++)
                {
                    auto argb = gConfig.emu.paletteTable.get(i);
                    qobject_cast<ColorSquareLabel*>(gridLayout->itemAtPosition(i / 16, i % 16)->widget())->setColor(argb);
                }
            }
        });
    QObject::connect(ui->check_box_exit_confirmation, &QCheckBox::stateChanged, this,
        [](const int state) {gConfig.gui.exitConfirmation = state == Qt::CheckState::Checked; });
    QObject::connect(ui->check_box_emu_full_screen, &QCheckBox::stateChanged, this,
        [](const int state) {gConfig.emu.fullScreen = state == Qt::CheckState::Checked; });
    QObject::connect(ui->check_box_emu_vsync, &QCheckBox::stateChanged, this,
        [](const int state) {gConfig.emu.vsync = state == Qt::CheckState::Checked; });
    QObject::connect(ui->double_spin_box_emu_scale, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
        [](const double value) {gConfig.emu.scale = static_cast<float>(value); });
    QObject::connect(ui->double_spin_box_emu_fps, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
        [](const double value) {gConfig.emu.fpsLimit = value; });
    QObject::connect(ui->spin_box_emu_sample_rate, qOverload<int>(&QSpinBox::valueChanged), this,
        [](const int value) {gConfig.emu.sampleRate = value; });
    QObject::connect(ui->spin_box_emu_tape_length, qOverload<int>(&QSpinBox::valueChanged), this,
        [](const int value) {gConfig.emu.tapeLength = value; });
    QObject::connect(ui->spin_box_emu_sprite_limit, qOverload<int>(&QSpinBox::valueChanged), this,
        [](const int value) {gConfig.emu.spriteLimit = value; });
    QObject::connect(ui->horizontal_slider_emu_volume, &QSlider::valueChanged, this,
        [](const int value)
        {
            gConfig.emu.volume = static_cast<float>(value);
            gEmulator.pushTask([](fcpp::io::Controller* controller) { controller->setVolume(gConfig.emu.volume); });
        });

    for (int i = 0; i < fcpp::io::PaletteTable::Size; i++)
    {
        auto gridLayout = qobject_cast<QGridLayout*>(ui->group_box_palette_control->layout());
        auto colorSquareLabel = qobject_cast<ColorSquareLabel*>(gridLayout->itemAtPosition(i / 16, i % 16)->widget());
        QObject::connect(colorSquareLabel, &ColorSquareLabel::colorChanged, this,
            [=](const QColor& color) {gConfig.emu.paletteTable.set(i, color.rgb()); });
    }
}
void SettingDialog::configureInput(const int idx)
{
    InputConfigureDialog inputConfigureDialog{};
    inputConfigureDialog.setWindowTitle(QStringLiteral("Port %1 input configure").arg(idx + 1));;
    inputConfigureDialog.setKeyboard(gConfig.emu.keyboardMap[idx]);
    inputConfigureDialog.setJoystick(gConfig.emu.joystickMap[idx]);
    inputConfigureDialog.setJoystickPort(gConfig.emu.joystickPort[idx]);
    inputConfigureDialog.setTurboSpeed(gConfig.emu.turboButtonSpeed[idx]);
    inputConfigureDialog.setJoystickHelper(fcpp::io::manager::joystickHelper(gConfig.emu.engineIdx));
    inputConfigureDialog.showKeyboardMap();
    if (inputConfigureDialog.exec())
    {
        inputConfigureDialog.getKeyboard(gConfig.emu.keyboardMap[idx]);
        inputConfigureDialog.getJoystick(gConfig.emu.joystickMap[idx]);
        gConfig.emu.joystickPort[idx] = inputConfigureDialog.getJoystickPort();
        gConfig.emu.turboButtonSpeed[idx] = inputConfigureDialog.getTurboSpeed();
        gEmulator.pushTask(
            [=](fcpp::io::Controller* controller)
            {
                controller->setTurboButtonSpeed(idx, gConfig.emu.turboButtonSpeed[idx]);
                controller->setJoystickPort(idx, gConfig.emu.joystickPort[idx]);
                controller->bind(idx, gConfig.emu.keyboardMap[idx]);
                controller->bind(idx, gConfig.emu.joystickMap[idx]);
            });
    }
}

// solts
void SettingDialog::on_push_button_rom_folders_add_clicked()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::Directory);
    if (fileDialog.exec())
    {
        auto paths = fileDialog.selectedFiles();
        gConfig.gui.romFolders.append(paths);
        romFoldersModel.setStringList(gConfig.gui.romFolders);
    }
}
void SettingDialog::on_push_button_rom_folders_remove_clicked()
{
    if (ui->list_view_rom_folders->selectionModel()->hasSelection())
    {
        gConfig.gui.romFolders.removeAt(ui->list_view_rom_folders->currentIndex().row());
        romFoldersModel.setStringList(gConfig.gui.romFolders);
    }
}
void SettingDialog::on_push_button_rom_folders_clear_clicked()
{
    gConfig.gui.romFolders.clear();
    romFoldersModel.setStringList(gConfig.gui.romFolders);
}
void SettingDialog::on_push_button_palette_save_clicked()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilters({ "Palette file (*.pal)", "Any files (*)" });
    fileDialog.setDefaultSuffix("pal");
    fileDialog.selectFile("MyPalette.pal");
    if (fileDialog.exec())
    {
        auto path = fileDialog.selectedFiles().first();
        if (gConfig.emu.paletteTable.save(path.toLocal8Bit().constData())) ui->line_edit_palette_path->setText(path);
    }
}
void SettingDialog::on_push_button_palette_load_clicked()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilters({ "Palette file (*.pal)", "Any files (*)" });
    if (fileDialog.exec()) ui->line_edit_palette_path->setText(fileDialog.selectedFiles().first());
}
void SettingDialog::on_push_button_input_port1_configure_clicked()
{
    configureInput(0);
}
void SettingDialog::on_push_button_input_port2_configure_clicked()
{
    configureInput(1);
}
void SettingDialog::on_push_button_input_presets_save_clicked()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilters({ "QFCPP input preset file (*.txt)" });
    fileDialog.setDefaultSuffix("txt");
    fileDialog.selectFile("Default.txt");
    if (fileDialog.exec()) util::saveInputBind(fileDialog.selectedFiles().first());
}
void SettingDialog::on_push_button_input_presets_load_clicked()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilters({ "QFCPP input preset file (*.txt)" });
    if (fileDialog.exec())
    {
        util::loadInputBind(fileDialog.selectedFiles().first());
        gEmulator.pushTask(
            [](fcpp::io::Controller* controller)
            {
                for (int i = 0; i < 2; i++)
                {
                    controller->setJoystickPort(i, gConfig.emu.joystickPort[i]);
                    controller->bind(i, gConfig.emu.keyboardMap[i]);
                    controller->bind(i, gConfig.emu.joystickMap[i]);
                }
            });
    }
}
