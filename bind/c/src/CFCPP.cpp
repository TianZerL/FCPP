#include "FCPP/Core.hpp"
#include "FCPP/CFCPP.h"

template<typename Client>
class Delegate
{
public:
    Delegate() = default;
    ~Delegate() = default;

    void operator()(const Client* const client);
public:
    Client client{};
};
template<typename Client>
void Delegate<Client>::operator()(const Client* const client)
{
    this->client = *client;
}

class InputScannerDelegate :
    public Delegate<fcpp_input_scanner>,
    public fcpp::core::InputScanner
{
public:
    InputScannerDelegate() = default;
    ~InputScannerDelegate() override = default;

    std::uint8_t scan() noexcept override;
    fcpp::core::JoypadType getJoypadType() noexcept override;
};
std::uint8_t InputScannerDelegate::scan() noexcept
{
    return client.scan != nullptr ? client.scan(client.data) : 0;
}
fcpp::core::JoypadType InputScannerDelegate::getJoypadType() noexcept
{
    return client.get_joypad_type != nullptr ? 
        static_cast<fcpp::core::JoypadType>(client.get_joypad_type(client.data)) : 
        fcpp::core::JoypadType::Standard;
}

class FrameBufferDelegate :
    public Delegate<fcpp_frame_buffer>,
    public fcpp::core::FrameBuffer
{
public:
    FrameBufferDelegate() = default;
    ~FrameBufferDelegate() override = default;

    void setPixel(int x, int y, std::uint32_t color) noexcept override;
    void completedSignal() noexcept override;
    const std::uint32_t* getPaletteTable() noexcept override;
};
void FrameBufferDelegate::setPixel(const int x, const int y, const std::uint32_t color) noexcept
{
    if(client.set_pixel != nullptr) client.set_pixel(x, y, color, client.data);
}
void FrameBufferDelegate::completedSignal() noexcept
{
    if (client.completed_signal != nullptr) client.completed_signal(client.data);
}
const std::uint32_t* FrameBufferDelegate::getPaletteTable() noexcept
{
    return client.get_palette_table != nullptr ? client.get_palette_table(client.data) : nullptr;
}

class SampleBufferDelegate :
    public Delegate<fcpp_sample_buffer>,
    public fcpp::core::SampleBuffer
{
public:
    SampleBufferDelegate() = default;
    ~SampleBufferDelegate() override = default;

    void sendSample(double sample) noexcept override;
    int getSampleRate() noexcept override;
};
void SampleBufferDelegate::sendSample(const double sample) noexcept
{
    if (client.send_sample != nullptr) client.send_sample(sample, client.data);
}
int SampleBufferDelegate::getSampleRate() noexcept
{
    return client.get_sample_rate != nullptr ? client.get_sample_rate(client.data) : 44100;
}

struct fcpp_fc
{
    fcpp::core::FC self{};
    InputScannerDelegate inputScanner[2]{};
    FrameBufferDelegate frameBuffer{};
    SampleBufferDelegate sampleBuffer{};
};
struct fcpp_ines
{
    fcpp::core::INES self{};
};
struct fcpp_snapshot
{
    fcpp::core::Snapshot self{};
};

fcpp_fc_t fcpp_fc_create(void) CFCPP_NOEXCEPT
{
    auto fc = new fcpp_fc;
    fc->self.connect(0, &fc->inputScanner[0]);
    fc->self.connect(1, &fc->inputScanner[1]);
    fc->self.connect(&fc->frameBuffer);
    fc->self.connect(&fc->sampleBuffer);
    return fc;
}
void fcpp_fc_destroy(const fcpp_fc_t fc) CFCPP_NOEXCEPT
{
    delete fc;
}
int fcpp_fc_insert_cartridge_from_file(const fcpp_fc_t fc, const char* const path) CFCPP_NOEXCEPT
{
    return fc->self.insertCartridge(path);
}
int fcpp_fc_insert_cartridge_from_ines(const fcpp_fc_t fc, const fcpp_ines_t ines) CFCPP_NOEXCEPT
{
    return fc->self.insertCartridge(ines->self);
}
void fcpp_fc_connect_input_scanner(const fcpp_fc_t fc, const int index, const struct fcpp_input_scanner* const input_scanner) CFCPP_NOEXCEPT
{
    fc->inputScanner[index](input_scanner);
}
void fcpp_fc_connect_frame_buffer(const fcpp_fc_t fc, const struct fcpp_frame_buffer* const frame_buffer) CFCPP_NOEXCEPT
{
    fc->frameBuffer(frame_buffer);
}
void fcpp_fc_connect_sample_buffer(const fcpp_fc_t fc, const struct fcpp_sample_buffer* const sample_buffer) CFCPP_NOEXCEPT
{
    fc->sampleBuffer(sample_buffer);
}
void fcpp_fc_set_frame_rate(const fcpp_fc_t fc, const double fps) CFCPP_NOEXCEPT
{
    fc->self.setFrameRate(fps);
}
void fcpp_fc_set_sprite_limit(const fcpp_fc_t fc, const int limit) CFCPP_NOEXCEPT
{
    fc->self.setSpriteLimit(limit);
}
void fcpp_fc_power_on(const fcpp_fc_t fc) CFCPP_NOEXCEPT
{
    fc->self.powerOn();
}
void fcpp_fc_reset(const fcpp_fc_t fc) CFCPP_NOEXCEPT
{
    fc->self.reset();
}
void fcpp_fc_save(const fcpp_fc_t fc, const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    fc->self.save(snapshot->self);
}
void fcpp_fc_load(const fcpp_fc_t fc, const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    fc->self.load(snapshot->self);
}
void fcpp_fc_exec(const fcpp_fc_t fc) CFCPP_NOEXCEPT
{
    fc->self.exec();
}

fcpp_ines_t fcpp_ines_create(void) CFCPP_NOEXCEPT
{
    return new fcpp_ines{};
}
fcpp_ines_t fcpp_ines_create_from(const fcpp_ines_t other) CFCPP_NOEXCEPT
{
    return new fcpp_ines{ other->self };
}
void fcpp_ines_destroy(const fcpp_ines_t ines) CFCPP_NOEXCEPT
{
    delete ines;
}
void fcpp_ines_load_from_file(const fcpp_ines_t ines, const char* const path) CFCPP_NOEXCEPT
{
    ines->self.load(path);
}
void fcpp_ines_load_from_buffer(const fcpp_ines_t ines, const uint8_t* const buffer, const size_t size) CFCPP_NOEXCEPT
{
    ines->self.load(buffer, size);
}
uint8_t fcpp_ines_get_prg_banks(const fcpp_ines_t ines) CFCPP_NOEXCEPT
{
    return ines->self.getPRGBanks();
}
uint8_t fcpp_ines_get_chr_banks(const fcpp_ines_t ines) CFCPP_NOEXCEPT
{
    return ines->self.getCHRBanks();
}
uint8_t fcpp_ines_get_mapper_type(const fcpp_ines_t ines) CFCPP_NOEXCEPT
{
    return ines->self.getMapperType();
}
uint8_t fcpp_ines_read_prg(const fcpp_ines_t ines, const uint32_t addr) CFCPP_NOEXCEPT
{
    return ines->self.readPRG(addr);
}
void fcpp_ines_write_prg(const fcpp_ines_t ines, const uint32_t addr, const uint8_t data) CFCPP_NOEXCEPT
{
    ines->self.writePRG(addr, data);
}
uint8_t fcpp_ines_read_chr(const fcpp_ines_t ines, const uint32_t addr) CFCPP_NOEXCEPT
{
    return ines->self.readCHR(addr);
}
void fcpp_ines_write_chr(const fcpp_ines_t ines, const uint32_t addr, const uint8_t data) CFCPP_NOEXCEPT
{
    ines->self.writeCHR(addr, data);
}

fcpp_snapshot_t fcpp_snapshot_create(void) CFCPP_NOEXCEPT
{
    return new fcpp_snapshot{};
}
fcpp_snapshot_t fcpp_snapshot_create_from(const fcpp_snapshot_t other) CFCPP_NOEXCEPT
{
    return new fcpp_snapshot{ other->self };
}
void fcpp_snapshot_destroy(const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    delete snapshot;
}
size_t fcpp_snapshot_size(const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    return snapshot->self.size();
}
size_t fcpp_snapshot_capacity(const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    return snapshot->self.capacity();
}
uint8_t* fcpp_snapshot_data(const fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT
{
    return snapshot->self.data();
}
