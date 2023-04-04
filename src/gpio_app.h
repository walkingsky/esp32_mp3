#ifndef __LED_H__
#define __LED_H__
#include <Arduino.h>
#include "gpios.h"
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11 // DHT 11

void led_init();
void led_loop();
void dht_init();
void dht_loop();
#endif