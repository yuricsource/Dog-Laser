#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "HttpServer.h"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"
#include "BaseConnection.h"
#include "BaseRouteHandler.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "WebSocket.h"
#include "CircularBuffer.h"

namespace Applications
{

using Hal::Timer;
using Hal::Adc;
using Hal::Hardware;
using Utilities::Logger;
using Utilities::CircularBuffer;

class RemoteReceiverService : public cpp_freertos::Thread
{
public:
    RemoteReceiverService();
    ~RemoteReceiverService();
protected:
    void Run() override;

private:
    Hardware *_hardware;
private:
    /// @brief	Hide Copy constructor.
    RemoteReceiverService(const RemoteReceiverService &) = delete;

    /// @brief	Hide Assignment operator.
    RemoteReceiverService &operator=(const RemoteReceiverService &) = delete;

    /// @brief	Hide Move constructor.
    RemoteReceiverService(RemoteReceiverService &&) = delete;

    /// @brief	Hide Move assignment operator.
    RemoteReceiverService &operator=(RemoteReceiverService &&) = delete;
};

} // namespace Applications