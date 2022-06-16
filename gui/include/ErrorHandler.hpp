#ifndef FCPP_QFCPP_ERROR_HANDLER_HPP
#define FCPP_QFCPP_ERROR_HANDLER_HPP

#include <QErrorMessage>
#include <QString>

#define QFCPP_ERROR_ROM_NOT_FOUND (ErrorHandler::tr("ROM not found"))
#define QFCPP_ERROR_ROM_NOT_SUPPORTED (ErrorHandler::tr("ROM not supported"))
#define QFCPP_ERROR_FAILED_TO_SAVE_STATE (ErrorHandler::tr("Failed to save state"))
#define QFCPP_ERROR_FAILED_TO_LOAD_STATE (ErrorHandler::tr("Failed to load state"))
#define gError (ErrorHandler::instance())

class ErrorHandler : private QErrorMessage
{
    Q_OBJECT

public:
    static ErrorHandler& instance() noexcept;
public:
    void send(const QString& msg);
private:
    ErrorHandler() noexcept;
    ~ErrorHandler() override = default;
};

#endif
