#ifndef FCPP_QFCPP_SETTING_DIALOG_HPP
#define FCPP_QFCPP_SETTING_DIALOG_HPP

#include <memory>

#include <QDialog>
#include <QStringListModel>

#include "Config.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingDialog; }
QT_END_NAMESPACE

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog() noexcept override;
private:
    void init();
    void connect();
    void configureInput(int idx);
private slots:
    void on_push_button_rom_folders_add_clicked();
    void on_push_button_rom_folders_remove_clicked();
    void on_push_button_rom_folders_clear_clicked();
    void on_push_button_palette_save_clicked();
    void on_push_button_palette_load_clicked();
    void on_push_button_input_port1_configure_clicked();
    void on_push_button_input_port2_configure_clicked();
    void on_push_button_input_presets_save_clicked();
    void on_push_button_input_presets_load_clicked();
signals:
    void refresh();
private:
    QStringListModel romFoldersModel{};
private:
    std::unique_ptr<Ui::SettingDialog> ui;
};

#endif
