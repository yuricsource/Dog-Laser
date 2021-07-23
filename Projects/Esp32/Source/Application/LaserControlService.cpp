#include "LaserControlService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"

namespace Applications
{
using Hal::Hardware;
using Hal::WiiNunchuk;
using Hal::ServoMotor;
using Hal::Rng;

LaserControlService::LaserControlService() : cpp_freertos::Thread("GWSVC", configGATEWAYSVC_STACK_DEPTH, 3)
{

}

void LaserControlService::Run()
{
    Logger::LogInfo(Logger::LogSource::LaserController, "Laser Controller Service Started");
       
	bool runningWithController = false;
    WiiNunchuk& wiiNunchuk = Hardware::Instance()->GetController();
    ServoMotor& motorY = Hardware::Instance()->GetMotorY();
    ServoMotor& motorX = Hardware::Instance()->GetMotorX();
    Rng& rng = Hardware::Instance()->GetRng();

	for(;;)
	{
		if (wiiNunchuk.IsPresent())
		{
			if (!runningWithController)
			{
				wiiNunchuk.Init();
				runningWithController = true;
			}
		}
		else
			runningWithController = false;

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
			motorX.SetPositon(rng.GetNumber()%100);
			motorY.SetPositon(rng.GetNumber()%100);
			vTaskDelay(200 + rng.GetNumber()%600);
		}
	}
}

} // namespace Applications
