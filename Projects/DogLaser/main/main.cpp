
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
	//ConfigurationAgent::Instance();
	ApplicationAgent::Instance();
	ApplicationAgent::Instance()->Initialize();
	ApplicationAgent::Instance()->GetRemoteReceiverService().Start();
	//ConfigurationAgent::Instance()->UseDefaultConfiguration();

	for (;;)
	{
		vTaskDelay(100);
		heap_caps_check_integrity(MALLOC_CAP_8BIT, true);
	}
}
