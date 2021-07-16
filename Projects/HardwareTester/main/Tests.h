
#ifndef TESTS_H_
#define TESTS_H_
#include <cstring>
#include <string>
#include "stdio.h"
#include <cstdint>
#include <cstdarg>
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"
#include "LearnerCode.h"

using Hal::Timer;
using Hal::Hardware;

static void get_string(char *line, size_t size)
{
    int count = 0;
    while (count < size) {
        int c = fgetc(stdin);
        if (c == '\n') {
            line[count] = '\0';
            break;
        } else if (c > 0 && c < 127) {
            line[count] = c;
            ++count;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void SoftwareResetTest();
void PutCpuToSleep();
void TestSpiffs();
void WifiMenu();
void LedMenu();
char ReadKey();
void ReadString(char * string, uint8_t size);
void TestLed();
void TestI2sClock();
void TestSdCard();
void IoExtenderMenu();
void TestTransmitter();
void LearnCode(bool infrared = false);
void ReadButtonAndAnalog();
void RainbowLedTest();


class TestClass : Timer::Callback
{
public:
    TestClass()
    {
        //Hal::Hardware::Instance()->GetTimer0().AddCallback(this);
        Hardware::Instance()->GetGpio().ConfigOutput(Hal::Gpio::GpioIndex::Gpio26,
            Hal::Gpio::OutputType::PullUp);
    }
    void TimerCallback() override
    {
       Hardware::Instance()->GetGpio().Set(Hal::Gpio::GpioIndex::Gpio26);
       Hal::Dwt::DelayMicrosecond(1);
       Hardware::Instance()->GetGpio().Reset(Hal::Gpio::GpioIndex::Gpio26);
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Test
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

static void testdrawline() {
  int16_t i;
Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

static void testdrawrect(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

static void testfillrect(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

static void testdrawcircle(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i < std::max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

static void testfillcircle(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=std::max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

static void testdrawroundrect(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

static void testfillroundrect(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

static void testdrawtriangle(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=0; i<std::max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

static void testfilltriangle(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  for(int16_t i=std::max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

static void testdrawchar(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

static void testdrawstyles(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
//   display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
//   display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
//   display.print(F("0x"));
//   display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

static void testscrolltext(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
//   display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

static void testdrawbitmap(void) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

static void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
    Adafruit_SSD1306& display = Hal::Hardware::Instance()->GetDisplay();
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = Hal::Hardware::Instance()->GetRandomNumberRange(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = Hal::Hardware::Instance()->GetRandomNumberRange(1, 6);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = Hal::Hardware::Instance()->GetRandomNumberRange(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] =  Hal::Hardware::Instance()->GetRandomNumberRange(1, 6);
      }
    }
  }
}


void TestI2cDisplay();


#endif /* TESTS_H_ */
