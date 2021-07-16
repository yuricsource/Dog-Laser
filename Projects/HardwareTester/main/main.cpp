#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_attr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "Hardware.h"
#include "Tests.h"
#include "Rng.h"
#include <cstring>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

void executetMenu(char Test)
{
	switch (Test)
	{
	case 't':
	case 'T':
		TestLed();
		LedMenu();
		break;
	case 'r':
	case 'R':
		SoftwareResetTest();
		break;
	case 'f':
	case 'F':
		PutCpuToSleep();
		break;
	case 's':
	case 'S':
		TestSpiffs();
		break;
	case 'v':
	case 'V':
		TestSdCard();
		break;
	case 'w':
	case 'W':
		WifiMenu();
		break;
	case 'd':
	case 'D':
		TestTransmitter();
		break;
	case 'l':
	case 'L':
		LearnCode(false);
		break;
	case 'g':
	case 'G':
		LearnCode(true);
		break;
	case 'i':
	case 'I':
		TestI2cDisplay();
		break;
	case 'b':
	case 'B':
		ReadButtonAndAnalog();
		break;
	default:
		break;
	}

	printf("\n");
	printf("Main menu:\n");
	printf("----------\n");
	printf("[T] - Led Menu \n");
	printf("[R] - Software Reset Test\n");
	printf("[F] - Deep Sleep for 5 Seconds.\n");
	printf("[S] - Test SPIFFS\n");
	printf("[V] - Test SD Card\n");
	printf("[W] - WiFi Menu\n");
	printf("[B] - Input Menu\n");
	printf("[D] - Test controller transmitter\n");
	printf("[L] - Learn RF remote control code\n");
	printf("[G] - Learn infrared remote control code\n");
	printf("[I] - Test I2C display\n");
}

extern "C" void app_main(void)
{
	Hal::Hardware::Instance();
 	printf("Hardware Tester for ESP32\n");

	TestClass testClass;
	char test = 0;

	while (1)
	{
		executetMenu(test);
		test = ReadKey();
	}
}
