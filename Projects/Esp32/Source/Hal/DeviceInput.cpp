
#include "HalCommon.h"
#include "DeviceInput.h"

namespace Hal
{

DeviceInput::DeviceInput(Gpio* IoPins, Adc* adc) : _gpio(IoPins), _adc(adc)
{
	// Initializing all Digital Inputs
	for (auto &digital : digitalInputIndex)
	{
		_gpio->ConfigInput(digital, Gpio::Pull::Up);
	}

	// Initializing all Analog Inputs
	for (uint8_t i = 0; i < MaxAnalogInputs; i ++)
	{
		analogAdcIndex[i] = _adc->GetAdcIndexFromGpio(analogIndex[i]);
		_adc->InitAdc(analogAdcIndex[i]);
	}
}

DeviceInput::~DeviceInput()
{
}

uint16_t DeviceInput::GetAnalogInput(AnalogInputIndex input, uint8_t averageSamples)
{
	return _adc->GetAdcValue(analogAdcIndex[static_cast<uint8_t>(input)], averageSamples);;
}

bool DeviceInput::GetDigitalInput(DigitalInputIndex input)
{
	return _gpio->Get(digitalInputIndex[static_cast<uint8_t>(input)]);
}

} // namespace Hal
