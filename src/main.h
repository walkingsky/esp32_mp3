#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <WiFiUdp.h>

#include "components_define.h"

#include "wifi/wifi.h"
#include "eeprom/eeprom.h"
#ifdef _COMPONENT_NTP
#include "netservice/ntp.h"
#endif
#ifdef _COMPONENT_HTTPSERVER
#include "netservice/webserver.h"
#endif
#ifdef _COMPONENT_WM8978
#include "audio/wm8978card.h"
#endif
#ifdef _COMPONENT_WM8978_AUDIO
#ifndef _COMPONENT_WM8978
#include "audio/wm8978card.h"
#endif
#endif
#include "gpio_app.h"
// #include "audio/a2dp_audio.h"
#ifdef _COMPONENT_SDCARD
#include "sdcard/sdcard.h"
#endif
#include "oled/oled.h"
#ifdef _COMPONENT_BLUETOOTH
#include "bluetooth/btAudio.h"
#endif

#include "menu.h"

#endif