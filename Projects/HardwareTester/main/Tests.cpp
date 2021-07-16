
#include "Tests.h"
#include "ColorConverter.h"
#include "LearnerCode.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "HalCommon.h"
#include "CodeReceiver.h"

Hal::CodeReceiver* learnerTest = nullptr;

using Hal::Dwt;
using Hal::Hardware;
using Hal::TimeLimit;

using namespace std;

void rgb2hsv(const unsigned char &src_r, const unsigned char &src_g, const unsigned char &src_b, unsigned char &dst_h, unsigned char &dst_s, unsigned char &dst_v)
{
    float r = src_r / 255.0f;
    float g = src_g / 255.0f;
    float b = src_b / 255.0f;

    float h = 0;
	float s = 0; 
	float v = 0; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

    float max = std::max(r , std::max(g, b));
    float min = std::max(r , std::max(g, b));

    v = max;

    if (max == 0.0f) {
        s = 0;
        h = 0;
    }
    else if (max - min == 0.0f) {
        s = 0;
        h = 0;
    }
    else {
        s = (max - min) / max;

        if (max == r) {
            h = 60 * ((g - b) / (max - min)) + 0;
        }
        else if (max == g) {
            h = 60 * ((b - r) / (max - min)) + 120;
        }
        else {
            h = 60 * ((r - g) / (max - min)) + 240;
        }
    }

    if (h < 0) h += 360.0f;

    dst_h = (unsigned char)(h / 2);   // dst_h : 0-180
    dst_s = (unsigned char)(s * 255); // dst_s : 0-255
    dst_v = (unsigned char)(v * 255); // dst_v : 0-255
}

void hsv2rgb(const uint16_t &src_h, const unsigned char &src_s, const unsigned char &src_v, unsigned char &dst_r, unsigned char &dst_g, unsigned char &dst_b)
{
    float h = src_h; // 0-360
    float s = src_s / 255.0f; // 0.0-1.0
    float v = src_v / 255.0f; // 0.0-1.0

    float r = 0;
	float g = 0;
	float b = 0; // 0.0-1.0

    int   hi = (int)(h / 60.0f) % 6;
    float f  = (h / 60.0f) - hi;
    float p  = v * (1.0f - s);
    float q  = v * (1.0f - s * f);
    float t  = v * (1.0f - s * (1.0f - f));

    switch(hi) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    dst_r = static_cast<unsigned char>(r * 255); // dst_r : 0-255
    dst_g = (unsigned char)(g * 255); // dst_r : 0-255
    dst_b = (unsigned char)(b * 255); // dst_r : 0-255
}


void TestSdCard()
{
	if (Hardware::Instance()->GetSdCard().IsMounted() == false)
	{
		if (Hardware::Instance()->GetSdCard().Mount() == false)
		{
			printf("\n\nSD Card Failed failed!\n\n");
			return;
		}
	}

	printf("\n\nOpening file\n");
	FILE *f = fopen("/sdcard/hello.txt", "w");
	if (f == NULL)
	{
		printf("Failed to open file for writing");
		return;
	}
	fprintf(f, "SD Card is Working!!! Hooray!!! :D\n");
	fclose(f);
	printf("File written\n");

	// Check if destination file exists before renaming
	struct stat st;
	if (stat("/sdcard/foo.txt", &st) == 0)
	{
		// Delete it if it exists
		unlink("/sdcard/foo.txt");
	}

	// Rename original file
	printf("Renaming file\n");
	if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0)
	{
		printf("Rename failed\n");
		return;
	}
}


void TestSpiffs()
{
	if (Hardware::Instance()->GetSpiffs().IsMounted() == false)
	{
		if (Hardware::Instance()->GetSpiffs().Mount() == false)
		{
			printf("\n\nSPIFFS failed!\n\n");
			return;
		}
	}

	printf("\n\nOpening file\n");
	FILE *f = fopen("/spiffs/hello.txt", "w");
	if (f == NULL)
	{
		printf("Failed to open file for writing");
		return;
	}
	fprintf(f, "SPIFFS is Working!!! Hooray!!! :D\n");
	fclose(f);
	printf("File written\n");

	// Check if destination file exists before renaming
	struct stat st;
	if (stat("/spiffs/foo.txt", &st) == 0)
	{
		// Delete it if it exists
		unlink("/spiffs/foo.txt");
	}

	// Rename original file
	printf("Renaming file\n");
	if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0)
	{
		printf("Rename failed\n");
		return;
	}
}

void PutCpuToSleep()
{
	printf("\n\nI'm going to bed and I will be back in 5 seconds. BYE :)\n\n");

	Hardware::Instance()->DeepSleep(5 * 1000 * 1000);
}

void SoftwareResetTest()
{
	Hardware::Instance()->SoftwareReset();
}

char ReadKey()
{
	char key = 0;
	while (key == 0)
	{
		scanf("%c", &key);
		vTaskDelay(5);
	}
	return key;
}

void ReadString(char *string, uint8_t size)
{
	uint8_t i = 0;
	char key = 0;
	while (true)
	{
		vTaskDelay(1);
		scanf("%c", &key);
		if (key == 10) // [Enter]
		{
			string[i] = '\0';
			break;
		}
		else if (key == 8) // [Backspace]
		{
			printf("%c %c", 8, 8); // clean the previous char
			i--;
			key = 0;
		}
		else if (key != 0)
		{
			string[i] = key;
			printf("%c", key);
			i++;
			key = 0;
			if (i == size - 1) // if the last key has reached the end of the buffer
			{
				string[i + 1] = '\0';
				break;
			}
		}
	}
	printf("\n");
}

void WifiMenu()
{
	char test = 0;

	while (1)
	{
		switch (test)
		{
		case 's':
		case 'S':
		{
			char mode = 0;
			printf("\n\nSet the Wifi mode:\n\n");
			printf("[1] - Client\n");
			printf("[2] - Hotspot\n");
			printf("[3] - Mesh Network\n");

			mode = ReadKey();
			if (mode == '1')
			{
				Hardware::Instance()->GetWifi().SetMode(Hal::WifiModeConfiguration::Client);
				printf("Configured as Wifi Client.\n");
			}
			else if (mode == '2')
			{
				Hardware::Instance()->GetWifi().SetMode(Hal::WifiModeConfiguration::HotSpot);
				printf("Configured as HotSpot.\n");
			}
			else if (mode == '3')
			{
				Hardware::Instance()->GetWifi().SetMode(Hal::WifiModeConfiguration::Mesh);
				printf("Configured as Mesh network (not supported yet).\n");
			}
			else
				printf("Invalid option.\n");
		}
		break;
		case 'i':
		case 'I':
		{
			char ssid[Hal::WifiSsidMaxLength] = {};
			test = 0;
			printf("Enter the SSID:\n");
			ReadString(ssid, Hal::WifiSsidMaxLength);
			printf("\nSSID set to \"%s\"\n", ssid);
			scanf("%s", ssid);
			Hardware::Instance()->GetWifi().SetSsid(ssid, strlen(ssid));
		}
		break;
		case 'p':
		case 'P':
		{
			char passwd[Hal::WifiPasswordMaxLength] = {};
			printf("Enter the Password:\n");
			ReadString(passwd, Hal::WifiPasswordMaxLength);
			printf("\nPassword set to \"%s\"\n", passwd);
			Hardware::Instance()->GetWifi().SetPassword(passwd, strlen(passwd));
		}
		break;
		case 'c':
		case 'C':
		{
			char channel = 0;
			printf("Enter the channel: \n");
			channel = ReadKey();
			channel -= 48;
			printf("%d", channel);
			Hardware::Instance()->GetWifi().SetChannel(channel);
		}
		break;
		case 't':
		case 'T':
		{
			Hardware::Instance()->GetWifi().Enable();
		}
		break;
		case 'z':
		case 'Z':
		{
			Hardware::Instance()->GetWifi().Disable();
		}
		break;
		case 'x':
		case 'X':
		{
			return;
		}
		break;
		default:
			break;
		}

		printf("\n");
		printf("Wifi menu:\n");
		printf("----------\n");
		printf("[S] - Set WiFi Mode (AP, STA or Mesh)\n");
		printf("[I] - Set WiFi SSID\n");
		printf("[P] - Set WiFi Password\n");
		printf("[C] - Set WiFi Channel\n");
		printf("[T] - Start Wifi\n");
		printf("[Z] - Stop Wifi\n");
		printf("[X] - Return\n");

		test = ReadKey();
	}
}

void LedMenu()
{
	char test = 0;

	while (1)
	{
		switch (test)
		{
		case 'q':
		case 'Q':
		{
			Hardware::Instance()->GetLeds().SetIndicatorLed(Hal::Leds::LedIndex::Green);
		}
		break;
		case 'w':
		case 'W':
		{
			Hardware::Instance()->GetLeds().SetIndicatorLed(Hal::Leds::LedIndex::Yellow);
		}
		case 'e':
		case 'E':
		{
			Hardware::Instance()->GetLeds().SetIndicatorLed(Hal::Leds::LedIndex::Orange);
		}
		break;
		case 'r':
		case 'R':
		{
			Hardware::Instance()->GetLeds().SetIndicatorLed(Hal::Leds::LedIndex::Red);
		}
		case 'a':
		case 'A':
		{
			Hardware::Instance()->GetLeds().ResetIndicatorLed(Hal::Leds::LedIndex::Green);
		}
		break;
		case 's':
		case 'S':
		{
			Hardware::Instance()->GetLeds().ResetIndicatorLed(Hal::Leds::LedIndex::Yellow);
		}
		case 'd':
		case 'D':
		{
			Hardware::Instance()->GetLeds().ResetIndicatorLed(Hal::Leds::LedIndex::Orange);
		}
		break;
		case 'f':
		case 'F':
		{
			Hardware::Instance()->GetLeds().ResetIndicatorLed(Hal::Leds::LedIndex::Red);
		}
		case 't':
		case 'T':
		{
			TestLed();
		}
		break;
		case 'x':
		case 'X':
		{
			return;
		}
		break;
		default:
			break;
		}

		printf("\n");
		printf("Led menu:\n");
		printf("----------\n");
		printf("[Q] - Turn Green On\n");
		printf("[A] - Turn Green Off\n");
		printf("[W] - Turn Yellow On\n");
		printf("[S] - Turn Yellow Off\n");
		printf("[E] - Turn Orange On\n");
		printf("[D] - Turn Orange Off\n");
		printf("[R] - Turn Red On\n");
		printf("[F] - Turn Red Off\n");
		printf("[T] - Test Leds Array\n");
		printf("[X] - Return\n");
		printf("Obs.: gpio 35, 34, 39, 36 can only be input\n");

		test = ReadKey();
	}
}

void AdafruitLibDisplayTest ()
{
	Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
	display.display();
	delay(2000); // Pause for 2 seconds

	// Clear the buffer
	display.clearDisplay();

	// Draw a single pixel in white
	display.drawPixel(10, 10, WHITE);

	// Show the display buffer on the screen. You MUST call display() after
	// drawing commands to make them visible on screen!
	display.display();
	delay(2000);
	// display.display() is NOT necessary after every single drawing command,
	// unless that's what you want...rather, you can batch up a bunch of
	// drawing operations and then update the screen all at once by calling
	// display.display(). These examples demonstrate both approaches...

	testdrawline();      // Draw many lines

	testdrawrect();      // Draw rectangles (outlines)

	testfillrect();      // Draw rectangles (filled)

	testdrawcircle();    // Draw circles (outlines)

	testfillcircle();    // Draw circles (filled)

	testdrawroundrect(); // Draw rounded rectangles (outlines)

	testfillroundrect(); // Draw rounded rectangles (filled)

	testdrawtriangle();  // Draw triangles (outlines)

	testfilltriangle();  // Draw triangles (filled)

	testdrawchar();      // Draw characters of the default font

	testdrawstyles();    // Draw 'stylized' characters

	testscrolltext();    // Draw scrolling text

	testdrawbitmap();    // Draw a small bitmap image

	// Invert and restore display, pausing in-between
	display.invertDisplay(true);
	delay(1000);
	display.invertDisplay(false);
	delay(1000);

	testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void TestI2cDisplay()
{
	char test = 0;
	while (1)
	{
		switch (test)
		{
			case 'a':
			case 'A':
			{
				AdafruitLibDisplayTest();
			}
			break;
			case 'c':
			case 'C':
			{
				uint8_t countdownCounter = 5;
				TimeLimit timeTlimit = {};
				for(uint8_t counter = countdownCounter; counter > countdownCounter; counter--)
				{
					uint16_t color = Hardware::Instance()->GetRng().GetNumber() % 361;
					Hal::LedHsv hsv = {color, Utilities::ColorConverter::MaxSaturation, Utilities::ColorConverter::MaxValue};
					Hal::Led led = {};

					while (timeTlimit.IsTimeUp(1000) == false)
					{
						for (uint16_t i = 0; i < 256; i++)
							{
								hsv.Color.Value = i;
								hsv.Color.Saturation = Utilities::ColorConverter::MaxSaturation;
								Utilities::ColorConverter::HsvToRgb(hsv, led);
								for(uint16_t ledIndex = 0; ledIndex < Hal::MaxAddressableLeds; ledIndex++)
									Hardware::Instance()->GetLeds().SetLedColor(ledIndex, led);
								Hardware::Instance()->GetLeds().Refresh();
								if (i != 0 && i % 26)
									vTaskDelay((50 / i));
							}
					}

					timeTlimit.Reset();
				}
			}
			break;
			case 'x':
			case 'X':
			{
				return;
			}
			break;
			default:
				break;
		}

		printf("\n");
		printf("I2C Display Tests:\n");
		printf("----------\n");
		printf("[L] - Adafruit Library Tests\n");
		printf("[S] - Countdown Test\n");
		printf("[X] - Return\n");

		test = ReadKey();
	}	
}

void IoExtenderMenu()
{
	char test = 0;

	while (1)
	{
		switch (test)
		{
			case 's':
			case 'S':
			{
				printf("\nAll GPIO on the IO Extender are turned on.\n");
				
				Hardware::Instance()->GetIoExtender().ConfigureOutput(0xFF);

				Hardware::Instance()->GetIoExtender().SetAll();				
			}
			break;
			case 'd':
			case 'D':
			{
				printf("\nAll GPIO on the IO Extender are turned off.\n");
				Hardware::Instance()->GetIoExtender().ConfigureOutput(0xFF);
				Hardware::Instance()->GetIoExtender().ResetAll();

			}
			break;
			case 'f':
			case 'F':
			{
				Hardware::Instance()->GetIoExtender().ConfigureInput(0xFF);
				printf("\nThe Inputs value:%x\n", Hardware::Instance()->GetIoExtender().GetInputs());
				
			}
			break;
			case 'x':
			case 'X':
			{
				return;
			}
			break;
			default:
				break;
			}

		printf("\n");
		printf("IO Extender menu:\n");
		printf("----------\n");
		printf("[S] - Turn on all outputs\n");
		printf("[D] - Turn off all outputs\n");
		printf("[F] - Read Inputs\n");
		printf("[X] - Return\n");

		test = ReadKey();
	}
}


void ReadButtonAndAnalog()
{
	char test = 0;
	while (1)
	{
		switch (test)
		{
			case 'l':
			case 'L':
			{
				Hal::DeviceInput* device = &Hal::Hardware::Instance()->GetDeviceInput();
				bool enterButton = device->GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonEnter);
				bool returnButton = device->GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonReturn);
				printf("Enter Button:\t%s\n", (!enterButton) ? "Pressed" : "Not Pressed");
				printf("Return Button:\t%s\n", (!returnButton) ? "Pressed" : "Not Pressed");
			}
			break;
			case 's':
			case 'S':
			{
				Hal::DeviceInput* device = &Hal::Hardware::Instance()->GetDeviceInput();
				uint32_t brightness = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Brightness);
				uint32_t color = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Color);
				printf("Color Value:\t%d\n", color);
				printf("brightness Value:\t%d\n", brightness);
			}
			break;
			case 'p':
			case 'P':
			{
				Hal::DeviceInput* device = &Hal::Hardware::Instance()->GetDeviceInput();
				bool enterButton = false;
				bool returnButton = false;
				uint32_t brightness = 0;
				uint32_t color = 0;
				while(true)
				{
					enterButton = device->GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonEnter);
					returnButton = device->GetDigitalInput(Hal::DeviceInput::DigitalInputIndex::UserButtonReturn);
					printf("Enter Button:\t%s\n", (!enterButton) ? "Pressed" : "Not Pressed");
					printf("Return Button:\t%s\n", (!returnButton) ? "Pressed" : "Not Pressed");
					brightness = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Brightness);
					color = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Color);
					printf("Color Value:\t%d\n", color);
					printf("brightness Value:\t%d\n", brightness);
					vTaskDelay(500);
				}
			}
			break;
			case 'r':
			case 'R':
			{
				Hal::Led led = {};
				led.Color.Red = 0xff /8;
				led.Color.Blue = 0xff /3;
				led.Color.Green = 0xff / 8;
				Hardware::Instance()->GetLeds().SetLedsCount(256);
				Hal::DeviceInput* device = &Hal::Hardware::Instance()->GetDeviceInput();
				for(;;)
				{
					uint16_t color = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Color, 100) / 12;
					uint8_t brightness = device->GetAnalogInput(Hal::DeviceInput::AnalogInputIndex::Brightness, 100) / 16;
					Hal::LedHsv hsv = {color, 255, brightness};
					Utilities::ColorConverter::HsvToRgb(hsv, led);
					for(uint16_t ledIndex = 0; ledIndex < 256; ledIndex++)
							Hardware::Instance()->GetLeds().SetLedColor(ledIndex, led);
					Hardware::Instance()->GetLeds().Refresh();
					vTaskDelay(100);
				}
			}
			break;
			case 'x':
			case 'X':
			{
				return;
			}
			break;
			default:
				break;
		}

		printf("\n");
		printf("Code Learner menu:\n");
		printf("----------\n");
		printf("[L] - Read Analog\n");
		printf("[S] - Read Buttons\n");
		printf("[P] - Print Analogs and Buttons in a loop\n");
		printf("[R] - Run Leds with Analogs\n");
		printf("[X] - Return\n");

		test = ReadKey();
	}	
}

void LearnCode(bool infrared)
{
	char test = 0;
	if (learnerTest == nullptr)
	{
		learnerTest = &Hal::Hardware::Instance()->GetCodeReceiver();
	}

	if (infrared)
		learnerTest->Configure(Hal::Gpio::GpioIndex::Gpio4);
	else
		learnerTest->Configure(Hal::Gpio::GpioIndex::Gpio14);

	learnerTest->Configure(infrared);

	while (1)
	{
		switch (test)
		{
			case 'l':
			case 'L':
			{
				learnerTest->Stop();
				Hardware::Instance()->GetTimer0().SetTimer(16000);
				learnerTest->Start();
			}
			break;
			case 's':
			case 'S':
			{
				learnerTest->Stop();
				Hardware::Instance()->GetTimer0().Stop();
			}
			break;
			case 'p':
			case 'P':
			{
				learnerTest->PrintResult();
			}
			break;
			case 'k':
			case 'K':
			{
				printf("Printing code:\n");
				while(true)
				{
					if (learnerTest->CodeReceived())
					{
						learnerTest->PrintResult();
						learnerTest->Stop();
						learnerTest->Start();
					}
					vTaskDelay(1000);
				}
			}
			break;
			case 'x':
			case 'X':
			{
				return;
			}
			break;
			default:
				break;
		}

		printf("\n");
		printf("Code Learner menu:\n");
		printf("----------\n");
		printf("[L] - Start Code Learner\n");
		printf("[S] - Stop Code Learner\n");
		printf("[P] - Print Code\n");
		printf("[K] - Keep Printing Code\n");
		printf("[X] - Return\n");

		test = ReadKey();
	}	
}

void TestLed()
{
	Hal::Led led = {};
	led.Color.Red = 0xff /8;
	led.Color.Blue = 0xff /3;
	led.Color.Green = 0xff / 8;
	Hardware::Instance()->GetLeds().SetLedsCount(256);
	
	for(;;)
	{
		uint16_t color = Hardware::Instance()->GetRng().GetNumber() % 361;
		Hal::LedHsv hsv = {color, 255, 255};
		 
		for (uint16_t i = 0; i < 256; i++)
		{
			hsv.Color.Value = i;
			hsv.Color.Saturation = 255;
			Utilities::ColorConverter::HsvToRgb(hsv, led);
			for(uint16_t ledIndex = 0; ledIndex < 256; ledIndex++)
				Hardware::Instance()->GetLeds().SetLedColor(ledIndex, led);
			Hardware::Instance()->GetLeds().Refresh();
			if (i != 0 && i % 26)
				vTaskDelay((50 / i));
			//printf("i = %d -> R=%d, G=%d, B=%d\n", i, led.Color.Red, led.Color.Blue, led.Color.Blue);
		}
		// printf("-----------------------\n");
		for (uint16_t i = 255; i != 0; i--)
		{
			hsv.Color.Value = i;
			hsv.Color.Saturation = 255;
			Utilities::ColorConverter::HsvToRgb(hsv, led);
			for(uint16_t ledIndex = 0; ledIndex < 256; ledIndex++)
				Hardware::Instance()->GetLeds().SetLedColor(ledIndex, led);
			Hardware::Instance()->GetLeds().Refresh();
			if (i % 26)
				vTaskDelay((50 / i));
			//printf("i = %d -> R=%d, G=%d, B=%d\n", i, led.Color.Red, led.Color.Blue, led.Color.Blue);
		}

	}
}

void RainbowLedTest()
{
	Hal::Led led = {};
	led.Color.Red = 0xff /8;
	led.Color.Blue = 0xff /3;
	led.Color.Green = 0xff / 8;
	Hardware::Instance()->GetLeds().SetLedsCount(256);
	
	for(;;)
	{
		for(uint16_t color = 0; color <= 360; color++)
		 {
			for(uint16_t ledIndex = 0; ledIndex < 256; ledIndex++)
			{
				uint16_t a = (color + ledIndex) % 360;
				Hal::LedHsv hsv = {a, 255, 255};
				hsv.Color.Value = 64;
				hsv.Color.Saturation = 255;
				Utilities::ColorConverter::HsvToRgb(hsv, led);
				Hardware::Instance()->GetLeds().SetLedColor(ledIndex, led);
			}
			Hardware::Instance()->GetLeds().Refresh();
			// vTaskDelay(1);
			//printf("i = %d -> R=%d, G=%d, B=%d\n", i, led.Color.Red, led.Color.Blue, led.Color.Blue);
		}
		 
	}
}

void TestTransmitter()
{
	//for(uint8_t i = 0; i < 10; i++)
		Hardware::Instance()->GetRfControl().RunCommand(0);
}

