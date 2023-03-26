#ifndef __WIFI_H__
#define __WIFI_H__

#include <WiFi.h>
#include "eeprom/eeprom.h"

// extern struct EepromConf eepromConf;
static bool wifiConnected = false; // wifi 是否连接上的标记
static uint8_t loadNum = 6;
// extern uint8_t wifi_connect_cnt;
static uint8_t wifi_connect_cnt = 120; // wifi连接重试次数。配合500毫秒的间隔，实际时间为120*0.5s = 60秒钟
static uint8_t SmartConfigStatus = 0;  // 是否在smartconfig自动配网状态，0，未开始，1，等待配置 2，收到配置，连接wifi，3 连接wifi配置错误 ，4。wifi正常连接

void connect_wifi(); // wifi联网
bool smart_config(); // 自动配网
#endif