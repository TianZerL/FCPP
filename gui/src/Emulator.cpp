#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <utility>

#include "FCPP/Util/Tape.hpp"

#include "Emulator.hpp"

namespace detail
{
    struct Messages
    {
        struct Message
        {
            std::uint16_t msg = 0;
            explicit operator bool() noexcept
            {
                return msg ? msg-- : false;
            }
            void send() noexcept
            {
                msg++;
            }
        } reset{}, save{}, load{};

        void clear() noexcept
        {
            *this = {};
        }
    };

    class TaskManager
    {
    public:
        using Arg = fcpp::io::Controller*;
        using Task = std::function<void(Arg)>;
    public:
        TaskManager() = default;
        ~TaskManager() = default;

        void clear() noexcept;
        void push(Task task);
        void polls(Arg arg) noexcept;
    private:
        std::mutex mutex{};
        std::vector<Task> tasks{};
    };
    inline void detail::TaskManager::clear() noexcept
    {
        tasks.clear();
    }
    inline void TaskManager::push(Task task)
    {
        const std::lock_guard<std::mutex> lock(mutex);
        tasks.emplace_back(std::move(task));
    }
    inline void TaskManager::polls(const Arg arg) noexcept
    {
        const std::lock_guard<std::mutex> lock(mutex);
        for (auto&& task : tasks) task(arg);
        clear();
    }

    class QuickSnapshotSlot
    {
    public:
        QuickSnapshotSlot() = default;
        ~QuickSnapshotSlot() = default;

        void clear() noexcept;
        fcpp::core::Snapshot& get() noexcept;
    private:
        fcpp::core::Snapshot snapshot{};
    };
    inline void QuickSnapshotSlot::clear() noexcept
    {
        snapshot.rewindReader();
        snapshot.rewindWriter();
    }
    inline fcpp::core::Snapshot& QuickSnapshotSlot::get() noexcept
    {
        return snapshot;
    }

    class EmulatorImpl
    {
    public:
        EmulatorImpl() = default;
        ~EmulatorImpl();

        bool run(std::string filePath) noexcept;
        void stop() noexcept;

        bool isStop() const noexcept;
        bool isPause() const noexcept;
        bool isRewind() const noexcept;

        void pushPause(bool v);
        void pushPause();
        void pushRewind();
        void pushMessage(Messages::Message& msg);
    private:
        bool stopFlag = true, pauseFlag = true, rewindFlag = false;
        std::thread thread{};
    public:
        std::uint64_t frameCount = 0;
        Messages messages{};
        QuickSnapshotSlot quickSnapshotSlot{};
        TaskManager taskManager{};
        Emulator::Config config{};
    };
    EmulatorImpl::~EmulatorImpl()
    {
        stop();
    }
    bool EmulatorImpl::run(const std::string filePath) noexcept
    {
        fcpp::core::INES rom{};
        if (!rom.load(filePath.c_str()) || !fcpp::core::Cartridge::support(rom)) return false;
        stop();
        frameCount = 0;
        stopFlag = pauseFlag = rewindFlag = false;
        messages.clear();
        quickSnapshotSlot.clear();
        taskManager.clear();
        thread = std::thread([=](Emulator::Config config, fcpp::core::INES content)
            {
                fcpp::core::FC fc{};
                if (!fc.insertCartridge(std::move(content))) return;

                bool recordFlag = false;
                fcpp::util::Tape<fcpp::core::Snapshot> tape{ config.tapeLength };
                auto controller = fcpp::io::manager::create(config.engineIdx);

                if (!controller) return;
                controller->setTitle(filePath.c_str());
                controller->setScale(config.scale);
                controller->setFPSLimit(config.fpsLimit);
                controller->setSampleRate(config.sampleRate);
                controller->setVolume(config.volume);
                controller->setBorderless(config.fullScreen);
                controller->setVerticalSync(config.vsync);
                controller->setPaletteTable(config.paletteTable);
                controller->setRenderDriver(config.renderDriverIdx);
                controller->setJoypadType(0, config.joypadType[0]);
                controller->setJoypadType(1, config.joypadType[1]);
                controller->setTurboButtonSpeed(0, config.turboButtonSpeed[0]);
                controller->setTurboButtonSpeed(1, config.turboButtonSpeed[1]);
                controller->setJoystickPort(0, config.joystickPort[0]);
                controller->setJoystickPort(1, config.joystickPort[1]);
                controller->bind(0, config.keyboardMap[0]);
                controller->bind(1, config.keyboardMap[1]);
                controller->bind(0, config.joystickMap[0]);
                controller->bind(1, config.joystickMap[1]);
                controller->setJoystickPort(1, config.joystickPort[1]);
                controller->setCloseCallback([&]() {stopFlag = true; });
                controller->setRenderCallback([&]()
                    {
                        if (!(frameCount++ & 0x0f) && !isPause() && !isRewind())
                        {
                            messages.save.send();
                            recordFlag = true;
                        }
                        taskManager.polls(controller.get());
                    });
                controller->setKeyPressCallback([&](const fcpp::io::Keyboard key)
                    {
                        switch (key)
                        {
                        case fcpp::io::Keyboard::Escape:
                            stopFlag = true;
                            break;
                        case fcpp::io::Keyboard::F1:
                            messages.save.send();
                            break;
                        case fcpp::io::Keyboard::F2:
                            messages.load.send();
                            break;
                        case fcpp::io::Keyboard::F3:
                            messages.reset.send();
                            break;
                        case fcpp::io::Keyboard::F4:
                            pushPause();
                            break;
                        case fcpp::io::Keyboard::F5:
                            pushRewind();
                            break;
                        default:
                            break;
                        }
                    });
                if (!controller->create()) return;

                fc.setFrameRate(config.fpsLimit);
                fc.setSpriteLimit(config.spriteLimit);
                fc.connect(0, controller->getInputScanner(0));
                fc.connect(1, controller->getInputScanner(1));
                fc.connect(controller->getFrameBuffer());
                fc.connect(controller->getSampleBuffer());
                fc.powerOn();

                emit gEmulator.started();

                while (!stopFlag)
                {
                    if (messages.reset) fc.reset(), pushPause(false);
                    else if (messages.save) fc.save(recordFlag ? recordFlag = false, tape.next() : quickSnapshotSlot.get());
                    else if (messages.load) fc.load(rewindFlag ? pushPause(true), tape.load() : quickSnapshotSlot.get());
                    else if (pauseFlag) controller->render();
                    else fc.exec();
                }

                fc.save(quickSnapshotSlot.get());
                emit gEmulator.stopped();
            }, config, std::move(rom));
        return true;
    }
    inline void EmulatorImpl::stop() noexcept
    {
        stopFlag = true;
        if (thread.joinable()) thread.join();
    }
    inline bool EmulatorImpl::isStop() const noexcept
    {
        return stopFlag;
    }
    inline bool EmulatorImpl::isPause() const noexcept
    {
        return pauseFlag;
    }
    inline bool EmulatorImpl::isRewind() const noexcept
    {
        return rewindFlag;
    }
    inline void EmulatorImpl::pushPause(const bool v)
    {
        taskManager.push([=](auto) {if (!(pauseFlag = v)) rewindFlag = false; });
    }
    inline void detail::EmulatorImpl::pushPause()
    {
        pushPause(!isPause());
    }
    inline void EmulatorImpl::pushRewind()
    {
        taskManager.push([=](auto)
            {
                rewindFlag = true;
                pauseFlag = false;
                messages.load.send();
            });
    }
    inline void EmulatorImpl::pushMessage(Messages::Message& msg)
    {
        taskManager.push([&](auto) {msg.send(); });
    }
}

struct Emulator::EmulatorData
{
    detail::EmulatorImpl impl{};
};

Emulator& Emulator::instance() noexcept
{
    static Emulator emulator{};
    return emulator;
}

Emulator::Emulator() : dptr(std::make_unique<EmulatorData>()) {}
Emulator::~Emulator() noexcept = default;

Emulator::Config& Emulator::getConfig() noexcept
{
    return dptr->impl.config;
}
std::uint64_t Emulator::getTotalFrame() noexcept
{
    return dptr->impl.frameCount;
}
void Emulator::setQuickSnapshot(const fcpp::core::Snapshot& other) noexcept
{
    dptr->impl.quickSnapshotSlot.get() = other;
}
fcpp::core::Snapshot Emulator::getQuickSnapshot() noexcept
{
    return dptr->impl.quickSnapshotSlot.get();
}

bool Emulator::run(const char* const filePath) noexcept
{
    return dptr->impl.run(filePath);
}
bool Emulator::running() noexcept
{
    return !dptr->impl.isStop();
}

// solts
void Emulator::stop() noexcept
{
    dptr->impl.stop();
}
void Emulator::pause()
{
    dptr->impl.pushPause();
}

void Emulator::pushTask(const std::function<void(fcpp::io::Controller*)>& task)
{
    dptr->impl.taskManager.push(task);
}
void Emulator::pushRewind()
{
    dptr->impl.pushRewind();
}
void Emulator::pushReset()
{
    dptr->impl.pushMessage(dptr->impl.messages.reset);
}
void Emulator::pushQuickSave()
{
    dptr->impl.pushMessage(dptr->impl.messages.save);
}
void Emulator::pushQuickLoad()
{
    dptr->impl.pushMessage(dptr->impl.messages.load);
}
