
#ifndef INCLUDE_HAL_SERVO_MOTOR_H_
#define INCLUDE_HAL_SERVO_MOTOR_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Timer.h"
#include "Rmt.h"

namespace Hal
{

class ServoMotor : public Timer::Callback
{

public:

	ServoMotor(Gpio *IoPins, Timer* timer, Rmt* rmt);
	~ServoMotor();

	void Refresh();

private:
	void IRAM_ATTR TimerCallback() override;

	Gpio *_gpio;
	Timer *_timer;
	Rmt *_rmt;
	uint16_t _maxLeds;
};
} // namespace Hal

#endif /* INCLUDE_HAL_LEDS_H_ */
