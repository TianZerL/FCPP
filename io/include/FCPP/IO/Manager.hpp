#ifndef FCPP_IO_MANAGER_HPP
#define FCPP_IO_MANAGER_HPP

#include <memory>

#include "FCPP/IO/SDL2/SDL2Controller.hpp"
#include "FCPP/IO/SFML2/SFML2Controller.hpp"
#include "FCPP/IO/RayLib/RayLibController.hpp"

namespace fcpp::io::manager
{
    constexpr int count() noexcept;
    constexpr const char* name(int idx) noexcept;
    std::unique_ptr<Controller> create(int idx);
    std::unique_ptr<Controller::Info> info(int idx);
    std::unique_ptr<Controller::JoystickHelper> joystickHelper(int idx);
}

inline constexpr int fcpp::io::manager::count() noexcept
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
inline constexpr const char* fcpp::io::manager::name(const int idx) noexcept
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
inline std::unique_ptr<fcpp::io::Controller> fcpp::io::manager::create(const int idx)
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
inline std::unique_ptr<fcpp::io::Controller::Info> fcpp::io::manager::info(const int idx)
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
inline std::unique_ptr<fcpp::io::Controller::JoystickHelper> fcpp::io::manager::joystickHelper(const int idx)
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

#endif
