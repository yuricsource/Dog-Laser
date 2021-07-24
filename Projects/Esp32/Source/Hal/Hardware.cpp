
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"
#include "soc/rtc.h"

#ifndef HARDWARE_TESTER
#include "RTOSExtra.h"
#endif

namespace Hal
{

Hardware *Hardware::_pHardware;

Hardware::Hardware() :	_gpio(),
						_adc(&_gpio),
						_debugPort(&_gpio, UartPort::Uart0, 115200, Gpio::GpioIndex::Gpio3, Gpio::GpioIndex::Gpio1),
						_spiffs(),
						_rng(),
						_wifiDriver(),
						_flash(),
						_bankConfig(),
						_timerInterruptHandler(),
						_timer0(&_timerInterruptHandler, TimerSelect::Timer0),
						_timer1(&_timerInterruptHandler, TimerSelect::Timer1),
						_rmt(&_gpio, Gpio::GpioIndex::Gpio18, RmtChannel::RmtChannel0, Hal::BitsPerLed * Hal::MaxAddressableLeds, Hal::BitsPerLed),
						_rmt2(&_gpio, Gpio::GpioIndex::Gpio5, RmtChannel::RmtChannel1, Hal::BitsPerLed * Hal::MaxAddressableLeds, Hal::BitsPerLed),
						_i2c(&_gpio, Hal::I2cPort::I2c0, Gpio::GpioIndex::Gpio15, Gpio::GpioIndex::Gpio16),
						_deviceInput(&_gpio, &_adc),
						_spi(),
						_display(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, &_i2c),
						_motor1(&_gpio, Gpio::GpioIndex::Gpio18, LEDC_CHANNEL_0),
						_motor2(&_gpio, Gpio::GpioIndex::Gpio5, LEDC_CHANNEL_1),
						_laser(&_gpio, Gpio::GpioIndex::Gpio17, LEDC_CHANNEL_2),
						_wiiNunchuk(&_i2c, 0x52)
{
	esp_chip_info(&_mcuInfo);
	esp_base_mac_addr_get(_macAdrress.data());
	printf("SDK Version         		: %s\n", (char *)esp_get_idf_version());
	printf("CPU Cores           		: %d\n", _mcuInfo.cores);
	printf("APB Clock           		: %d Hz\n", GetSystemClockBase());
	printf("CPU Revision        		: %d\n", _mcuInfo.revision);
	printf("Embedded Flash      		: %s\n", (_mcuInfo.features & CHIP_FEATURE_EMB_FLASH) ? "YES" : "NO");
	printf("Wi-Fi Modle         		: %s\n", (_mcuInfo.features & CHIP_FEATURE_WIFI_BGN) ? "YES" : "NO");
	printf("Bluetooth Classic   		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BT) ? "YES" : "NO");
	printf("Bluetooth LE        		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BLE) ? "YES" : "NO");
	printf("MAC Address         		: %02X:%02X:%02X:%02X\n",
		   _macAdrress[0],
		   _macAdrress[1],
		   _macAdrress[2],
		   _macAdrress[3]);
	printf("MCU Free Heap       		: %d\n", esp_get_free_heap_size());
#ifndef HARDWARE_TESTER
	printf("MCU Project Heap Allocated	: %d\n", configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED);
#endif
	printf("Firmware Image Size	        : %d of %d\n", _bankConfig.GetCurrentBank().ImageSize, _bankConfig.GetCurrentBank().PartitionSize);
	printf("Reset Reason        		: %s\n", GetResetReasonAsString(GetResetReason()));
	printf("Running On	                : %s\n", (_bankConfig.GetCurrentBank().BankRunning == Bank::Bank1) ?
												"Bank 1" : "Bank 2");
	printf("Bank Info	                : %s %s Version: %s\n",	_bankConfig.GetCurrentBank().Date.data(),
									_bankConfig.GetCurrentBank().Time.data(),
									_bankConfig.GetCurrentBank().Version.data());
	printf("\n");

	if (_pHardware == nullptr)
		_pHardware = this;
	else
		printf("!!! Error: Only one instance of System can be created !!!\n");

#ifdef HARDWARE_TESTER
	_i2c.ScanDevices();
#endif
	// _spiffs.Mount();
	
	// initializing display
	_display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);
	_display.clearDisplay();
	_display.setTextSize(2);
	_display.setTextColor(WHITE);
	_display.setCursor(0,0);
	_display.print("  Dog WII");
	_display.display();
	
	_motor1.Init();
	_motor2.Init();
	_laser.Init();
	_laser.SetPower(30);	
}

uint32_t Hardware::GetSystemClockBase()
{
	return rtc_clk_apb_freq_get();
}

void Hardware::SoftwareReset()
{
	esp_restart();
}

void Hardware::DeepSleep(uint32_t uSeconds)
{
	esp_sleep_enable_timer_wakeup(uSeconds);
	vTaskDelay(20);
	esp_deep_sleep_start();
}

ResetReason Hardware::GetResetReason()
{
	esp_reset_reason_t info = esp_reset_reason();
	return static_cast<ResetReason>(info);
}

char *Hardware::GetResetReasonAsString(ResetReason reason)
{
	switch (reason)
	{
	case ResetReason::Unknown:
		return (char *)"Unknown";
	case ResetReason::PowerOn:
		return (char *)"Power On";
	case ResetReason::ExternalReset:
		return (char *)"ExternalReset";
	case ResetReason::SoftwareReset:
		return (char *)"Software Reset";
	case ResetReason::ExceptionPanicReset:
		return (char *)"Exception/Panic Reset";
	case ResetReason::IntWatchDog:
		return (char *)"Internal External WatchDog";
	case ResetReason::TaskWatchDog:
		return (char *)"Task WatchDog";
	case ResetReason::WatchDog:
		return (char *)"Other WatchDog";
	case ResetReason::DeepSleep:
		return (char *)"Deep Sleep";
	case ResetReason::Brownout:
		return (char *)"Brownout";
	case ResetReason::Sdio:
		return (char *)"Sdio";
	default:
		return (char *)"";
	}
}

uint32_t Hardware::GetHeapSize()
{
	multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    return info.total_free_bytes + info.total_allocated_bytes;
}

uint32_t Hardware::GetMinFreeHeap(void)
{
    return heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
}

uint32_t Hardware::GetMaxAllocHeap(void)
{
    return heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
}

int Hardware::GetRandomNumberRange(int from, int to)
{
	uint32_t result = esp_random();
	if (from == to)
		return result;

	result = result % ( to - from);
	result = from + result;
	return result;
}

uint32_t Hardware::GetRandomNumber()
{
	return esp_random();
}

uint32_t Hardware::GetPsramSize(void)
{
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    return info.total_free_bytes + info.total_allocated_bytes;
}

uint32_t Hardware::GetFreePsram(void)
{
    return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
}

uint32_t Hardware::GetMinFreePsram(void)
{
    return heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
}

uint32_t Hardware::GetMaxAllocPsram(void)
{
    return heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
}

Hardware::~Hardware()
{
}

uint32_t Hardware::Milliseconds()
{
	return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

} // namespace Hal
