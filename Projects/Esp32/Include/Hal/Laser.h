
#ifndef INCLUDE_HAL_LASER_MOTOR_H_
#define INCLUDE_HAL_LASER_MOTOR_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/ledc.h"

namespace Hal
{

class Laser
{

public:

	Laser(Gpio *IoPins, Gpio::GpioIndex pin, ledc_channel_t channel);
	~Laser();
	void Init();
	void Refresh();
	void SetPower(uint8_t percentage);

private:
	static const uint16_t MaxResolution = 8192;
	Gpio *_gpio;
	Gpio::GpioIndex _pin;
	ledc_channel_config_t ledc_channel = {};
	ledc_timer_config_t ledc_timer = {};
	ledc_channel_t _channel;
};
} // namespace Hal

#endif /* INCLUDE_HAL_LASER_MOTOR_H_ */
