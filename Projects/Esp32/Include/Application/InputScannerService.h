#pragma once

#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "RTOSExtra.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"
#include "BaseConnection.h"
#include "BaseRouteHandler.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "WebSocket.h"

namespace Applications
{

using Hal::TimeLimit;
using Utilities::Logger;
using Hal::Hardware;

class InputScannerService : public cpp_freertos::Thread
{
public:
    InputScannerService();
private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    InputScannerService(const InputScannerService &) = delete;

    /// @brief	Hide Assignment operator.
    InputScannerService &operator=(const InputScannerService &) = delete;

    /// @brief	Hide Move constructor.
    InputScannerService(InputScannerService &&) = delete;

    /// @brief	Hide Move assignment operator.
    InputScannerService &operator=(InputScannerService &&) = delete;
};

} // namespace Applications