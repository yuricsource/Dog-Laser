
#include <WiiNunchuk.h>
#include "Dwt.h"

namespace Hal
{

WiiNunchuk::WiiNunchuk(I2c* i2c, uint8_t address) :
    _i2c(i2c), _address(address)
{
    if (!_i2c->IsDeviceConnected(address))
        printf("WiiNunchuk Address:%d, NOT FOUND!\n", address);
}

bool WiiNunchuk::Init()
{
    uint8_t data1[2] = {0xF0, 0x55};
    if (!_i2c->Write(_address, data1, 2))
    {
        printf("WiiNunchuk Failed to write data1.\n");
        return false;
    }

    uint8_t data2[2] = {0xFB, 0x00};
    if (!_i2c->Write(_address, data2, 2))
    {
        printf("WiiNunchuk Failed to write data2.\n");
        return false;
    }

    uint8_t data = 0x0;
    if (!_i2c->Write(_address, &data, 1))
    {
        printf("WiiNunchuk Failed to write data3.\n");
        return false;
    }

    return true;
}

bool WiiNunchuk::IsPresent()
{
    return _i2c->IsDeviceConnected(_address);
}

uint8_t WiiNunchuk::GetJoystickY()
{
    uint16_t result = _nunChukData.Control.AxisY * 100 / 255;
    return result;
}

uint8_t WiiNunchuk::GetJoystickX()
{
    uint16_t result = _nunChukData.Control.AxisX * 100 / 255;
    return result;
}

void WiiNunchuk::Update()
{
    if (!_i2c->Read(_address, _nunChukData.Raw, 6))
    {
        printf("WiiNunchuk Failed to read data.\n");
        return;
    }

    for(uint8_t i = 0; i < 6; i ++)
    {
        printf("%d ", _nunChukData.Raw[i]);
    }
    printf("\n");
    uint8_t data = 0x0;
    if (!_i2c->Write(_address, &data, 1))
    {
        printf("WiiNunchuk Failed to write data3.\n");
        return;
    }
}


WiiNunchuk::~WiiNunchuk()
{
}

} // namespace Hal
