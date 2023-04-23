#ifndef __LED_H__
#define __LED_H__
#include <Arduino.h>
#include "gpios.h"
#include "components_define.h"
#ifdef _COMPONENT_DHT11
#include <DHT.h>
#include <DHT_U.h>
#endif

#define DHTTYPE DHT11 // DHT 11

#define LED_DELAY 1000           // led操作间隔延时1000毫秒 1秒
#define TEMPERATURE_DELAY 20000  // 获取温湿度数据的间隔 20秒
#define PRESS_KEY_DELAY 200      // 获取按键变化的时间间隔
#define LONGPRESS_KEY_DELAY 2000 // 获取长按按键变化的时间间隔
#define DOUBLE_KEY_DELAY 2000    // 获取双击按键变化的时间间隔

#define KEY_UP 1
#define KEY_RIGHT 2
#define KEY_LEFT 3
#define KEY_DOWN 4
#define KEY_OK 5

#ifdef _COMPONENT_LED
extern unsigned long LastTime1; // LED 的时间间隔
#endif
#ifdef _COMPONENT_DHT11
// extern unsigned long LastTime2; // DHT的时间间隔

struct DHT_result
{
    float humidity = 0;
    float temperature = 0;
};

#endif
extern unsigned long LastTimeKey;

extern uint8_t key_value; // 按键值
extern uint8_t old_key_value;
#ifdef _COMPONENT_LED
void led_init();
void led_loop();
#endif
#ifdef _COMPONENT_DHT11
void dht_init();
struct DHT_result dht_loop();
#endif
uint8_t key_loop();

#endif