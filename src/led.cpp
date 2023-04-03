#include "led.h"

bool LED_status = false;
unsigned long LastTime = 0;

void led_init()
{
    pinMode(IO2_LED, OUTPUT);
    digitalWrite(IO2_LED, LOW);
    LED_status = false;
}

void led_loop()
{
    if (millis() - LastTime > 1000)
    { // 10分钟更新一次天气
        LastTime = millis();
        if (LED_status)
        {
            digitalWrite(IO2_LED, LOW);
            LED_status = false;
        }
        else
        {
            digitalWrite(IO2_LED, HIGH);
            LED_status = true;
        }
    }
}