
#include "SdCard.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

namespace Hal
{

	SdCard::SdCard(Gpio *gpio, Gpio::GpioIndex miso, Gpio::GpioIndex mosi,
				   Gpio::GpioIndex clock, Gpio::GpioIndex cs) : _gpio(gpio), _miso(miso), _mosi(mosi),
																_clock(clock), _cs(cs)
	{
	}

	bool SdCard::Mount()
	{
		if (isMounted)
			return true;

		// Reseting the pins
		_gpio->SetMode(_miso, Gpio::Mode::Input);
		_gpio->SetPull(_miso, Gpio::Pull::Up);
		_gpio->SetAlternate(_miso, Gpio::AltFunc::Spi);
		_gpio->SetMode(_mosi, Gpio::Mode::Input);
		_gpio->SetPull(_mosi, Gpio::Pull::Up);
		_gpio->SetAlternate(_mosi, Gpio::AltFunc::Spi);
		_gpio->SetMode(_clock, Gpio::Mode::Input);
		_gpio->SetPull(_clock, Gpio::Pull::Down);
		_gpio->SetAlternate(_clock, Gpio::AltFunc::Spi);
		_gpio->SetMode(_cs, Gpio::Mode::Input);
		_gpio->SetPull(_cs, Gpio::Pull::Up);
		_gpio->SetAlternate(_cs, Gpio::AltFunc::Spi);

		// sdmmc_host_t host = SDSPI_HOST_DEFAULT();
		sdspi_slot_config_t slotConfig = {};
		slotConfig.gpio_miso = static_cast<gpio_num_t>(_miso);
		slotConfig.gpio_mosi = static_cast<gpio_num_t>(_mosi);
		slotConfig.gpio_sck = static_cast<gpio_num_t>(_clock);
		slotConfig.gpio_cs = static_cast<gpio_num_t>(_cs);
		slotConfig.gpio_cd = PinNotConfigured;
		slotConfig.gpio_wp = PinNotConfigured;
		slotConfig.gpio_int = PinNotConfigured;
		slotConfig.dma_channel = 1;

		sdmmc_host_t host = {};

		host.flags = SDMMC_HOST_FLAG_SPI;
		host.slot = HSPI_HOST;
		host.max_freq_khz = SDMMC_FREQ_DEFAULT;
		host.io_voltage = 3.3f;
		host.init = &sdspi_host_init;
		host.set_bus_width = nullptr;
		host.get_bus_width = nullptr;
		host.set_bus_ddr_mode = nullptr;
		host.set_card_clk = &sdspi_host_set_card_clk;
		host.do_transaction = &sdspi_host_do_transaction;
		host.deinit = &sdspi_host_deinit;
		host.io_int_enable = &sdspi_host_io_int_enable;
		host.io_int_wait = &sdspi_host_io_int_wait;
		host.command_timeout_ms = 0;
		
		esp_vfs_fat_sdmmc_mount_config_t mountConfig = {};
		mountConfig.format_if_mount_failed = false;
		mountConfig.max_files = 5;
		mountConfig.allocation_unit_size = 16 * 1024;

		sdmmc_card_t *card;
		esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slotConfig, &mountConfig, &card);

		if (ret != ESP_OK)
		{
			if (ret == ESP_FAIL)
			{
				printf("Failed to mount filesystem. "
					   "If you want the card to be formatted, set format_if_mount_failed = true.");
			}
			else
			{
				printf("Failed to initialize the card (%s). "
					   "Make sure SD card lines have pull-up resistors in place.",
					   esp_err_to_name(ret));
			}
			return false;
		}

		isMounted = true;
		return true;
	}

	void SdCard::Unmount()
	{
		esp_vfs_fat_sdmmc_unmount();
		_gpio->SetMode(Gpio::GpioIndex::Gpio4, Gpio::Mode::Input);
		_gpio->SetPull(Gpio::GpioIndex::Gpio4, Gpio::Pull::Down);
		isMounted = false;
	}

	bool SdCard::IsMounted()
	{
		return isMounted;
	}

	SdCard::~SdCard()
	{
	}
} // namespace Hal
