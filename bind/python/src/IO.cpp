#include <tuple>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include "FCPP/IO.hpp"

namespace py = pybind11;

class PyController : public fcpp::io::Controller
{
public:
    class PyInfo : public fcpp::io::Controller::Info
    {
    public:
        PyInfo() = default;
        ~PyInfo() override = default;
        int getRenderDriverCount() noexcept override;
        const char* getRenderDriverName(int idx) noexcept override;
    };
public:
    PyController() = default;
    ~PyController() override = default;

    bool create() noexcept override;
    fcpp::core::FrameBuffer* getFrameBuffer() noexcept override;
    fcpp::core::InputScanner* getInputScanner(int idx) noexcept override;
    fcpp::core::SampleBuffer* getSampleBuffer() noexcept override;
    void render() noexcept override;
    bool setFullScreen(bool enable) noexcept override;
    bool setBorderless(bool enable) noexcept override;
    bool setVerticalSync(bool enable) noexcept override;
    bool setRenderDriver(int idx) noexcept override;
    bool setJoystickPort(int idx, int port) noexcept override;
    void setScale(float factor) noexcept override;
    void setTitle(const char* text) noexcept override;
    void setFPSLimit(double fps) noexcept override;
    void setVolume(float volume) noexcept override;
    void setSampleRate(int rate) noexcept override;
    void setJoypadType(int idx, fcpp::core::JoypadType type) noexcept override;
    void setPaletteTable(const fcpp::io::PaletteTable& paletteTable) noexcept override;
    void setPaletteTable(fcpp::io::PaletteTable&& paletteTable) noexcept override;
    void setTurboButtonSpeed(int idx, std::uint8_t v) noexcept override;
    void setFrameBufferData(const std::uint8_t* data) noexcept override;
    void getFrameBufferData(std::uint8_t* data) const noexcept override;
    void bind(int idx, int standardButtonIdx, fcpp::io::Keyboard key) noexcept override;
    void bind(int idx, int standardButtonIdx, fcpp::io::Joystick button) noexcept override;
    void setKeyPressCallback(std::function<void(fcpp::io::Keyboard)> callback) noexcept override;
    void setFrameCompleteCallback(std::function<void()> callback) noexcept override;
    void setRenderCallback(std::function<void()> callback) noexcept override;
    void setCloseCallback(std::function<void()> callback) noexcept override;
};
bool PyController::create() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "create",
        create
    );
}
fcpp::core::FrameBuffer* PyController::getFrameBuffer() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        fcpp::core::FrameBuffer*,
        Controller,
        "get_frame_buffer",
        getFrameBuffer
    );
}
fcpp::core::InputScanner* PyController::getInputScanner(const int idx) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        fcpp::core::InputScanner*,
        Controller,
        "get_input_scanner",
        getInputScanner,
        idx
    );
}
fcpp::core::SampleBuffer* PyController::getSampleBuffer() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        fcpp::core::SampleBuffer*,
        Controller,
        "get_sample_buffer",
        getSampleBuffer
    );
}
void PyController::render() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "render",
        render
    );
}
bool PyController::setFullScreen(const bool enable) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "set_full_screen",
        setFullScreen,
        enable
    );
}
bool PyController::setBorderless(const bool enable) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "set_borderless",
        setBorderless,
        enable
    );
}
bool PyController::setVerticalSync(const bool enable) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "set_vertical_sync",
        setVerticalSync,
        enable
    );
}
bool PyController::setRenderDriver(const int idx) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "set_render_driver",
        setRenderDriver,
        idx
    );
}
bool PyController::setJoystickPort(const int idx, const int port) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        bool,
        Controller,
        "set_render_driver",
        setRenderDriver,
        idx, port
    );
}
void PyController::setScale(const float factor) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_scale",
        setScale,
        factor
    );
}
void PyController::setTitle(const char* const text) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_title",
        setTitle,
        text
    );
}
void PyController::setFPSLimit(const double fps) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_fps_limit",
        setFPSLimit,
        fps
    );
}
void PyController::setVolume(const float volume) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_volume",
        setVolume,
        volume
    );
}
void PyController::setSampleRate(const int rate) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_sample_rate",
        setSampleRate,
        rate
    );
}
void PyController::setJoypadType(const int idx, const fcpp::core::JoypadType type) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_joypad_type",
        setJoypadType,
        idx, type
    );
}
void PyController::setPaletteTable(const fcpp::io::PaletteTable& paletteTable) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_palette_table",
        setPaletteTable,
        paletteTable
    );
}
void PyController::setPaletteTable(fcpp::io::PaletteTable&& paletteTable) noexcept
{
    setPaletteTable(paletteTable);
}
void PyController::setTurboButtonSpeed(const int idx, const std::uint8_t v) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_turbo_button_speed",
        setTurboButtonSpeed,
        idx, v
    );
}
void PyController::setFrameBufferData(const std::uint8_t* const data) noexcept {/*Not supported for pyfcpp*/ }
void PyController::getFrameBufferData(std::uint8_t* const data) const noexcept {/*Not supported for pyfcpp*/ }
void PyController::bind(const int idx, const int standardButtonIdx, const fcpp::io::Keyboard key) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "bind",
        bind,
        idx, standardButtonIdx, key
    );
}
void PyController::bind(const int idx, const int standardButtonIdx, const fcpp::io::Joystick button) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "bind",
        bind,
        idx, standardButtonIdx, button
    );
}
void PyController::setKeyPressCallback(const std::function<void(fcpp::io::Keyboard)> callback) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_key_press_callback",
        setKeyPressCallback,
        callback
    );
}
void PyController::setFrameCompleteCallback(const std::function<void()> callback) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_frame_complete_callback",
        setFrameCompleteCallback,
        callback
    );
}
void PyController::setRenderCallback(const std::function<void()> callback) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_render_callback",
        setRenderCallback,
        callback
    );
}
void PyController::setCloseCallback(const std::function<void()> callback) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        void,
        Controller,
        "set_close_callback",
        setCloseCallback,
        callback
    );
}

int PyController::PyInfo::getRenderDriverCount() noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        int,
        Info,
        "get_render_driver_count",
        getRenderDriverCount
    );
}
const char* PyController::PyInfo::getRenderDriverName(const int idx) noexcept
{
    PYBIND11_OVERLOAD_PURE_NAME(
        const char*,
        Info,
        "get_render_driver_name",
        getRenderDriverName,
        idx
    );
}

static void initIOManagerModule(py::module_& m)
{
    m.def("count", &fcpp::io::manager::count);
    m.def("name", &fcpp::io::manager::name);
    m.def("create", &fcpp::io::manager::create);
    m.def("info", &fcpp::io::manager::info);
}

void initIOModule(py::module_& m)
{
    auto manager = m.def_submodule("manager", "fcpp io manager");
    initIOManagerModule(manager);

    py::enum_<fcpp::io::Keyboard>(m, "Keyboard")
        .value("A", fcpp::io::Keyboard::A)
        .value("B", fcpp::io::Keyboard::B)
        .value("C", fcpp::io::Keyboard::C)
        .value("D", fcpp::io::Keyboard::D)
        .value("E", fcpp::io::Keyboard::E)
        .value("F", fcpp::io::Keyboard::F)
        .value("G", fcpp::io::Keyboard::G)
        .value("H", fcpp::io::Keyboard::H)
        .value("I", fcpp::io::Keyboard::I)
        .value("J", fcpp::io::Keyboard::J)
        .value("K", fcpp::io::Keyboard::K)
        .value("L", fcpp::io::Keyboard::L)
        .value("M", fcpp::io::Keyboard::M)
        .value("N", fcpp::io::Keyboard::N)
        .value("O", fcpp::io::Keyboard::O)
        .value("P", fcpp::io::Keyboard::P)
        .value("Q", fcpp::io::Keyboard::Q)
        .value("R", fcpp::io::Keyboard::R)
        .value("S", fcpp::io::Keyboard::S)
        .value("T", fcpp::io::Keyboard::T)
        .value("U", fcpp::io::Keyboard::U)
        .value("V", fcpp::io::Keyboard::V)
        .value("W", fcpp::io::Keyboard::W)
        .value("X", fcpp::io::Keyboard::X)
        .value("Y", fcpp::io::Keyboard::Y)
        .value("Z", fcpp::io::Keyboard::Z)
        .value("Num0", fcpp::io::Keyboard::Num0)
        .value("Num1", fcpp::io::Keyboard::Num1)
        .value("Num2", fcpp::io::Keyboard::Num2)
        .value("Num3", fcpp::io::Keyboard::Num3)
        .value("Num4", fcpp::io::Keyboard::Num4)
        .value("Num5", fcpp::io::Keyboard::Num5)
        .value("Num6", fcpp::io::Keyboard::Num6)
        .value("Num7", fcpp::io::Keyboard::Num7)
        .value("Num8", fcpp::io::Keyboard::Num8)
        .value("Num9", fcpp::io::Keyboard::Num9)
        .value("Escape", fcpp::io::Keyboard::Escape)
        .value("LControl", fcpp::io::Keyboard::LControl)
        .value("LShift", fcpp::io::Keyboard::LShift)
        .value("LAlt", fcpp::io::Keyboard::LAlt)
        .value("System", fcpp::io::Keyboard::System)
        .value("RControl", fcpp::io::Keyboard::RControl)
        .value("RShift", fcpp::io::Keyboard::RShift)
        .value("RAlt", fcpp::io::Keyboard::RAlt)
        .value("Menu", fcpp::io::Keyboard::Menu)
        .value("LBracket", fcpp::io::Keyboard::LBracket)
        .value("RBracket", fcpp::io::Keyboard::RBracket)
        .value("Semicolon", fcpp::io::Keyboard::Semicolon)
        .value("Comma", fcpp::io::Keyboard::Comma)
        .value("Period", fcpp::io::Keyboard::Period)
        .value("Quote", fcpp::io::Keyboard::Quote)
        .value("Slash", fcpp::io::Keyboard::Slash)
        .value("Backslash", fcpp::io::Keyboard::Backslash)
        .value("Tilde", fcpp::io::Keyboard::Tilde)
        .value("Equal", fcpp::io::Keyboard::Equal)
        .value("Hyphen", fcpp::io::Keyboard::Hyphen)
        .value("Space", fcpp::io::Keyboard::Space)
        .value("Enter", fcpp::io::Keyboard::Enter)
        .value("Backspace", fcpp::io::Keyboard::Backspace)
        .value("Tab", fcpp::io::Keyboard::Tab)
        .value("PageUp", fcpp::io::Keyboard::PageUp)
        .value("PageDown", fcpp::io::Keyboard::PageDown)
        .value("End", fcpp::io::Keyboard::End)
        .value("Home", fcpp::io::Keyboard::Home)
        .value("Insert", fcpp::io::Keyboard::Insert)
        .value("Delete", fcpp::io::Keyboard::Delete)
        .value("Add", fcpp::io::Keyboard::Add)
        .value("Subtract", fcpp::io::Keyboard::Subtract)
        .value("Multiply", fcpp::io::Keyboard::Multiply)
        .value("Divide", fcpp::io::Keyboard::Divide)
        .value("Left", fcpp::io::Keyboard::Left)
        .value("Right", fcpp::io::Keyboard::Right)
        .value("Up", fcpp::io::Keyboard::Up)
        .value("Down", fcpp::io::Keyboard::Down)
        .value("Numpad0", fcpp::io::Keyboard::Numpad0)
        .value("Numpad1", fcpp::io::Keyboard::Numpad1)
        .value("Numpad2", fcpp::io::Keyboard::Numpad2)
        .value("Numpad3", fcpp::io::Keyboard::Numpad3)
        .value("Numpad4", fcpp::io::Keyboard::Numpad4)
        .value("Numpad5", fcpp::io::Keyboard::Numpad5)
        .value("Numpad6", fcpp::io::Keyboard::Numpad6)
        .value("Numpad7", fcpp::io::Keyboard::Numpad7)
        .value("Numpad8", fcpp::io::Keyboard::Numpad8)
        .value("Numpad9", fcpp::io::Keyboard::Numpad9)
        .value("F1", fcpp::io::Keyboard::F1)
        .value("F2", fcpp::io::Keyboard::F2)
        .value("F3", fcpp::io::Keyboard::F3)
        .value("F4", fcpp::io::Keyboard::F4)
        .value("F5", fcpp::io::Keyboard::F5)
        .value("F6", fcpp::io::Keyboard::F6)
        .value("F7", fcpp::io::Keyboard::F7)
        .value("F8", fcpp::io::Keyboard::F8)
        .value("F9", fcpp::io::Keyboard::F9)
        .value("F10", fcpp::io::Keyboard::F10)
        .value("F11", fcpp::io::Keyboard::F11)
        .value("F12", fcpp::io::Keyboard::F12)
        .value("Pause", fcpp::io::Keyboard::Pause)
        .value("Unknown", fcpp::io::Keyboard::Unknown);

    py::enum_<fcpp::io::Joystick>(m, "Joystick")
        .value("Button0", fcpp::io::Joystick::Button0)
        .value("Button1", fcpp::io::Joystick::Button1)
        .value("Button2", fcpp::io::Joystick::Button2)
        .value("Button3", fcpp::io::Joystick::Button3)
        .value("Button4", fcpp::io::Joystick::Button4)
        .value("Button5", fcpp::io::Joystick::Button5)
        .value("Button6", fcpp::io::Joystick::Button6)
        .value("Button7", fcpp::io::Joystick::Button7)
        .value("Button8", fcpp::io::Joystick::Button8)
        .value("Button9", fcpp::io::Joystick::Button9)
        .value("Button10", fcpp::io::Joystick::Button10)
        .value("Up", fcpp::io::Joystick::Up)
        .value("Down", fcpp::io::Joystick::Down)
        .value("Left", fcpp::io::Joystick::Left)
        .value("Right", fcpp::io::Joystick::Right)
        .value("Axis0P", fcpp::io::Joystick::Axis0P)
        .value("Axis0N", fcpp::io::Joystick::Axis0N)
        .value("Axis1P", fcpp::io::Joystick::Axis1P)
        .value("Axis1N", fcpp::io::Joystick::Axis1N)
        .value("Axis2P", fcpp::io::Joystick::Axis2P)
        .value("Axis2N", fcpp::io::Joystick::Axis2N)
        .value("Axis3P", fcpp::io::Joystick::Axis3P)
        .value("Axis3N", fcpp::io::Joystick::Axis3N)
        .value("Axis4P", fcpp::io::Joystick::Axis4P)
        .value("Axis5P", fcpp::io::Joystick::Axis5P);

    auto paletteColorGetter = 
        [](fcpp::io::PaletteTable& object, const int idx) -> std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> {
            std::uint8_t r = 0, g = 0, b = 0;
            object.get(idx, r, g, b);
            return std::make_tuple(r, g, b);
        };
    py::class_<fcpp::io::PaletteTable>(m, "PaletteTable")
        .def(py::init())
        .def("set", py::overload_cast<int, std::uint8_t, std::uint8_t, std::uint8_t>(&fcpp::io::PaletteTable::set), py::arg("index"), py::arg("r"), py::arg("g"), py::arg("b"))
        .def("set", py::overload_cast<int, std::uint32_t>(&fcpp::io::PaletteTable::set), py::arg("index"), py::arg("argb"))
        .def("get", paletteColorGetter, py::arg("index"))
        .def("get", py::overload_cast<int>(&fcpp::io::PaletteTable::get), py::arg("index"))
        .def("save", &fcpp::io::PaletteTable::save, py::arg("path"))
        .def("load", &fcpp::io::PaletteTable::load, py::arg("path"))
        .def("__setitem__", [](fcpp::io::PaletteTable& object, const int idx, std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> rgb) {
                std::uint8_t r = 0, g = 0, b = 0;
                std::tie(r, g, b) = rgb;
                object.set(idx, r, g, b);
            })
        .def("__getitem__", paletteColorGetter)
        .def_readonly_static("SIZE", &fcpp::io::PaletteTable::Size);

    auto& controller =
        py::class_<fcpp::io::Controller, PyController>(m, "Controller")
        .def(py::init())
        .def("create", &fcpp::io::Controller::create)
        .def("get_frame_buffer", &fcpp::io::Controller::getFrameBuffer, py::return_value_policy::reference_internal)
        .def("get_input_scanner", &fcpp::io::Controller::getInputScanner, py::return_value_policy::reference_internal)
        .def("get_sample_buffer", &fcpp::io::Controller::getSampleBuffer, py::return_value_policy::reference_internal)
        .def("render", &fcpp::io::Controller::render)
        .def("set_full_screen", &fcpp::io::Controller::setFullScreen)
        .def("set_borderless", &fcpp::io::Controller::setBorderless)
        .def("set_vertical_sync", &fcpp::io::Controller::setVerticalSync)
        .def("set_render_driver", &fcpp::io::Controller::setRenderDriver)
        .def("set_joystick_port", &fcpp::io::Controller::setJoystickPort)
        .def("set_scale", &fcpp::io::Controller::setScale)
        .def("set_title", &fcpp::io::Controller::setTitle)
        .def("set_fps_limit", &fcpp::io::Controller::setFPSLimit)
        .def("set_volume", &fcpp::io::Controller::setVolume)
        .def("set_sample_rate", &fcpp::io::Controller::setSampleRate)
        .def("set_joypad_type", &fcpp::io::Controller::setJoypadType)
        .def("set_palette_table", py::overload_cast<const fcpp::io::PaletteTable&>(&fcpp::io::Controller::setPaletteTable))
        .def("set_turbo_button_speed", &fcpp::io::Controller::setTurboButtonSpeed)
        .def("bind", static_cast<void(fcpp::io::Controller::*)(int, int, fcpp::io::Keyboard)>(&fcpp::io::Controller::bind))
        .def("bind", static_cast<void(fcpp::io::Controller::*)(int, int, fcpp::io::Joystick)>(&fcpp::io::Controller::bind))
        .def("set_key_press_callback", &fcpp::io::Controller::setKeyPressCallback)
        .def("set_frame_complete_callback", &fcpp::io::Controller::setFrameCompleteCallback)
        .def("set_render_callback", &fcpp::io::Controller::setRenderCallback)
        .def("set_close_callback", &fcpp::io::Controller::setCloseCallback);

    py::class_<fcpp::io::Controller::Info, PyController::PyInfo>(controller, "Info")
        .def(py::init())
        .def("get_render_driver_count", &fcpp::io::Controller::Info::getRenderDriverCount)
        .def("get_render_driver_name", &fcpp::io::Controller::Info::getRenderDriverName);
}
