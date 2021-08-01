#include "MenuService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"
#include "ApplicationAgent.h"

namespace Applications
{

MenuService::MenuService() : cpp_freertos::Thread("MENUSVC", configMENUSVC_STACK_DEPTH, 3)
{
    _display = &Hardware::Instance()->GetDisplay();
	_stateMachine = MenuStateMachine::Idle;
}

bool MenuService::updateDisplay(const char * message) const
{
	if (_stateMachine == MenuStateMachine::Idle)
	{
		writeDisplay(message);
		return true;
	}
	return false;
}

void MenuService::writeDisplay(const char * message) const
{
	_display->clearDisplay();
	_display->setTextSize(2);
	_display->setCursor(0,0);
	_display->print("  Dog WII\n");
	_display->setTextSize(1);
	_display->print("\n\n");
	_display->print(message);
	_display->display();
}

void MenuService::Run()
{
    Logger::LogInfo(Logger::LogSource::Menu, "Menu Service Started");
	LaserControlService* laserConterol = ApplicationAgent::Instance()->GetLaserControlService();
	bool controllerState = true;
	for(;;)
	{
		if (laserConterol->IsControllerConnected())
		{
			if (!controllerState)
			{
				updateDisplay("Controller\nConnected");
				controllerState = true;
			}
		}
		else
        {
            if (controllerState)
            	updateDisplay("Controller\nDisconnected");
            controllerState = false;
        }
		Delay(100);
	}
}

} // namespace Applications
