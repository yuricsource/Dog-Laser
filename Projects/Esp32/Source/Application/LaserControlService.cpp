#include "LaserControlService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"
#include "ApplicationAgent.h"
#include "StatusAgent.h"

namespace Applications
{
using Hal::Hardware;
using Hal::WiiNunchuk;
using Hal::ServoMotor;
using Hal::DeviceInput;
using Hal::Laser;
using Hal::Rng;

LaserControlService::LaserControlService() : cpp_freertos::Thread("LSRVC", configLASERSVC_STACK_DEPTH, 3)
{

}

void LaserControlService::Run()
{
    Logger::LogInfo(Logger::LogSource::LaserController, "Laser Controller Service Started");
       
	bool runningWithController = true;
    WiiNunchuk& wiiNunchuk = Hardware::Instance()->GetController();
    ServoMotor& motorY = Hardware::Instance()->GetMotorY();
    ServoMotor& motorX = Hardware::Instance()->GetMotorX();
    Laser& laser = Hardware::Instance()->GetLaser();
   	ApplicationAgent* appAgent = ApplicationAgent::Instance();
	DeviceInput& deviceInput = Hardware::Instance()->GetDeviceInput();
	uint16_t laserPower = 0;
	for(;;)
	{
		if (wiiNunchuk.IsPresent())
		{
			if (!runningWithController)
			{
				wiiNunchuk.Init();
				runningWithController = true;
				appAgent->GetMenuService()->UpdateDisplay("Controller\nConnected");
				Logger::LogInfo(Logger::LogSource::LaserController, "Controller Connected.");
			}
		}
		else
        {
            if (runningWithController)
            {
				appAgent->GetMenuService()->UpdateDisplay("Controller\nDisconnected");
				Logger::LogInfo(Logger::LogSource::LaserController, "Controller Disconnected.");
            }
            runningWithController = false;
        }

		if (runningWithController)
		{
			vTaskDelay(5);
			wiiNunchuk.Update();
			motorY.SetPositon(100 - wiiNunchuk.GetJoystickX());
			motorX.SetPositon(wiiNunchuk.GetJoystickY());
			vTaskDelay(5);
		}
		else
		{
			roundEffect();
		}
		laserPower = Status::StatusAgent::Instance()->GetInputStatusList().
						GetInput(Configuration::InputIndex::PotLaserPower).GetAnalogLevel();
		if (laserPower > 0)
			laserPower = (laserPower * 100) / 4096;
		laser.SetPower(laserPower);
	}
}

void LaserControlService::randomPosition()
{
	ServoMotor& motorY = Hardware::Instance()->GetMotorY();
    ServoMotor& motorX = Hardware::Instance()->GetMotorX();
	uint16_t laserDelay = Status::StatusAgent::Instance()->GetInputStatusList().
					GetInput(Configuration::InputIndex::PotLaserDelay).GetAnalogLevel();
    Rng& rng = Hardware::Instance()->GetRng();
	motorX.SetPositon(rng.GetNumber()%100);
	motorY.SetPositon(rng.GetNumber()%100);

	if (laserDelay < 300)
		laserDelay = 300;

	vTaskDelay(laserDelay + rng.GetNumber()%(laserDelay / 2));
}

void LaserControlService::roundEffect()
{
ServoMotor& motorY = Hardware::Instance()->GetMotorY();
    ServoMotor& motorX = Hardware::Instance()->GetMotorX();
	switch(_roundEffectState)
	{
		case LaserRoundEffectStateMachine::RasingXDroppingY:
			_remoteX++;
			_remoteY--;
			if (_remoteX == 100)
				_roundEffectState = LaserRoundEffectStateMachine::DroppingXDroppingY;
			break;
		case LaserRoundEffectStateMachine::DroppingXDroppingY:
			_remoteX--;
			_remoteY--;
			if (_remoteX == 50)
				_roundEffectState = LaserRoundEffectStateMachine::DroppingXRasingY;
			break;
		case LaserRoundEffectStateMachine::DroppingXRasingY:
			_remoteX--;
			_remoteY++;
			if (_remoteX == 0)
				_roundEffectState = LaserRoundEffectStateMachine::RasingXRasingY;
			break;
		case LaserRoundEffectStateMachine::RasingXRasingY:
			_remoteX++;
			_remoteY++;
			if (_remoteX == 50)
				_roundEffectState = LaserRoundEffectStateMachine::None;
			break;
		
		default:
			_roundEffectState = LaserRoundEffectStateMachine::RasingXDroppingY;
			_remoteX = 50;
			_remoteY = 100;
	}
	motorX.SetPositon(_remoteX);
	motorY.SetPositon(_remoteY);
	uint16_t laserDelay = Status::StatusAgent::Instance()->GetInputStatusList().
				GetInput(Configuration::InputIndex::PotLaserDelay).GetAnalogLevel();

	vTaskDelay(10 + (laserDelay / 100));
}

} // namespace Applications
