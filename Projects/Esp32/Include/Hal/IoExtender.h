
#ifndef INCLUDE_HAL_IO_EXTENDER_H_
#define INCLUDE_HAL_IO_EXTENDER_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "I2c.h"

namespace Hal
{

class IoExtender
{
public:
	IoExtender(Gpio *IoPins, I2c *i2c, Gpio::GpioIndex resetPin, uint8_t deviceAddress);
	~IoExtender();

	enum class IoPin : uint8_t
	{
		Gpio1,
		Gpio2,
		Gpio3,
		Gpio4,
		Gpio5,
		Gpio6,
		Gpio7,
		Gpio8
	};
	
	/// @brief Get the input/output status. 
	bool Get(IoPin gpio);
	
	/// @brief Set the output state.
	void Set(IoPin gpio, bool state = true);

	/// @brief Set All pins that are configured as output.
	void SetAll();

	/// @brief Reset the output state.
	void Reset(IoPin gpio);

	/// @brief Reset All pins that are configured as output.
	void ResetAll();

	/// @brief Configure the Gpio as input.
	void ConfigureInput(IoPin gpio);

	/// @brief Configure the all Gpio as input. 0b00010011 -> turns output 1, 2 and 5 in input. 
	void ConfigureInput(uint8_t value);

	/// @brief Configure the Gpio as output.
	void ConfigureOutput(IoPin gpio);

	/// @brief Configure the all Gpio as output. 0b00010011 -> turns output 1, 2 and 5 in outputs. 
	void ConfigureOutput(uint8_t value);

	/// @brief Flush the input/output and the Io configuration
	void Refresh(bool updateConfig = false);

	/// @brief Returns the input register
	bool GetInputs();

private:
	
	// I2C register addresses
	static constexpr uint8_t InputPortRegister 			= 0x00; 
	static constexpr uint8_t OutputPortRegister 		= 0x01; 
	static constexpr uint8_t PolarityInversionRegister 	= 0x02; 
	static constexpr uint8_t ConfigurationRegister 		= 0x03; 

	union IoExtenderPins
	{
		struct
		{
			uint8_t Gpio1 : 1;
			uint8_t Gpio2 : 1;
			uint8_t Gpio3 : 1;
			uint8_t Gpio4 : 1;
			uint8_t Gpio5 : 1;
			uint8_t Gpio6 : 1;
			uint8_t Gpio7 : 1;
			uint8_t Gpio8 : 1;
		}Status;
		uint8_t Value;
	};

	static_assert(sizeof(IoExtenderPins) == 1, "IoExtenderPins has invalid size.");

	/// @brief Get the input/output from struct. 
	bool getIoStatus(IoPin gpio);

	/// @brief Check if the Gpio pin is configured as input.
	bool isInput(IoPin gpio);

	/// @brief Check if the Gpio pin is configured as input.
	bool getBit(IoPin gpio, IoExtenderPins &ioExtender);

	/// @brief Set bit in the IoExtenderPins register.
	void setBit(IoPin gpio, IoExtenderPins &ioExtender, bool state);

	/// @brief Write into Io Extender Register
	bool writeRegister(uint8_t Register, uint8_t value);

	/// @brief Read Io Extender Register
	uint8_t readRegister(uint8_t Register);

	Gpio *_gpio;
	I2c *_i2c;
	uint8_t _address;
	bool _deviceOnline;
	IoExtenderPins _ioMode;
	IoExtenderPins _ioStatus;
	Gpio::GpioIndex _resetPin;
};

} // namespace Hal

#endif /* INCLUDE_HAL_IO_EXTENDER_H_ */
