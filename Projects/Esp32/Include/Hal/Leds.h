
#ifndef INCLUDE_HAL_LEDS_H_
#define INCLUDE_HAL_LEDS_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Timer.h"
#include "Rmt.h"

namespace Hal
{

class Leds : public Timer::Callback
{

public:
	enum class LedIndex : uint8_t
	{
		Red,
		Orange,
		Yellow,
		Green
	};

	Leds(Gpio *IoPins, Timer* timer, Rmt* rmt);
	~Leds();

	bool SetLedColor(uint16_t ledIndex, Led led);
	void SetAllLeds(LedsArray &array);
	void ResetAllLeds();
	void Refresh();
	bool SetLedsCount(uint16_t ledIndex);
	void SetIndicatorLed(LedIndex led);
	void ResetIndicatorLed(LedIndex led);
private:

	static constexpr uint16_t MaxLeds = MaxAddressableLeds;
	static constexpr uint16_t BytesPerColor = 3;
	static constexpr uint8_t LedColors = 3;
	static constexpr uint8_t LedIndicatorsCount = 4;

	Gpio::GpioIndex LedIndicatorPin[LedIndicatorsCount] =
	{
		Gpio::GpioIndex::Gpio35, // Led Red
		Gpio::GpioIndex::Gpio34, // Led Orange
		Gpio::GpioIndex::Gpio39, // Led Yellow
		Gpio::GpioIndex::Gpio36, // Led Green
	};


	void TimerCallback() override;

	Gpio *_gpio;
	Timer *_timer;
	Rmt *_rmt;
	uint16_t _maxLeds;
	static constexpr Led BlankColor = {0, 255, 0};
	
	LedsArray _outputLeds = { };
};
} // namespace Hal

#endif /* INCLUDE_HAL_LEDS_H_ */
