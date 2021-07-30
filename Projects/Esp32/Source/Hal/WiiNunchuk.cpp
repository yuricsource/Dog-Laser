
#include <WiiNunchuk.h>
#include "Dwt.h"

namespace Hal
{

// #define DEBUG_CONTROLLER

WiiNunchuk::WiiNunchuk(I2c* i2c, uint8_t address) :
    _i2c(i2c), _address(address)
{
#ifdef DEBUG_CONTROLLER
    if (!_i2c->IsDeviceConnected(address))
        printf("WiiNunchuk Address:%d, NOT FOUND!\n", address);
#endif
}

bool WiiNunchuk::Init()
{
    uint8_t data1[2] = {0xF0, 0x55};
    
    if (_controllerInitialized)
        return true;

    if (!_i2c->Write(_address, data1, 2))
    {
#ifdef DEBUG_CONTROLLER
        printf("WiiNunchuk Failed to write data1.\n");
#endif
        return false;
    }

    uint8_t data2[2] = {0xFB, 0x00};
    if (!_i2c->Write(_address, data2, 2))
    {
#ifdef DEBUG_CONTROLLER
        printf("WiiNunchuk Failed to write data2.\n");
#endif
        return false;
    }

    uint8_t data = 0x0;
    if (!_i2c->Write(_address, &data, 1))
    {
#ifdef DEBUG_CONTROLLER
        printf("WiiNunchuk Failed to write data3.\n");
#endif
        return false;
    }
    _controllerInitialized = true;
    return true;
}

bool WiiNunchuk::IsPresent()
{
    bool result = _i2c->IsDeviceConnected(_address);
    if (!result)
        _controllerInitialized = false;
    
    return result;
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
    if (!_controllerInitialized)
    {
        if (!Init())
            return;
    }

    if (!_i2c->Read(_address, _nunChukData.Raw, 6))
    {
#ifdef DEBUG_CONTROLLER
        printf("WiiNunchuk Failed to read data.\n");
#endif
        _controllerInitialized = false;
        return;
    }
#ifdef DEBUG_CONTROLLER

    for(uint8_t i = 0; i < 6; i ++)
    {
        printf("%d ", _nunChukData.Raw[i]);
    }
    printf("\n");
#endif
    uint8_t data = 0x0;
    if (!_i2c->Write(_address, &data, 1))
    {
#ifdef DEBUG_CONTROLLER
        printf("WiiNunchuk Failed to write data3.\n");
#endif
        _controllerInitialized = false;
        return;
    }
}


WiiNunchuk::~WiiNunchuk()
{
}

} // namespace Hal
