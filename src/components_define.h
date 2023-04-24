#ifndef __COMPONENTS_DEFINE_H__
#define __COMPONENTS_DEFINE_H__

#define _COMPONENT_DHT11 // 温湿度传感器
#define _COMPONENT_WIFI
#define _COMPONENT_HTTPSERVER // http 服务
// #define _COMPONENT_SDCARD_HTTP // http sd卡文件管理
// #define _COMPONENT_OTA         // OTA 升级
#define _COMPONENT_NTP // ntp网络时间协议
// #define _COMPONENT_BLUETOOTH // 蓝牙声卡
// #define _COMPONENT_LED     //板载led等 IO2口
#define _COMPONENT_SDCARD       // TF卡
#define _COMPONENT_WM8978       // wm8978 IIC控制
#define _COMPONENT_WM8978_AUDIO // I2S音频控制

#ifdef _COMPONENT_WM8978_AUDIO
#ifndef _COMPONENT_WM8978
#define _COMPONENT_WM8978
#endif
#endif

#endif