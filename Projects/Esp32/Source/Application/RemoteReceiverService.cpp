#include "RemoteReceiverService.h"
#include "ConfigurationAgent.h"
#include "Hardware.h"
#include "BankConfiguration.h"
#include "Md5Hash.h"
#include "HalCommon.h"
#include "Flash.h"
#include "Timer.h"

namespace Applications
{
using Hal::Gpio;

RemoteReceiverService::RemoteReceiverService() : cpp_freertos::Thread("REMSVC", configAUDPLAYERSVC_STACK_DEPTH, 3)
{
	_hardware = Hardware::Instance();
}

RemoteReceiverService::~RemoteReceiverService()
{
}

void RemoteReceiverService::Run()
{
	Delay(1000);
	Logger::LogInfo(Utilities::Logger::LogSource::RemoteReceiver, "Remote Receiver Service Initialized.");
	Hal::Hardware* hardware = Hal::Hardware::Instance();
#ifdef SPG_GATE
	bool buttomState = false;
	uint8_t codeId = 2;
	for(;;)
	{
		Delay(300);
		
		if (hardware->GetDeviceInput().GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonEnter) != buttomState)
		{
			buttomState = hardware->GetDeviceInput().GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonEnter);
			if (!buttomState)
			{
				Logger::LogInfo(Utilities::Logger::LogSource::RemoteReceiver, "Transmitting Code. Code ID:%d", codeId);
				hardware->GetRfControl().RunCommand(codeId, 10);
			}
		}
	}
#else
	hardware->GetCodeReceiver().Stop();
	hardware->GetCodeReceiver().Start();
	for(;;)
	{
		Delay(500);
		
		if (hardware->GetCodeReceiver().GetState() == Hal::CodeReceiver::CodeLearnerState::Finished)
		{
			Logger::LogInfo(Utilities::Logger::LogSource::RemoteReceiver, "Code Received.");
			hardware->GetCodeReceiver().PrintResult();
			if (hardware->GetCodeReceiver().GetCode()[0] == 0x25E00000) //0x25E00000
			{
				Logger::LogInfo(Utilities::Logger::LogSource::RemoteReceiver, "Code Found.");
				hardware->GetRfControl().RunCommand(0, 10);
			}
			else
			{
				Logger::LogInfo(Utilities::Logger::LogSource::RemoteReceiver, "Wrong Code: 0x%08X", 
				hardware->GetCodeReceiver().GetCode()[0]);
			}
			hardware->GetCodeReceiver().Reset();
		}
	}
#endif
}

} // namespace Applications
