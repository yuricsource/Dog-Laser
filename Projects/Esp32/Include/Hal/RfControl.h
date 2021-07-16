
#ifndef INCLUDE_HAL_RD_CONTROL_H_
#define INCLUDE_HAL_RD_CONTROL_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Rmt.h"

namespace Hal
{

class RfControl
{

public:
	static constexpr uint8_t MaxCommandSupported = 10;
	RfControl(Gpio *IoPins, Rmt* rmt);
	~RfControl();

	bool SetCommand(RfCommandArray& command, const uint8_t commandId);
	bool RunCommand(uint8_t commandId, uint8_t count = 1);

private:
	RfCommandArray _commands[MaxCommandSupported];
	Gpio *_gpio;
	Rmt *_rmt;
};
} // namespace Hal

#endif /* INCLUDE_HAL_RD_CONTROL_H_ */
