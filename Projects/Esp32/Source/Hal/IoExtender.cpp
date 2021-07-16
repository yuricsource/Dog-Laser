#include "IoExtender.h"
#include "DebugAssert.h"
#include "Dwt.h"
#include <stdio.h>
#include <stdlib.h>
#include "DebugAssert.h"


#define DEBUG_IO_EXTENDER

namespace Hal
{

IoExtender::IoExtender(Gpio *IoPins, I2c *i2c, Gpio::GpioIndex resetPin, uint8_t deviceAddress) :
						_gpio(IoPins), _i2c(i2c), _address(deviceAddress), _deviceOnline(false),
						_resetPin(resetPin)
{
	// Set all Gpio as Input
	_ioMode.Value = 0xFF;
	_ioStatus.Value = 0;
	//if (_gpio->Get(_resetPin) == false)
	{
		_gpio->ConfigOutput(_resetPin, Gpio::OutputType::PullUp);
		_gpio->Reset(_resetPin);
		Dwt::DelayMilliseconds(10);
		_gpio->Set(_resetPin);
	}

	if(_i2c->IsDeviceConnected(_address))
		_deviceOnline = true;
	else
		printf("IO extender not found\n");

	if (_deviceOnline)
		Refresh(true);
}

IoExtender::~IoExtender()
{
	
}

bool IoExtender::writeRegister(uint8_t Register, uint8_t value)
{
	uint8_t data = value;
	_i2c->WriteRegister(_address, Register, data);
	data = Register;
	_i2c->Write(_address, &data, 1);
	_i2c->ReadRegister(_address, Register, &data);
	DebugAssertMessage(data == value, "Address : %d, Register : %d, Byte Written %d, Byte Read %d", _address, Register, value, data);
	return data == value;
}

uint8_t IoExtender::readRegister(uint8_t Register)
{
	uint8_t data = Register;
	_i2c->Write(_address, &data, 1);
	_i2c->ReadRegister(_address, Register, &data);
	return data;
}

bool IoExtender::Get(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	if (isInput(gpio))
		Refresh();

	return getIoStatus(gpio);
}

bool IoExtender::GetInputs()
{
	if (!_deviceOnline)
		return false;

	Refresh();

	return _ioStatus.Value;
}

void IoExtender::Set(IoPin gpio, bool state)
{
	if (!_deviceOnline)
		return;

	setBit(gpio, _ioStatus, state);
	Refresh();
	return;
}

void IoExtender::SetAll()
{
	if (!_deviceOnline)
		return;

	_ioStatus.Value = 0xFF;

	Refresh();
	return;
}

void IoExtender::ResetAll()
{
	if (!_deviceOnline)
		return;

	_ioStatus.Value = 0;

	Refresh();
	return;
}

void IoExtender::Reset(IoPin gpio)
{
	if (!_deviceOnline)
		return;

	setBit(gpio, _ioStatus, false);
	Refresh();
	return;
}

void IoExtender::ConfigureInput(IoPin gpio)
{
	if (!_deviceOnline)
		return;

	setBit(gpio, _ioMode, true);
	Refresh(true);
}

void IoExtender::ConfigureInput(uint8_t value)
{
	if (!_deviceOnline)
		return;

	_ioMode.Value |= value;
	Refresh(true);
}

void IoExtender::ConfigureOutput(IoPin gpio)
{
	if (!_deviceOnline)
		return;
	setBit(gpio, _ioMode, false);
	Refresh(true);
}

void IoExtender::ConfigureOutput(uint8_t value)
{
	if (!_deviceOnline)
		return;
	value = ~value;
	_ioMode.Value &= value;
	Refresh(true);
}

void IoExtender::Refresh(bool updateConfig)
{
	if (!_deviceOnline)
		return;

	if (updateConfig)
	{
		// Update configuration in the Chip
		writeRegister(PolarityInversionRegister, 0);
		writeRegister(ConfigurationRegister, _ioMode.Value);
#ifdef DEBUG_IO_EXTENDER
		printf("IoExtender: Refreshing configuration Register: %x\n", _ioMode.Value);
#endif
	}
	
	writeRegister(OutputPortRegister, _ioStatus.Value);
	_ioStatus.Value = readRegister(InputPortRegister);
#ifdef DEBUG_IO_EXTENDER
		printf("IoExtender: OutputInput Register: %x\n", _ioStatus.Value);
#endif
}

bool IoExtender::isInput(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	return getBit(gpio, _ioMode);
}

bool IoExtender::getIoStatus(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	return getBit(gpio, _ioStatus);
}

bool IoExtender::getBit(IoPin gpio, IoExtenderPins &ioExtender)
{
	switch (gpio)
	{
	case IoPin::Gpio1:
		return ioExtender.Status.Gpio1;
		break;
	case IoPin::Gpio2:
		return ioExtender.Status.Gpio2;
		break;
	case IoPin::Gpio3:
		return ioExtender.Status.Gpio3;
		break;
	case IoPin::Gpio4:
		return ioExtender.Status.Gpio4;
		break;
	case IoPin::Gpio5:
		return ioExtender.Status.Gpio5;
		break;
	case IoPin::Gpio6:
		return ioExtender.Status.Gpio6;
		break;
	case IoPin::Gpio7:
		return ioExtender.Status.Gpio7;
		break;
	case IoPin::Gpio8:
		return ioExtender.Status.Gpio8;
		break;
	default:
		break;
	}
	return false;
}

void IoExtender::setBit(IoPin gpio, IoExtenderPins &ioExtender, bool state)
{
	switch (gpio)
	{
	case IoPin::Gpio1:
		ioExtender.Status.Gpio1 = state;
		break;
	case IoPin::Gpio2:
		ioExtender.Status.Gpio2 = state;
		break;
	case IoPin::Gpio3:
		ioExtender.Status.Gpio3 = state;
		break;
	case IoPin::Gpio4:
		ioExtender.Status.Gpio4 = state;
		break;
	case IoPin::Gpio5:
		ioExtender.Status.Gpio5 = state;
		break;
	case IoPin::Gpio6:
		ioExtender.Status.Gpio6 = state;
		break;
	case IoPin::Gpio7:
		ioExtender.Status.Gpio7 = state;
		break;
	case IoPin::Gpio8:
		ioExtender.Status.Gpio8 = state;
		break;
	default:
		break;
	}
	return;
}

}; // namespace Hal
