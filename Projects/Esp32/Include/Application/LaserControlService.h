#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"

namespace Applications
{

using Hal::TimeLimit;
using Hal::Hardware;
using Hal::WiiNunchuk;
using Hal::ServoMotor;
using Hal::DeviceInput;
using Hal::Laser;
using Hal::Rng;

class LaserControlService : public cpp_freertos::Thread
{
public:
    LaserControlService();
    ~LaserControlService();

    const bool IsControllerConnected() const
    {
        return _runningWithController;
    }

private:

    uint8_t _remoteX = 0;
    uint8_t _remoteY = 0;       
	bool _runningWithController = true;
    WiiNunchuk* _wiiNunchuk;
    ServoMotor* _motorY;
    ServoMotor* _motorX;
    Rng* _rng;

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