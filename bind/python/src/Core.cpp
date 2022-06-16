#include <cstddef>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "FCPP/Core.hpp"

namespace py = pybind11;

class PyInputScanner : public fcpp::core::InputScanner
{
public:
    PyInputScanner() = default;
    ~PyInputScanner() override = default;

    std::uint8_t scan() noexcept override;
    fcpp::core::JoypadType getJoypadType() noexcept override;
};
std::uint8_t PyInputScanner::scan() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        std::uint8_t,
        InputScanner,
        "scan",
        scan
    );
}
fcpp::core::JoypadType PyInputScanner::getJoypadType() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        fcpp::core::JoypadType,
        InputScanner,
        "get_joypad_type",
        getJoypadType
    );
}

class PyFrameBuffer : public fcpp::core::FrameBuffer
{
public:
    PyFrameBuffer() = default;
    ~PyFrameBuffer() override = default;

    void setPixel(int x, int y, std::uint32_t color) noexcept override;
    void completedSignal() noexcept override;
    const std::uint32_t* getPaletteTable() noexcept override;
};
void PyFrameBuffer::setPixel(const int x, const int y, const std::uint32_t color) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        FrameBuffer,
        "set_pixel",
        setPixel,
        x, y, color
    );
}
void PyFrameBuffer::completedSignal() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        FrameBuffer,
        "completed_signal",
        completedSignal,
    );
}
const std::uint32_t* PyFrameBuffer::getPaletteTable() noexcept
{
    return nullptr;
}

class RenderFrameBuffer : public PyFrameBuffer
{
public:
    RenderFrameBuffer() = default;
    ~RenderFrameBuffer() override = default;

    void setPixel(int x, int y, std::uint32_t color) noexcept override;
    void completedSignal() noexcept override;

    virtual void render(py::array_t<std::uint8_t> frame) = 0;
private:
    std::uint8_t buffer[240 * 256 * 3]{};
};
void RenderFrameBuffer::setPixel(const int x, const int y, const std::uint32_t color) noexcept
{
    constexpr auto step = static_cast<std::ptrdiff_t>(3);
    constexpr auto pitch = static_cast<std::ptrdiff_t>(256 * 3);
    auto data = buffer + y * pitch + x * step;
    data[0] = color & 0xff; // B
    data[1] = (color >> 8) & 0xff; // G
    data[2] = (color >> 16) & 0xff; // R
}
void RenderFrameBuffer::completedSignal() noexcept
{
    render(py::array_t<std::uint8_t>{ {240, 256, 3}, buffer});
}

class PyRenderFrameBuffer : public RenderFrameBuffer
{
public:
    PyRenderFrameBuffer() = default;
    ~PyRenderFrameBuffer() override = default;

    void render(py::array_t<std::uint8_t> frame) override;
};
void PyRenderFrameBuffer::render(py::array_t<std::uint8_t> frame)
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        RenderFrameBuffer,
        "render",
        render,
        frame
    );
}

class PySampleBuffer : public fcpp::core::SampleBuffer
{
public:
    PySampleBuffer() = default;
    ~PySampleBuffer() override = default;

    void sendSample(double sample) noexcept override;
    int getSampleRate() noexcept override;
};
void PySampleBuffer::sendSample(const double sample) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        SampleBuffer,
        "send_sample",
        sendSample,
        sample
    );
}
int PySampleBuffer::getSampleRate() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        int,
        SampleBuffer,
        "get_sample_rate",
        getSampleRate
    );
}

void initCoreModule(const py::module_& m)
{
    m.doc() = "fcpp core";

    py::enum_<fcpp::core::JoypadType>(m, "JoypadType")
        .value("Standard", fcpp::core::JoypadType::Standard);

    py::class_<fcpp::core::StandardJoypad::Buttons>(m, "StandardJoypadButtons")
        .def_readonly_static("A", &fcpp::core::StandardJoypad::Buttons::A)
        .def_readonly_static("B", &fcpp::core::StandardJoypad::Buttons::B)
        .def_readonly_static("SELECT", &fcpp::core::StandardJoypad::Buttons::SELECT)
        .def_readonly_static("START", &fcpp::core::StandardJoypad::Buttons::START)
        .def_readonly_static("UP", &fcpp::core::StandardJoypad::Buttons::UP)
        .def_readonly_static("DOWN", &fcpp::core::StandardJoypad::Buttons::DOWN)
        .def_readonly_static("LEFT", &fcpp::core::StandardJoypad::Buttons::LEFT)
        .def_readonly_static("RIGHT", &fcpp::core::StandardJoypad::Buttons::RIGHT);

    py::class_<fcpp::core::InputScanner, PyInputScanner>(m, "InputScanner")
        .def(py::init())
        .def("scan", &fcpp::core::InputScanner::scan)
        .def("get_joypad_type", &fcpp::core::InputScanner::getJoypadType);

    py::class_<fcpp::core::FrameBuffer, PyFrameBuffer>(m, "FrameBuffer")
        .def(py::init())
        .def("set_pixel", &fcpp::core::FrameBuffer::setPixel)
        .def("completed_signal", &fcpp::core::FrameBuffer::completedSignal);

    py::class_<RenderFrameBuffer, fcpp::core::FrameBuffer, PyRenderFrameBuffer>(m, "RenderFrameBuffer")
        .def(py::init())
        .def("render", &RenderFrameBuffer::render);

    py::class_<fcpp::core::SampleBuffer, PySampleBuffer>(m, "SampleBuffer")
        .def(py::init())
        .def("send_sample", &fcpp::core::SampleBuffer::sendSample)
        .def("get_sample_rate", &fcpp::core::SampleBuffer::getSampleRate);

    py::class_<fcpp::core::FC>(m, "FC")
        .def(py::init())
        .def("insert_cartridge", py::overload_cast<const char*>(&fcpp::core::FC::insertCartridge), py::arg("path"))
        .def("insert_cartridge", py::overload_cast<const fcpp::core::INES&>(&fcpp::core::FC::insertCartridge), py::arg("iNES"))
        .def("connect", py::overload_cast<int, fcpp::core::InputScanner*>(&fcpp::core::FC::connect), py::arg("index"), py::arg("input_scanner"))
        .def("connect", py::overload_cast<fcpp::core::FrameBuffer*>(&fcpp::core::FC::connect), py::arg("frame_buffer"))
        .def("connect", py::overload_cast<fcpp::core::SampleBuffer*>(&fcpp::core::FC::connect), py::arg("sample_buffer"))
        .def("set_frame_rate", &fcpp::core::FC::setFrameRate, py::arg("fps"))
        .def("set_sprite_limit", &fcpp::core::FC::setSpriteLimit, py::arg("limit"))
        .def("power_on", &fcpp::core::FC::powerOn)
        .def("reset", &fcpp::core::FC::reset)
        .def("save", &fcpp::core::FC::save, py::arg("snapshot"))
        .def("load", &fcpp::core::FC::load, py::arg("snapshot"))
        .def("exec", &fcpp::core::FC::exec);

    py::class_<fcpp::core::INES>(m, "INES")
        .def(py::init())
        .def("load", [](fcpp::core::INES& object, const py::buffer& buffer) -> bool
            {
                py::buffer_info info = buffer.request();
                if (info.format != py::format_descriptor<std::uint8_t>::format() || info.ndim != 1) return false;
                return object.load(static_cast<std::uint8_t*>(info.ptr), info.size);
            }, py::arg("buffer"))
        .def("load", py::overload_cast<const char*>(&fcpp::core::INES::load), py::arg("path"))
        .def("get_prg_banks", &fcpp::core::INES::getPRGBanks)
        .def("get_chr_banks", &fcpp::core::INES::getCHRBanks)
        .def("get_mapper_type", &fcpp::core::INES::getMapperType);

    py::class_<fcpp::core::Snapshot>(m, "Snapshot")
        .def(py::init())
        .def("size", &fcpp::core::Snapshot::size)
        .def("capacity", &fcpp::core::Snapshot::capacity);
}
