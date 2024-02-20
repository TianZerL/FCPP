#include <algorithm>
#include <string>
#include <iostream>

#include "FCPP/Core.hpp"
#include "FCPP/IO.hpp"
#include "FCPP/Util/Archive.hpp"

#include "Options.hpp"

static void showVersion()
{
    std::cout
        << "TFCPP v" TFCPP_VERSION_STR "\n"
        << "A Simple FC/NES emulator powered by FCPP\n\n"
        << "Built on " TFCPP_BUILD_DATE " " __TIME__ " with:\n"
        << "   FCPP (v" FCPP_VERSION_STR ")\n"
        << "   FCPP_IO (v" FCPP_IO_VERSION_STR ")\n";
}

static void listEngine()
{
    for (int i = 0; i < fcpp::io::manager::count(); i++)
    {
        std::cout << fcpp::io::manager::name(i) << " (" << i << ")\n";
        auto info = fcpp::io::manager::info(i);
        for (int j = 0; j < info->getRenderDriverCount(); j++)
            std::cout << "  " << info->getRenderDriverName(j) << " (" << j << ")\n";
    }
}

int main(int argc, char* argv[])
{
    Options options{};
    options.parse(argc, argv);

    if (options.showUsage)
    {
        std::cout << options.usage() << std::endl;
        return 0;
    }
    if (options.showVersion)
    {
        showVersion();
        return 0;
    }
    if (options.listEngine)
    {
        listEngine();
        return 0;
    }

    fcpp::core::FC fc{};
    if (!fc.insertCartridge(options.romPath.c_str()))
    {
        std::cerr << "Failed to load ROM file: " << options.romPath << std::endl;
        return 0;
    }

    std::string fileName{};
    if (auto pos = options.romPath.find_last_of("/\\"); pos == std::string::npos) fileName = options.romPath;
    else fileName = options.romPath.substr(pos + 1);
    std::string saveName = fileName + ".sav";

    options.engineIndex = std::clamp(options.engineIndex, 0, fcpp::io::manager::count() - 1);
    auto controller = fcpp::io::manager::create(options.engineIndex);

    if (!controller)
    {
        std::cerr << "Failed to create IO Controller" << std::endl;
        return 0;
    }

    controller->setTitle(fileName.c_str());
    controller->setScale(2.0f);
    controller->setVerticalSync(true);

    auto info = fcpp::io::manager::info(options.engineIndex);
    options.rendererIndex = std::clamp(options.rendererIndex, 0, info->getRenderDriverCount() - 1);
    controller->setRenderDriver(options.rendererIndex);

    bool stopFlag = false, pauseFlag = false, resetFlag = false, saveFlag = false, loadFlag = false;
    controller->setCloseCallback([&]() {stopFlag = true; });
    controller->setKeyPressCallback([&](const fcpp::io::Keyboard key)
        {
            switch (key)
            {
            case fcpp::io::Keyboard::Escape:
                stopFlag = true;
                break;
            case fcpp::io::Keyboard::F1:
                saveFlag = true;
                break;
            case fcpp::io::Keyboard::F2:
                loadFlag = true;
                break;
            case fcpp::io::Keyboard::F3:
                resetFlag = true;
                break;
            case fcpp::io::Keyboard::F4:
                pauseFlag = !pauseFlag;
                break;
            default:
                break;
            }
        });
   
    if (!controller->create())
    {
        std::cerr << "Failed to create IO Controller" << std::endl;
        return 0;
    }

    fcpp::core::Snapshot snapshot{};

    fc.connect(0, controller->getInputScanner(0));
    fc.connect(1, controller->getInputScanner(1));
    fc.connect(controller->getFrameBuffer());
    fc.connect(controller->getSampleBuffer());
    fc.powerOn();

    if (std::uint64_t size = 0; fcpp::util::archive::load(saveName, fileName, reinterpret_cast<char*>(snapshot.data()), size))
    {
        snapshot.setSize(static_cast<std::size_t>(size));
        fc.load(snapshot);
    }

    while (!stopFlag)
    {
        if (resetFlag)
        {
            pauseFlag = resetFlag = false;
            fc.reset();
        }
        else if (saveFlag)
        {
            saveFlag = false;
            fc.save(snapshot);
        }
        else if (loadFlag)
        {
            loadFlag = false;
            fc.load(snapshot);
        }
        else if (pauseFlag) controller->render();
        else fc.exec();
    }

    fc.save(snapshot);
    fcpp::util::archive::save(saveName, fileName, reinterpret_cast<char*>(snapshot.data()), snapshot.size());

    return 0;
}
