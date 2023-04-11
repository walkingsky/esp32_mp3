#include "gpio_app.h"

bool LED_status = false;
unsigned long LastTime1 = 0;
unsigned long LastTime2 = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);

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
    if (millis() - LastTime1 > 1000) // 延时1秒
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

/*初始化 dht11 温湿度传感器*/
void dht_init()
{
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
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
    dht.humidity().getSensor(&sensor);
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
}

/*循环获取温湿度*/
void dht_loop()
{
    if (millis() - LastTime2 < 20000) // 延时20秒
        return;
    LastTime2 = millis();
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
    }
    else
    {
        Serial.print(F("Humidity: "));
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
    }
}

uint8_t key_loop()
{
    uint16_t an_result = 0;
    uint32_t volt = 0;
    an_result = analogRead(ADC_KEY);
    volt = analogReadMilliVolts(ADC_KEY);

    delay(1);
    if (an_result < 10) // key1
    {
        Serial.printf("KEY1 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        return 1;
    }
    else if (650 <= an_result && an_result <= 750) // key2
    {
        Serial.printf("KEY2 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        return 2;
    }
    else if (1400 <= an_result && an_result <= 1500) // key3
    {
        Serial.printf("KEY3 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        return 3;
    }
    else if (2180 <= an_result && an_result <= 2280) // key4
    {
        Serial.printf("KEY4 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        return 4;
    }
    else if (3100 <= an_result && an_result <= 3200) // key5
    {
        Serial.printf("KEY5 pressed ADC_KEY VALUE:%d %dmv\n\r", an_result, volt);
        return 5;
    }
    else
        return 0;
}
