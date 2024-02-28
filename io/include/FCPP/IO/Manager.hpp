#ifndef FCPP_IO_MANAGER_HPP
#define FCPP_IO_MANAGER_HPP

#include <memory>

#include <FCPPIOExport.hpp>

#include "FCPP/IO/Controller.hpp"

namespace fcpp::io::manager
{
    FCPP_IO_EXPORT int count() noexcept;
    FCPP_IO_EXPORT const char* name(int idx) noexcept;
    FCPP_IO_EXPORT std::unique_ptr<Controller> create(int idx);
    FCPP_IO_EXPORT std::unique_ptr<Controller::Info> info(int idx);
    FCPP_IO_EXPORT std::unique_ptr<Controller::JoystickHelper> joystickHelper(int idx);
}

#endif
