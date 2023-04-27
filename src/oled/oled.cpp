#include "oled.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define SECONDS 10
uint8_t flip_color = 0;

// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/LED_SCL, /* data=*/LED_SDA, /* reset=*/U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/LED_SCL, /* data=*/LED_SDA);
// U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/LED_SCL, /* data=*/LED_SDA);
void oled_init(void)
{
    pinMode(LED_SDA, OUTPUT);
    digitalWrite(LED_SDA, 0); // default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0
    pinMode(LED_SCL, OUTPUT);
    digitalWrite(LED_SCL, 0);
    u8g2.setBusClock(8000000);
    u8g2.begin();
    u8g2.enableUTF8Print();
}

void oled_logo(void)
{
    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 23, 128, 18, logo);
    u8g2.sendBuffer();
    delay(1000);
}
