#include <QCloseEvent>
#include <QDataStream>
#include <QDateTime>
#include <QDropEvent>
#include <QFile>
#include <QFileDialog>
#include <QMimeData>
#include <QMessageBox>
#include <QStyleFactory>

#include "ErrorHandler.hpp"
#include "RomInfo.hpp"
#include "Util.hpp"
#include "SettingDialog.hpp"
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* const parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    init();
    connect();
}

MainWindow::~MainWindow() noexcept = default;

void MainWindow::init()
{
    qApp->setStyle(QStyleFactory::create(gConfig.gui.styleName));
    qApp->setPalette(qApp->style()->standardPalette());
    ui->list_view_roms->setModel(&romsModel);
    ui->action_auto_resume->setChecked(gConfig.gui.autoResume);
    loadRomList();
    ui->action_refresh->trigger();
}
void MainWindow::connect()
{
    QObject::connect(ui->list_view_roms, &QListView::doubleClicked, this,
        [=](const QModelIndex& index) {runRom(index); });
    QObject::connect(ui->list_view_roms, &QListView::customContextMenuRequested, this,
        [=](const QPoint& pos)
        {
            QMenu menu;
            menu.addActions({ ui->action_play, ui->action_remove, ui->action_clear });
            menu.exec(ui->list_view_roms->mapToGlobal(pos));
        });
    QObject::connect(ui->action_auto_resume, &QAction::toggled, this,
        [](const bool checked) { gConfig.gui.autoResume = checked; });
    QObject::connect(ui->action_stop, &QAction::triggered, &gEmulator, &Emulator::stop);
    QObject::connect(ui->action_pause, &QAction::triggered, &gEmulator, &Emulator::pause);
    QObject::connect(ui->action_quick_save, &QAction::triggered, &gEmulator, &Emulator::pushQuickSave);
    QObject::connect(ui->action_quick_load, &QAction::triggered, &gEmulator, &Emulator::pushQuickLoad);
    QObject::connect(ui->action_rewind, &QAction::triggered, &gEmulator, &Emulator::pushRewind);
    QObject::connect(ui->action_reset, &QAction::triggered, &gEmulator, &Emulator::pushReset);
    QObject::connect(&gEmulator, &Emulator::started,
        []()
        {
            if (!gConfig.gui.autoResume) return;
            if (util::loadState(QFCPP_AUTO_SAVE_FILE)) gEmulator.pushQuickLoad();
        });
    QObject::connect(&gEmulator, &Emulator::stopped,
        []()
        {
            if (!gConfig.gui.autoResume) return;
            util::saveState(QFCPP_AUTO_SAVE_FILE);
        });
}
void MainWindow::addRom(const QFileInfo& fileInfo)
{
    if (fileInfo.suffix().compare("nes", Qt::CaseInsensitive)) return;
    RomInfo romInfo{ fileInfo.absoluteFilePath(), fileInfo.fileName() };
    if (!romsModel.findItems(romInfo.name).isEmpty()) return;
    auto item = new QStandardItem{ romInfo.name };
    item->setData(QVariant::fromValue(romInfo), Qt::UserRole);
    romsModel.appendRow(item);
}
void MainWindow::runRom(const QModelIndex& index)
{
    if (gEmulator.running()) gEmulator.stop();
    auto romInfo = index.data(Qt::UserRole).value<RomInfo>();
    if (QFile::exists(romInfo.path))
    {
        gConfig.gui.lastPlay = romInfo;
        if (!gEmulator.run(romInfo.path.toLocal8Bit().constData())) gError.send(QFCPP_ERROR_ROM_NOT_SUPPORTED);
    }
    else
    {
        gError.send(QFCPP_ERROR_ROM_NOT_FOUND);
        romsModel.removeRow(index.row());
    }
}
void MainWindow::runRom(const QString& name)
{
    auto items = romsModel.findItems(name);
    if (!items.isEmpty())
    {
        auto index = romsModel.indexFromItem(items.first());
        runRom(index);
    }
}
void MainWindow::scanRoms(const QString& path)
{
    QDir dir{ path };
    if (!dir.exists()) return;
    auto files = dir.entryInfoList(QDir::Files);
    for (auto&& file : files) addRom(file);
}
void MainWindow::saveRomList()
{
    QFile file(QFCPP_ROM_LIST_FILE);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        int rows = romsModel.rowCount();
        stream << rows;
        for (int i = 0; i < rows; i++)
            romsModel.item(i, 0)->write(stream);
    }
}
void MainWindow::loadRomList()
{
    qRegisterMetaType<RomInfo>("RomInfo");
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    qRegisterMetaTypeStreamOperators<RomInfo>("RomInfo");
#endif
    QFile file(QFCPP_ROM_LIST_FILE);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        int rows = 0;
        stream >> rows;
        if (stream.atEnd()) return;
        romsModel.setRowCount(rows);
        for (int i = 0; i < rows; i++)
        {
            auto item = new QStandardItem{};
            item->read(stream);
            romsModel.setItem(i, 0, item);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* const e)
{
    if (!gConfig.gui.exitConfirmation || QMessageBox::Yes == QMessageBox::question(this,
        "Close Confirmation",
        "Exit?",
        QMessageBox::Yes | QMessageBox::No))
    {
        gEmulator.stop();
        saveRomList();
        e->accept();
    }
    else e->ignore();
}
void MainWindow::dragEnterEvent(QDragEnterEvent* const e)
{
    if (e->mimeData()->hasUrls()) e->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent* const e)
{
    QFileInfo fileInfo{};
    auto urls = e->mimeData()->urls();
    for (auto&& url : urls)
    {
        fileInfo.setFile(url.toLocalFile());
        if (fileInfo.isDir()) scanRoms(fileInfo.absoluteFilePath());
        else addRom(fileInfo);
    }
    if (urls.size() == 1 && !fileInfo.isDir()) runRom(fileInfo.fileName());
    e->acceptProposedAction();
}

// solts
void MainWindow::on_action_open_triggered()
{
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setNameFilters({ "FC/NES rom files (*.nes)", "Any files (*)" });
    if (fileDialog.exec())
    {
        auto urls = fileDialog.selectedUrls();
        if (urls.size() == 1)
        {
            auto fileInfo = QFileInfo{ urls.first().toLocalFile() };
            addRom(fileInfo);
            runRom(fileInfo.fileName());
        }
        else for (auto&& url : urls) addRom(QFileInfo{ url.toLocalFile() });
    }
}
void MainWindow::on_action_remove_triggered()
{
    if (ui->list_view_roms->selectionModel()->hasSelection()) romsModel.removeRow(ui->list_view_roms->currentIndex().row());
}
void MainWindow::on_action_clear_triggered()
{
    romsModel.clear();
}
void MainWindow::on_action_refresh_triggered()
{
    for (auto&& path : gConfig.gui.romFolders) scanRoms(path);
    romsModel.sort(0);
}
void MainWindow::on_action_play_triggered()
{
    if (ui->list_view_roms->selectionModel()->hasSelection()) runRom(ui->list_view_roms->currentIndex());
}
void MainWindow::on_action_save_state_triggered()
{
    if (!gEmulator.running()) return;
    auto state = gEmulator.getQuickSnapshot();
    gEmulator.pushQuickSave();
    QFileInfo fileInfo{ gConfig.gui.lastPlay.path };
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilters({ "QFCPP state file (*.sav)" });
    fileDialog.setDefaultSuffix("sav");
    fileDialog.selectFile(QStringLiteral("%1_%2.sav").arg(
        fileInfo.completeBaseName(),
        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss")));
    if (fileDialog.exec())
    {
        if (!util::saveState(fileDialog.selectedFiles().first())) gError.send(QFCPP_ERROR_FAILED_TO_SAVE_STATE);
    }
    gEmulator.setQuickSnapshot(state);
}
void MainWindow::on_action_load_state_triggered()
{
    if (!gEmulator.running()) return;
    QFileDialog fileDialog{};
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilters({ "QFCPP state file (*.sav)" });
    if (fileDialog.exec())
    {
        if (util::loadState(fileDialog.selectedFiles().first())) gEmulator.pushQuickLoad();
        else gError.send(QFCPP_ERROR_FAILED_TO_LOAD_STATE);
    }
}
void MainWindow::on_action_settings_triggered()
{
    auto settingDialog = new SettingDialog(this);
    settingDialog->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(settingDialog, &SettingDialog::refresh, this, &MainWindow::on_action_refresh_triggered);
    settingDialog->show();
}
void MainWindow::on_action_about_triggered()
{
    QMessageBox::about(this, "About",
        "QFCPP v" QFCPP_VERSION_STR "\n"
        "FC/NES emulator powered by FCPP and Qt\n\n"
        "Built on " QFCPP_BUILD_DATE " " __TIME__ " with:\n"
        "   FCPP (v" FCPP_VERSION_STR ")\n"
        "   FCPP_IO (v" FCPP_IO_VERSION_STR ")\n"
        "   Qt (v" QT_VERSION_STR ")\n\n"
        "Copyright (c) by TianZerL the FCPP project 2022-\n\n"
        "The emulator is for educational and development purposes only");
}
