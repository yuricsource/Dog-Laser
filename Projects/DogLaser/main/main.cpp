#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <cstring>
#include <esp_attr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "Hardware.h"
#include "Logger.h"
#include "CircularBuffer.h"
#include "DebugAssert.h"
#include "ApplicationAgent.h"
#include "ConfigurationAgent.h"
#include "StatusAgent.h"
#include "InputStatus.h"

using Applications::ApplicationAgent;
using Status::StatusAgent;
using Configuration::ConfigurationAgent;
using Hal::Hardware;
using Utilities::Logger;

extern "C" void app_main(void)
{
	Hardware * hardware = Hardware::Instance();

	// Initialize Status Agent
	StatusAgent::Instance()->Initialize();

	// Start Applications
	ApplicationAgent::Instance()->Initialize();
	ApplicationAgent::Instance()->GetLaserControlService().Start();
	ApplicationAgent::Instance()->GetMenuService()->Start();

	Status::InputStatus* input = new Status::InputStatus("Button 1", Hal::InputType::Analog);

	printf("%s: %d\n\n",input->GetName(), input->GetAnalogLevel());

	for (;;)
	{
		vTaskDelay(1000);
	}
}
