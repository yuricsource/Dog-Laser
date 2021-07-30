#include "InputScannerService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"
#include "Hardware.h"
#include "StatusAgent.h"

namespace Applications
{
using Hal::Hardware;
using Hal::DeviceInput;
using Hal::WiiNunchuk;
using Hal::DigitalInputIndex;
using Hal::AnalogInputIndex;

using Status::InputStatusList;
using Status::StatusAgent;

InputScannerService::InputScannerService() : cpp_freertos::Thread("ISNSVC", configINPUTSCANNERSVC_STACK_DEPTH, 3)
{
}

void InputScannerService::Run()
{
    Logger::LogInfo(Logger::LogSource::InputScanner, "Input Scanner Service Started");
	
	// Gathering low laywer references
	DeviceInput& deviceInput = Hal::Hardware::Instance()->GetDeviceInput();
	WiiNunchuk& controller = Hal::Hardware::Instance()->GetController();

	// Getting input and controller Status
	InputStatusList& inputList = StatusAgent::Instance()->GetInputStatusList();

	for(;;)
	{
		DelayUntil(100);

		// Read physical input and save in the status list
		inputList.GetInput(Configuration::InputIndex::ButtonBack).
			SetDigitalLevel(deviceInput.GetDigitalInput(DigitalInputIndex::UserButtonReturn));

		inputList.GetInput(Configuration::InputIndex::ButtonOk).
			SetDigitalLevel(deviceInput.GetDigitalInput(DigitalInputIndex::UserButtonEnter));	

		inputList.GetInput(Configuration::InputIndex::PotLaserDelay).
			SetAnalogLevel(deviceInput.GetAnalogInput(AnalogInputIndex::LaserDelay));	
			
		inputList.GetInput(Configuration::InputIndex::PotLaserPower).
			SetAnalogLevel(deviceInput.GetAnalogInput(AnalogInputIndex::LaserPower));			
	}
}

} // namespace Applications
