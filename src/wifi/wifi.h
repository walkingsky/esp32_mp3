#ifndef __WIFI_H__
#define __WIFI_H__

#include <WiFi.h>
#include "eeprom/eeprom.h"

void connect_wifi(); // wifi联网
bool smart_config(); // 自动配网
#endif