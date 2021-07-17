
#include "HalCommon.h"
#include "ServoMotor.h"
#include "Dwt.h"
#include "string.h"

namespace Hal
{

ServoMotor::ServoMotor(Gpio *IoPins, Timer* timer, Rmt* rmt) : _gpio(IoPins), _timer(timer), _rmt(rmt)
{
	_timer->AddCallback(this);
}

void IRAM_ATTR ServoMotor::TimerCallback()
{
}

ServoMotor::~ServoMotor()
{
}

void ServoMotor::Refresh()
{
	// uint32_t* ledsBufferPointer = reinterpret_cast<uint32_t*>(_outputLeds.data());
	// _rmt->UpdateBuffer(ledsBufferPointer, _outputLeds.size());
	// _rmt->Write();
}

} // namespace Hal
