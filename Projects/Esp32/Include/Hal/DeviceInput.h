
#ifndef INCLUDE_HAL_DEVICEINPUT_H_
#define INCLUDE_HAL_DEVICEINPUT_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Adc.h"

namespace Hal
{

class DeviceInput
{

public:
	static constexpr uint8_t MaxDigitalInputs = 2;
	static constexpr uint8_t MaxAnalogInputs = 2;

	enum class DigitalInputIndex : uint8_t
	{
		UserButtonEnter,
		UserButtonReturn,
	};
	
	enum class AnalogInputIndex : uint8_t
	{
		Color,
		Brightness,
	};

	uint16_t GetAnalogInput(AnalogInputIndex input, uint8_t averageSamples = 1);
	DeviceInput(Gpio* IoPins, Adc* adc);
	~DeviceInput();
	bool GetDigitalInput(DigitalInputIndex input);

private:
	Gpio* _gpio;
	Adc* _adc;
	Gpio::GpioIndex digitalInputIndex[MaxDigitalInputs] =
	{
		Gpio::GpioIndex::Gpio26,
		Gpio::GpioIndex::Gpio18
	};
	
	Gpio::GpioIndex analogIndex[MaxAnalogInputs] =
	{
		Gpio::GpioIndex::Gpio33,
		Gpio::GpioIndex::Gpio32
	};
	
	Adc::AdcIndex analogAdcIndex[MaxAnalogInputs] = {};
};
} // namespace Hal

#endif /* INCLUDE_HAL_DEVICEINPUT_H_ */
