#pragma once

#include "Hardware.h"
#include "WifiService.h"
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

class LaserControlService : public cpp_freertos::Thread
{
public:
    LaserControlService();

private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    LaserControlService(const LaserControlService &) = delete;

    /// @brief	Hide Assignment operator.
    LaserControlService &operator=(const LaserControlService &) = delete;

    /// @brief	Hide Move constructor.
    LaserControlService(LaserControlService &&) = delete;

    /// @brief	Hide Move assignment operator.
    LaserControlService &operator=(LaserControlService &&) = delete;
};

} // namespace Applications