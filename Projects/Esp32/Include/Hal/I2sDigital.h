
#ifndef INCLUDE_HAL_I2S_DIGITAL_H_
#define INCLUDE_HAL_I2S_DIGITAL_H_

#include "HalCommon.h"
#include "driver/i2s.h"
#include "Gpio.h"
#include "esp32/rom/lldesc.h"

namespace Hal
{

class I2sDigital
{
public:
	I2sDigital(Gpio *gpio, Gpio::GpioIndex i2sPin, I2sBus i2sBus);
	~I2sDigital(); 
	void Stop();
	bool UpdateChannelClock(I2sBitSample sampleBit, uint32_t sampleRate);
	void Start(Gpio::GpioIndex data);
	void Refresh();
	bool Send(const uint8_t *buffer, uint32_t size, uint32_t timeout = 0);
	bool Init( I2sBitSample bitSample, uint32_t bufferSize, uint32_t sampleRate, bool autoTrigger);
	bool UpdateBuffer(uint8_t *sssssbuffer, uint32_t size, uint32_t offset);
private:
	Gpio *_gpio;
	Gpio::GpioIndex _i2sPin;
	I2sBus _i2s;
	I2sBitSample _bitSample;
	uint32_t _sampleRate;
	uint32_t _bufferSize;
	lldesc_t *_dmaBuffer;
	bool _autoTrigger;
	uint8_t _bufferCounts;
	static constexpr uint32_t DefaultConfigurationModeData = (I2S_MODE_MASTER | I2S_MODE_TX);
	static constexpr uint32_t BytesPerBuffer = 600;
	bool _enabled = false;
};
} // namespace Hal

#endif /* INCLUDE_HAL_I2S_DIGITAL_H_ */
