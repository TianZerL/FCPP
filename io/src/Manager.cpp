#include "FCPP/IO/Manager.hpp"

int fcpp::io::manager::count() noexcept
{
    int counter = 0;
#ifdef FCPP_IO_SDL2
    counter++;
#endif
#ifdef FCPP_IO_SFML2
    counter++;
#endif
#ifdef FCPP_IO_RAYLIB
    counter++;
#endif
    return counter;
}
const char* fcpp::io::manager::name(const int idx) noexcept
{
    int counter = 0;
#ifdef FCPP_IO_SDL2
    if (idx == counter++) return "SDL2";
#endif
#ifdef FCPP_IO_SFML2
    if (idx == counter++) return "SFML2";
#endif
#ifdef FCPP_IO_RAYLIB
    if (idx == counter++) return "raylib";
#endif
    return nullptr;
}
std::unique_ptr<fcpp::io::Controller> fcpp::io::manager::create(const int idx)
{
    int counter = 0;
#ifdef FCPP_IO_SDL2
    if (idx == counter++) return std::make_unique<fcpp::io::SDL2Controller>();
#endif
#ifdef FCPP_IO_SFML2
    if (idx == counter++) return std::make_unique<fcpp::io::SFML2Controller>();
#endif
#ifdef FCPP_IO_RAYLIB
    if (idx == counter++) return std::make_unique<fcpp::io::RayLibController>();
#endif
    return nullptr;
}
std::unique_ptr<fcpp::io::Controller::Info> fcpp::io::manager::info(const int idx)
{
    int counter = 0;
#ifdef FCPP_IO_SDL2
    if (idx == counter++) return std::make_unique<fcpp::io::SDL2Controller::Info>();
#endif
#ifdef FCPP_IO_SFML2
    if (idx == counter++) return std::make_unique<fcpp::io::SFML2Controller::Info>();
#endif
#ifdef FCPP_IO_RAYLIB
    if (idx == counter++) return std::make_unique<fcpp::io::RayLibController::Info>();
#endif
    return nullptr;
}
std::unique_ptr<fcpp::io::Controller::JoystickHelper> fcpp::io::manager::joystickHelper(const int idx)
{
    int counter = 0;
#ifdef FCPP_IO_SDL2
    if (idx == counter++) return std::make_unique<fcpp::io::SDL2Controller::JoystickHelper>();
#endif
#ifdef FCPP_IO_SFML2
    if (idx == counter++) return std::make_unique<fcpp::io::SFML2Controller::JoystickHelper>();
#endif
#ifdef FCPP_IO_RAYLIB
    if (idx == counter++) return std::make_unique<fcpp::io::RayLibController::JoystickHelper>();
#endif
    return nullptr;
}
