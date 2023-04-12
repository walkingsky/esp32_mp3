#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <WiFiUdp.h>

#include "wifi/wifi.h"
#include "eeprom/eeprom.h"
#include "netservice/ntp.h"
#include "netservice/webserver.h"
#include "audio/wm8978.h"
#include "gpio_app.h"
#include "audio/a2dp_audio.h"
#include "sdcard/sdcard.h"
#include "oled/oled.h"

void mainMenu(int key);
void file_menu_display();
#endif