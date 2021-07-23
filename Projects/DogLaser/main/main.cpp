
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
#include "Rng.h"
#include "CircularBuffer.h"
#include "DebugAssert.h"
#include "ApplicationAgent.h"
#include "ConfigurationAgent.h"

using Applications::ApplicationAgent;
using Configuration::ConfigurationAgent;
using Hal::Hardware;
using Utilities::Logger;

extern "C" void app_main(void)
{
	Hardware * hardware = Hardware::Instance();

	ApplicationAgent::Instance();
	ApplicationAgent::Instance()->Initialize();
	ApplicationAgent::Instance()->GetLaserControlService().Start();

	for (;;)
	{
		vTaskDelay(1000);
	}
}
