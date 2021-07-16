
#include "RfControl.h"
#include <string>
#include <cstring>
#include "Dwt.h"
#include "Hardware.h"
namespace Hal
{

RfControl::RfControl(Gpio *IoPins, Rmt* rmt) : _gpio(IoPins), _rmt(rmt)
{
	memset(&_commands[0], 0, sizeof(_commands[0]));
	_commands[0] = {0x22437856, 0xF3BA1FFB, 0x80000000};
	_commands[1] = {0x38BBF977, 0x079B877B, 0x40000000};
	_commands[2] = {0xAB6B1BE8, 0x079B877D, 0x40000000};
	// _commands[0] = {0x00000008, 0xFB1FBAF3, 0x56784322};
}

bool RfControl::SetCommand(RfCommandArray& command, const uint8_t commandId)
{
	if (commandId > MaxCommandSupported)
		return false;

	memcpy(_commands[commandId].data(), command.data(), _commands[commandId].size());
	return true;
}

bool RfControl::RunCommand(uint8_t commandId, uint8_t count)
{
	RfCommandArray commandToSend = {};
	
	memcpy(commandToSend.data(), _commands[commandId].data(), sizeof(RfCommandArray));
	commandToSend.data()[0] = Hardware::Instance()->GetRandomNumber();

	for (uint8_t i = 0; i < count; i++)
	{
		Dwt::DelayMilliseconds(12);
		_rmt->SetProtocol(Rmt::ProtocolSupported::herculiftRemoteControl);
		// first send the 12 pulses
		_rmt->SetTimeBitOn(440, 500);
		_rmt->SetTimeBitOff(440, 500);
		uint32_t pulses[3];

		memset(pulses, 0xFF, sizeof(pulses));
		
		_rmt->SetBitsPerUnit(12);
		_rmt->SetMaxUnitsToSend(1);
		_rmt->UpdateBuffer(pulses, sizeof(pulses));
		_rmt->Write(true);
		Dwt::DelayMilliseconds(3);
		_rmt->SetProtocol(Rmt::ProtocolSupported::herculiftRemoteControl);
		
		_rmt->SetBitsPerUnit(32);
		_rmt->SetMaxUnitsToSend(3);
		_rmt->SetMaxBitsToSend(66);
		uint32_t* commandBufferPointer = reinterpret_cast<uint32_t*>(commandToSend.data()); 
		_rmt->UpdateBuffer(commandBufferPointer, 9); // how many bytes has to be sent
		_rmt->Write(true);
		
		Dwt::DelayMilliseconds(50);
	}
	return true;
}

RfControl::~RfControl()
{
}

} // namespace Hal
