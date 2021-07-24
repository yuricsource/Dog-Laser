#include "MenuService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"

namespace Applications
{

MenuService::MenuService() : cpp_freertos::Thread("MENUVC", configMENUSVC_STACK_DEPTH, 3)
{
    _display = &Hardware::Instance()->GetDisplay();
	_stateMachine = MenuStateMachine::Idle;
}

bool MenuService::UpdateDisplay(const char * message) const
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

	for(;;)
	{
		vTaskDelay(1000);
	}
}

} // namespace Applications
