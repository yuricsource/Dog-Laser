
#ifndef INCLUDE_HAL_RMT_H_
#define INCLUDE_HAL_RMT_H_

//#include "Hardware.h"
#include "HalCommon.h"
#include "Gpio.h"
#include "Timer.h"
#include "Rmt.h"
#include "driver/rmt.h"
#include <array>
#include "freertos/semphr.h"
#include "DebugAssert.h"

// These values are determined by measuring pulse timing with logic 
// analyzer and adjusting to match datasheet. 
#define T0H  15  // 0 bit high time
#define T0L  35  // 1 bit high time
#define T1H  35  // low time for either bit
#define T1L  15  // low time for either bit

	/*
		In order to send bits through the leds,
		they need to respect the timing table below
		_____________________________________
		|	Bit		|	T On	|	T Off	|
		_____________________________________
		|	Bit 1	|	800ns	|	450ns	|
		|	Bit 0	|	400ns	|	850ns	|
		-------------------------------------

		The full pixel colour needs to receive 24 bits.
		If the next 24 bits comes withing the reset code timing, the first 24 bit
		will be passed to the next led, otherwise, the new 24 bits
		colours will overwrite the first led colour.
		_________________________________________________________________
		|				   pixel LedColor				|	Reset Code	|
		_________________________________________________________________
		| 8 bits Green	|	8 bits Red	|	8 bits Blue	|	  >=50us	|
		-----------------------------------------------------------------
	*/


namespace Hal
{
using std::array;

class Rmt // : public Timer::Callback
{

public:
	// @brief Gpio flass pointer, Gpio pin, RMT Channel, Buffer Size (uint32_t)
	Rmt(Gpio *IoPins, Gpio::GpioIndex transmitterPin, RmtChannel channel, uint16_t bufferSize, uint16_t unitSize = 1);
	~Rmt();
	void Write(bool wait = false);
	bool UpdateBuffer(uint32_t *buffer, uint16_t length);
	bool SetMaxUnitsToSend(uint16_t maxUnits);
	bool SetBitsPerUnit(uint16_t unitSize);

	// @brief Set the period of bit 1. Ex.: |	Bit 1	|	timeHigh = 15 -> 800ns	|	timeLow = 35-> 450ns	|
	inline bool SetTimeBitOn(const uint16_t timeHigh, const uint16_t timeLow)
	{
		if (timeHigh == 0 || timeLow == 0)
			return false;

		_timeOn.duration0 = timeHigh;
		_timeOn.duration1 = timeLow;

		return true;
	}

	// @brief Set the maximum bit to be sent. This variable has to be the last setting to be set if the maximum bit doesn't mach 
	// MaxUnitsToSend end BitsPerUnit
	bool SetMaxBitsToSend(uint16_t maxBits);
	
	// @brief Set the period of bit 0. Ex.: |	Bit 1	|	timeHigh = 35 -> 400ns	|	timeLow = 15-> 850ns	|
	inline bool SetTimeBitOff(const uint16_t timeHigh, const uint16_t timeLow)
	{
		if (timeHigh == 0 || timeLow == 0)
			return false;

		_timeOff.duration0 = timeHigh;
		_timeOff.duration1 = timeLow;
		
		return true;
	}

	enum class ProtocolSupported : uint8_t
	{
		WS2812B,
		herculiftRemoteControl,
		// Max Protol Supported
		MaxProtocolSupported
	};
	
	void SetProtocol(ProtocolSupported protocol)
	{
		uint8_t protocolIndex = static_cast<uint8_t>(protocol);
		DebugAssertMessage(protocolIndex < static_cast<uint8_t>(ProtocolSupported::MaxProtocolSupported),
			"Protocol not supported. Protocol Id: %d", protocolIndex);
		
		_timeOn = ProtocolSupportedList[protocolIndex][0];
		_timeOff = ProtocolSupportedList[protocolIndex][1];

		switch(protocol)
		{
			case ProtocolSupported::WS2812B:
			{
				rmt_config_t config;
				config.rmt_mode = RMT_MODE_TX;
				config.channel = static_cast<rmt_channel_t>(_rmtBuffer.Channel);
				config.gpio_num = static_cast<gpio_num_t>(_transmitterPin);
				config.mem_block_num = 3;
				config.tx_config.loop_en = false;
				config.tx_config.carrier_en = false;
				config.tx_config.idle_output_en = true;
				config.tx_config.idle_level = static_cast<rmt_idle_level_t>(0);
				config.clk_div = 2;
				ESP_ERROR_CHECK(rmt_config(&config));
			}
			break;
			case  ProtocolSupported::herculiftRemoteControl:
			{
				rmt_config_t config;
				config.rmt_mode = RMT_MODE_TX;
				config.channel = static_cast<rmt_channel_t>(_rmtBuffer.Channel);
				config.gpio_num = static_cast<gpio_num_t>(_transmitterPin);
				config.mem_block_num = 3;
				config.tx_config.loop_en = false;
				config.tx_config.carrier_en = false;
				config.tx_config.idle_output_en = true;
				config.tx_config.idle_level = static_cast<rmt_idle_level_t>(0);
				config.clk_div = 64;
				ESP_ERROR_CHECK(rmt_config(&config));
			}
			break;

			default:
				break;
		}

	}

	struct RmtBufferLed
	{
		uint16_t Index = 0;
		uint16_t MaxUnitsToSend = 0;
		uint16_t MaxBits = 0;
		uint16_t UnitSize = 0;
		uint16_t BufferSize = 0;
		xSemaphoreHandle Semaphore;
		rmt_item32_t* Buffer = nullptr;
		RmtChannel Channel = RmtChannel::RmtChannel0;
	};
private:
	RmtBufferLed _rmtBuffer = {};
	static void IRAM_ATTR doneOnChannel(rmt_channel_t channel, void * arg);
	Gpio *_gpio;
	Gpio::GpioIndex _transmitterPin;

	rmt_item32_t ProtocolSupportedList [static_cast<uint8_t>(ProtocolSupported::MaxProtocolSupported)][2] = 
	{
		{{35, 1, 15, 0}, {15, 1, 35, 0}},
		{{920, 1, 505, 0}, {440, 1, 983, 0}}
	};

	rmt_item32_t _timeOn = {{{T1H, 1, T1L, 0}}};
	rmt_item32_t _timeOff = {{{T0H, 1, T0L, 0}}};
};
} // namespace Hal

#endif /* INCLUDE_HAL_RMT_H_ */
