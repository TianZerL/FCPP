#ifndef FCPP_QFCPP_MAIN_WINDOW_HPP
#define FCPP_QFCPP_MAIN_WINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileInfo>

#include "Config.hpp"
#include "Emulator.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() noexcept override;
private:
    void init();
    void connect();
    void addRom(const QFileInfo& fileInfo);
    void runRom(const QModelIndex& index);
    void runRom(const QString& name);
    void scanRoms(const QString& path);
    void saveRomList();
    void loadRomList();
private:
    void closeEvent(QCloseEvent* e) override;
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
private slots:
    void on_action_open_triggered();
    void on_action_remove_triggered();
    void on_action_clear_triggered();
    void on_action_refresh_triggered();
    void on_action_play_triggered();
    void on_action_save_state_triggered();
    void on_action_load_state_triggered();
    void on_action_settings_triggered();
    void on_action_about_triggered();
private:
    QStandardItemModel romsModel{};
private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif
