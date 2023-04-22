#ifndef __COMPONENTS_DEFINE_H__
#define __COMPONENTS_DEFINE_H__

#define _COMPONENT_DHT11 // 温湿度传感器
#define _COMPONENT_WIFI
#define _COMPONENT_HTTPSERVER
// #define _COMPONENT_SDCARD_HTTP // http sd卡文件管理
//  #define _COMPONENT_OTA
#define _COMPONENT_NTP
// #define _COMPONENT_BLUETOOTH // 蓝牙声卡
//  #define _COMPONENT_LED
#define _COMPONENT_SDCARD
#define _COMPONENT_WM8978
#define _COMPONENT_WM8978_AUDIO

#ifdef _COMPONENT_WM8978_AUDIO
#ifndef _COMPONENT_WM8978
#define _COMPONENT_WM8978
#endif
#endif

#endif