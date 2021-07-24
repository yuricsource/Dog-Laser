#include "LaserControlService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"
#include "ApplicationAgent.h"

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
    Rng& rng = Hardware::Instance()->GetRng();
	ApplicationAgent* appAgent = ApplicationAgent::Instance();
	DeviceInput& deviceInput = Hardware::Instance()->GetDeviceInput();
	uint16_t laserDelay = 0;
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
			laserDelay = deviceInput.GetAnalogInput(DeviceInput::AnalogInputIndex::LaserDelay);
			motorX.SetPositon(rng.GetNumber()%100);
			motorY.SetPositon(rng.GetNumber()%100);

			if (laserDelay == 0)
				laserDelay = 300;

			vTaskDelay(laserDelay + rng.GetNumber()%(laserDelay / 2));
		}
		laserPower = deviceInput.GetAnalogInput(DeviceInput::AnalogInputIndex::LaserPower);
		if (laserPower > 0)
			laserPower = (laserPower * 100) / 4096;
		laser.SetPower(laserPower);
	}
}

} // namespace Applications
