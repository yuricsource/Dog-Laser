
#include "I2c.h"
#include "driver/i2c.h"
#include <stdio.h>

#define I2C_DEBUG

namespace Hal
{
I2c::I2c(Gpio *IoPins, I2cPort i2cPort, Gpio::GpioIndex Sda, Gpio::GpioIndex Scl) : _gpio(IoPins), _i2cPort(i2cPort), _sdaPin(Sda), _sclPin(Scl)
{
    _gpio->ConfigOutput(_sclPin, Gpio::OutputType::PullUp);
    _gpio->ConfigOutput(_sdaPin, Gpio::OutputType::PullUp);
    _gpio->SetAlternate(_sclPin, Gpio::AltFunc::I2c);
    _gpio->SetAlternate(_sdaPin, Gpio::AltFunc::I2c);

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = static_cast<gpio_num_t>(_sdaPin);
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = static_cast<gpio_num_t>(_sclPin);
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = DefaultFrequency;
    i2c_param_config(static_cast<i2c_port_t>(_i2cPort), &conf);
    i2c_driver_install(static_cast<i2c_port_t>(_i2cPort), conf.mode, 0, 0, 0);
}

I2c::~I2c()
{

}

void I2c::ScanDevices()
{
    uint8_t i = 0;
	bool espRc = false;
    printf("\n\n");
	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	printf("00:         ");
	for (i=3; i< 0x78; i++) {
		espRc = IsDeviceConnected(i);
		if (i%16 == 0) {
			printf("\n%.2x:", i);
		}
		if (espRc) {
			printf(" %.2x", i);
		} else {
			printf(" --");
		}
	}
	printf("\n\n");
}

bool I2c::IsDeviceConnected(uint8_t address)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, AckCheck);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK)
    {
        return false;
    }
    return true;
}

bool I2c::BeginTransmission(uint8_t address)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, AckCheck);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        return false;
    }
    return true;
}

bool I2c::Send(uint8_t byte)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_write_byte(cmd, byte | I2C_MASTER_WRITE, AckCheck);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        return false;
    }
    return true;
}

bool I2c::EndTransmission()
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret == ESP_OK;
}

bool I2c::Write(uint8_t slave_addr, uint8_t *data, uint8_t len)
{
    if (len == 0)
        return false;

#ifdef DEBUG_I2C
    printf("Write addr: 0x%2X, len: %d\n", slave_addr, len);
    
    for (uint8_t i = 0; i < len; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
#endif
\
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_WRITE, AckCheck);
    i2c_master_write(cmd, data, len, static_cast<i2c_ack_type_t>(AckValue));
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
 #ifdef DEBUG_I2C
    if (ret != ESP_OK)
        printf("Error: %d\n", ret);
#endif
    return ret == ESP_OK;
}

bool I2c::WriteRegister(uint8_t slave_addr, uint8_t byteRegister, uint8_t byte)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    uint8_t commandByte[2] = {byteRegister, byte};
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_WRITE, AckCheck);
    i2c_master_write(cmd, commandByte, 2, static_cast<i2c_ack_type_t>(AckValue));
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret == ESP_OK;
}

bool I2c::ReadRegister(uint8_t slave_addr, uint8_t byteRegister, uint8_t * byte)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_READ, AckCheck);
    i2c_master_read_byte(cmd, byte, static_cast<i2c_ack_type_t>(AckValue));
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret == ESP_OK;
}

bool I2c::Read(uint8_t slave_addr, uint8_t *data, uint32_t len)
{
    if (len == 0)
        return false;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_READ, AckCheck);
    if (len > 1)
    {
        i2c_master_read(cmd, data, len - 1, static_cast<i2c_ack_type_t>(AckValue));
    }
    i2c_master_read_byte(cmd, data + len - 1, static_cast<i2c_ack_type_t>(AckValue));
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ret == ESP_OK;
}

bool I2c::RequestFrom(uint8_t slave_addr, uint32_t len)
{
    if (len == 0)
        return false;

    uint8_t data = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_READ, AckCheck);
    if (len > 1)
    {
        i2c_master_read(cmd, &data, len - 1, static_cast<i2c_ack_type_t>(AckValue));
    }
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);

    i2c_cmd_link_delete(cmd);
    if (ret == ESP_OK)
    {
        bytesToReceive = len;
        return true;
    }
    return false;
}

uint8_t I2c::Receive()
{
    uint8_t data = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    i2c_master_read_byte(cmd, &data + bytesToReceive - 1, static_cast<i2c_ack_type_t>(AckValue));
    bytesToReceive--;

    if (bytesToReceive == 0)
        i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(static_cast<i2c_port_t>(_i2cPort), cmd, 10 / portTICK_RATE_MS);

    if (bytesToReceive == 0)
        i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK)
    {
        return data;
    }

    return 0;
}
} // namespace Hal
