
#include "HalCommon.h"
#include "Leds.h"
#include "Dwt.h"
#include "string.h"

namespace Hal
{

Leds::Leds(Gpio *IoPins, Timer* timer, Rmt* rmt) : _gpio(IoPins), _timer(timer), _rmt(rmt),
														_maxLeds(Hal::MaxAddressableLeds)
{
	for (uint8_t i = 0; i < LedIndicatorsCount; i++)
	{
		_gpio->SetMode(LedIndicatorPin[i], Gpio::Mode::OutputOpenDrain);
		_gpio->Reset(LedIndicatorPin[i]);
	}
}

void Leds::SetIndicatorLed(LedIndex led)
{
	_gpio->Set(LedIndicatorPin[static_cast<uint8_t>(led)]);
}

void Leds::ResetIndicatorLed(LedIndex led)
{
	_gpio->Reset(LedIndicatorPin[static_cast<uint8_t>(led)]);
}

void Leds::TimerCallback()
{
}

Leds::~Leds()
{
}

bool Leds::SetLedsCount(uint16_t ledIndex)
{
	if (ledIndex > _outputLeds.size())
		return false;

	_maxLeds = ledIndex;
	_rmt->SetMaxUnitsToSend(_maxLeds);
	return true;
}
bool Leds::SetLedColor(uint16_t ledIndex, Led led)
{
	if (ledIndex > _outputLeds.size() || ledIndex > _maxLeds)
		return false;

	_outputLeds.data()[ledIndex].Value = led.Value;
	//Refresh();
	return true;
}

void Leds::SetAllLeds(LedsArray &array)
{
	memcpy(_outputLeds.data(), array.data(), _outputLeds.size());
	Refresh();
}

void Leds::ResetAllLeds()
{
	LedsArray ledEmpty = {};
	memcpy(_outputLeds.data(), ledEmpty.data(), _outputLeds.size());
	Refresh();
}

void Leds::Refresh()
{
	uint32_t* ledsBufferPointer = reinterpret_cast<uint32_t*>(_outputLeds.data());
	_rmt->UpdateBuffer(ledsBufferPointer, _outputLeds.size());
	_rmt->Write();
}

} // namespace Hal
