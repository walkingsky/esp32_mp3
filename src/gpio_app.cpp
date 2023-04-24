#include "gpio_app.h"
#include "menu.h"

bool LED_status = false;
uint8_t old_key = 0;

#ifdef _COMPONENT_DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
#endif
#ifdef _COMPONENT_LED
/* 初始化配置LED的IO口 */
void led_init()
{
    pinMode(IO2_LED, OUTPUT);
    digitalWrite(IO2_LED, LOW);
    LED_status = false;
}

/*循环点亮LED*/
void led_loop()
{
    if (millis() - LastTime1 > LED_DELAY) // 延时1秒
    {
        LastTime1 = millis();
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
#endif
#ifdef _COMPONENT_DHT11
/*初始化 dht11 温湿度传感器*/
void dht_init()
{
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
}

/*循环获取温湿度*/
struct DHT_result dht_loop()
{
    struct DHT_result dth_result;
    /* 取消全局变量控制获取数据频率
    if (millis() - LastTime2 < TEMPERATURE_DELAY) // 延时20秒
        return;
    LastTime2 = millis();
    */
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        // Serial.println(F("Error reading temperature!"));
        dth_result.temperature = -100;
    }
    else
    {
        // Serial.print(F("Temperature: "));
        // Serial.print(event.temperature);
        // Serial.println(F("°C"));
        dth_result.temperature = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        // Serial.println(F("Error reading humidity!"));
        dth_result.humidity = -1;
    }
    else
    {
        // Serial.print(F("Humidity: "));
        // Serial.print(event.relative_humidity);
        // Serial.println(F("%"));
        dth_result.humidity = event.relative_humidity;
    }
    return dth_result;
}
#endif
void key_loop()
{
    uint16_t an_result = 0;
    uint8_t key = 0;
    // uint32_t volt = 0;
    an_result = analogRead(ADC_KEY);
    // volt = analogReadMilliVolts(ADC_KEY);

    if (an_result < 300) // key1
    {
        // Serial.printf("KEY1 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_UP;
    }
    else if (300 <= an_result && an_result <= 1000) // key2
    {
        // Serial.printf("KEY2 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_RIGHT;
    }
    else if (1001 <= an_result && an_result <= 1900) // key3
    {
        // Serial.printf("KEY3 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_LEFT;
    }
    else if (2000 <= an_result && an_result <= 2800) // key4
    {
        // Serial.printf("KEY4 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_DOWN;
    }
    else if (2900 <= an_result && an_result <= 3400) // key5
    {
        // Serial.printf("KEY5 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_OK;
    }
    else if (3400 < an_result)
    {
        key = 0;
    }
    else
    {
        Serial.printf("(%d)", an_result);
    }

    if (old_key == 0 && old_key != key) // 按键被按下
    {
        LastTimeKey = millis(); // KEY值改变是开始计时
        old_key = key;
        // Serial.printf("开始计时\r\n");
    }
    else if (key == 0 && old_key != key) // 按键松开
    {
        // Serial.printf("按键松开\r\n");
        if ((millis() - LastTimeKey) >= PRESS_KEY_DELAY && (millis() - LastTimeKey) <= LONGPRESS_KEY_DELAY)
        {
            Serial.printf("pressed ADC_KEY VALUE:%d\n\r", old_key);
            key = old_key;
            old_key = 0;
        }
        else if ((millis() - LastTimeKey) > LONGPRESS_KEY_DELAY)
        {

            Serial.printf("long pressed ADC_KEY VALUE:%d\n\r", old_key + 10);
            key = old_key + 10;
            old_key = 0;
        }
        if (key > 0)
            doMenu(key); // 执行菜单操作
    }
    else // 按键没变
    {
        // return 0; // 没有按键
    }
}
