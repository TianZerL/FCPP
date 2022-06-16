#include "ErrorHandler.hpp"

ErrorHandler& ErrorHandler::instance() noexcept
{
    static ErrorHandler errorHandler{};
    return errorHandler;
}

void ErrorHandler::send(const QString& msg)
{
    showMessage(msg);
}

ErrorHandler::ErrorHandler() noexcept : 
    QErrorMessage(nullptr) 
{
    setWindowTitle(tr("Error"));
}
