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

    uint8_t _remoteX = 0;
    uint8_t _remoteY = 0;

    enum class LaserRoundEffectStateMachine : uint8_t
    {
        None,
        RasingXDroppingY,
        RasingXRasingY,
        DroppingXDroppingY,
        DroppingXRasingY,
    };

    LaserRoundEffectStateMachine _roundEffectState = LaserRoundEffectStateMachine::None;
    void randomPosition();
    void roundEffect();
    
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