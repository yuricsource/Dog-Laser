#include "InputScannerService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"

namespace Applications
{

InputScannerService::InputScannerService() : cpp_freertos::Thread("ISNSVC", configINPUTSCANNERSVC_STACK_DEPTH, 3)
{
}

void InputScannerService::Run()
{
    Logger::LogInfo(Logger::LogSource::InputScanner, "Input Scanner Service Started");

	for(;;)
	{
		vTaskDelay(1000);
	}
}

} // namespace Applications
