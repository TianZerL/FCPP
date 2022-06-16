#ifndef FCPP_QFCPP_CONFIG_HPP
#define FCPP_QFCPP_CONFIG_HPP

#include <QString>
#include <QStringList>

#include "Emulator.hpp"
#include "RomInfo.hpp"

#define QFCPP_DATA_PATH "data"
#define QFCPP_AUTO_SAVE_PATH "save"
#define QFCPP_ROM_LIST_FILE QFCPP_DATA_PATH"/rom_list.dat"
#define QFCPP_SETTINGS_FILE QFCPP_DATA_PATH"/qfcpp_settings.ini"
#define QFCPP_AUTO_SAVE_FILE (QFCPP_AUTO_SAVE_PATH"/" + gConfig.gui.lastPlay.name + ".sav")
#define gConfig (Config::instance())

class Config
{
public:
	struct GUI
	{
		bool autoResume = false;
		bool exitConfirmation = false;
		QString styleName{ "Fusion" };
		QString palettePath{};
		QStringList romFolders{};
		RomInfo lastPlay{};
	} gui{};

	Emulator::Config& emu;
public:
	static Config& instance() noexcept;
private:
	explicit Config(Emulator::Config& config) noexcept;
	~Config() noexcept;

	void save() noexcept;
	void load() noexcept;
};

#endif
