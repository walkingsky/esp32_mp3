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
    // u8g2.setFont(u8g2_font_6x12_m_symbols);
}

void drawLogo(void)
{
    u8g2.setFontMode(1); // Transparent
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");

    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14, 8, "8");

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36, 22, "g");
    u8g2.drawStr(48, 22, "\xb2");

    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");

    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21, 8, "8");

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51, 30, "g");
    u8g2.drawStr(67, 30, "\xb2");

    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);

#endif
}

void drawUTF8String(uint8_t x, uint8_t y, String str, bool clear = false)
{
    u8g2.setFont(u8g2_font_unifont_t_chinese2); // use chinese2 for all the glyphs of "你好世界"
    // u8g2.setFontDirection(0);
    if (clear)
        u8g2.clearDisplay();
    u8g2.drawUTF8(x, y, str.c_str());
    u8g2.sendBuffer();
}

void drawURL(void)
{
#ifndef MINI_LOGO
    u8g2.setFont(u8g2_font_4x6_tr);
    if (u8g2.getDisplayHeight() < 59)
    {
        u8g2.drawStr(89, 20, "github.com");
        u8g2.drawStr(73, 29, "/olikraus/u8g2");
    }
    else
    {
        u8g2.drawStr(1, 54, "github.com/olikraus/u8g2");
    }
#endif
}

void oled_loop(void)
{
    u8g2.clearBuffer();
    drawLogo();
    drawURL();
    u8g2.sendBuffer();
    delay(1000);
}
