#include "gpio_app.h"

bool LED_status = false;
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
    /*
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    */
    dht.humidity().getSensor(&sensor);
    /*
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    */
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
uint8_t key_loop(uint8_t *old_key)
{
    uint16_t an_result = 0;
    uint8_t key = 0;
    bool key_change = false;
    // uint32_t volt = 0;
    an_result = analogRead(ADC_KEY);
    // volt = analogReadMilliVolts(ADC_KEY);

    if (an_result < 10) // key1
    {
        // Serial.printf("KEY1 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_UP;
    }
    else if (650 <= an_result && an_result <= 750) // key2
    {
        // Serial.printf("KEY2 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_RIGHT;
    }
    else if (1400 <= an_result && an_result <= 1500) // key3
    {
        // Serial.printf("KEY3 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_LEFT;
    }
    else if (2180 <= an_result && an_result <= 2280) // key4
    {
        // Serial.printf("KEY4 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_DOWN;
    }
    else if (3050 <= an_result && an_result <= 3250) // key5
    {
        // Serial.printf("KEY5 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        key = KEY_OK;
    }
    else
        key = 0;

    if (*old_key == 0 && *old_key != key)
    {
        LastTimeKey = millis(); // KEY值改变是开始计时
        *old_key = key;
        key_change = true;
        // Serial.printf("开始计时\r\n");
    }
    else if (*old_key != key)
    {
        key_change = true;
        // Serial.printf("按键松开\r\n");
    }

    if (key_change && (millis() - LastTimeKey) >= PRESS_KEY_DELAY && (millis() - LastTimeKey) <= LONGPRESS_KEY_DELAY)
    {
        Serial.printf("pressed ADC_KEY VALUE:%d\n\r", *old_key);
        key = *old_key;
        *old_key = 0;
    }
    else if (key_change && (millis() - LastTimeKey) > LONGPRESS_KEY_DELAY)
    {

        Serial.printf("long pressed ADC_KEY VALUE:%d\n\r", *old_key + 10);
        key = *old_key + 10;
        *old_key = 0;
    }
    else if (!key_change && (millis() - LastTimeKey) > LONGPRESS_KEY_DELAY)
    {
        LastTimeKey = millis();
        return *old_key + 10;
    }
    else if (!key_change) // 按键一直没有松开仍按原状态返回
    {
        return 0;
    }
    else if (key_change) // 小于触发时间的
    {
        return 0; // 没有按键
    }
    return key;
}
