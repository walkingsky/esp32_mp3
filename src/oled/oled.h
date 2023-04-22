#ifndef __OLED_H__
#define __OLED_H__

#include <U8g2lib.h>
// #include <U8x8lib.h>
//   #include <SPI.h>

#include <Wire.h>
#include "gpios.h"

void oled_init();
void oled_loop();
// void drawUTF8String(uint8_t x, uint8_t y, String str, bool clear);
#endif