#include <iostream>
#include <chrono>

#include "FCPP/Core.hpp"

struct TestIO :
    public fcpp::core::FrameBuffer,
    public fcpp::core::SampleBuffer,
    public fcpp::core::InputScanner
{
    TestIO() = default;
    ~TestIO() override = default;

    void setPixel(int /* x */, int /* y */, std::uint32_t /* color */) noexcept override {}
    void completedSignal() noexcept override
    {
        frames += 1000.0;
        if ((++count & 0xff) == 0)
        {
            std::chrono::duration<double, std::milli> time = std::chrono::steady_clock::now() - frameStartTime;
            std::cerr << frames / time.count() << "fps\r";
            frames = 0.0;
            frameStartTime = std::chrono::steady_clock::now();
        }
    }
    const std::uint32_t* getPaletteTable() noexcept override
    {
        return nullptr;
    }

    void sendSample(double /* sample */) noexcept override {}
    int getSampleRate() noexcept override
    {
        return 44100;
    }

    std::uint8_t scan() noexcept override
    {
        return 0;
    }
    fcpp::core::JoypadType getJoypadType() noexcept override
    {
        return fcpp::core::JoypadType::Standard;
    }

    std::chrono::steady_clock::time_point frameStartTime = std::chrono::steady_clock::now();
    unsigned int count = 0;
    double frames = 0;
};

int main(int argc, char* argv[])
{
    TestIO io{};
    fcpp::core::FC fc{};

    auto path = argc > 1 ? argv[argc - 1] : TEST_ROM_LOAD_PATH;
    std::clog << "Load: " << path << std::endl;
    if (!fc.insertCartridge(path))
    {
        std::cerr << "Failed to load rom" << std::endl;
        return 0;
    }

    fc.connect(0, &io);
    fc.connect(dynamic_cast<fcpp::core::FrameBuffer*>(&io));
    fc.connect(dynamic_cast<fcpp::core::SampleBuffer*>(&io));

    fc.powerOn();

    for (;;) fc.exec();
}
