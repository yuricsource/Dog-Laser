
#ifndef INCLUDE_HAL_WII_NUNCHUK_H_
#define INCLUDE_HAL_WII_NUNCHUK_H_

#include "HalCommon.h"
#include "I2c.h"

namespace Hal
{

class WiiNunchuk
{
public:
	WiiNunchuk(I2c* i2c, uint8_t address);
	~WiiNunchuk();
	bool Init();
	void Update();
	uint8_t GetJoystickY();
	uint8_t GetJoystickX();
	bool IsPresent();
private:
	I2c* _i2c;
	uint8_t _address;
	static const uint8_t RawDataSize = 6;
	bool _controllerInitialized = false;
	union NunChukData
	{
		struct
		{
			uint8_t AxisX;
			uint8_t AxisY;
			uint8_t AccelX;
			uint8_t AccelY;
			uint8_t AccelZ;
			uint8_t Buttons;
		}Control;
		uint8_t Raw[RawDataSize];
	};
	NunChukData _nunChukData = {};
};

} // namespace Hal

#endif /* INCLUDE_HAL_WII_NUNCHUK_H_ */
