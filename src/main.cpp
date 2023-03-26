// #include <Arduino.h>
#include "main.h"
/*
extern EepromConf eepromConf;

extern bool wifiConnected; // wifi 是否连接上的标记
extern uint8_t loadNum;    // 开机启动进度条的起始位置

extern WebServer server;
*/
void setUpOverTheAirProgramming() // OAT升级
{

  // Change OTA port.
  // Default: 8266
  // ArduinoOTA.setPort(8266);

  // Change the name of how it is going to
  // show up in Arduino IDE.
  // Default: esp8266-[ChipID]
  ArduinoOTA.setHostname("esp32_mp3");

  // Re-programming passowrd.
  // No password by default.
  // ArduinoOTA.setPassword("123");

  ArduinoOTA.begin();
}

void setup()
{

  Serial.begin(115200);

  EEPROM.begin(512); // 读取eeprom配置
  readEEpromConf();

  connect_wifi(); // 联网处理

  setUpOverTheAirProgramming(); // 开启OTA升级服务

  ntpBegin(); // 连接时间服务器

  webServiceBegin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  audio.loop();
  ArduinoOTA.handle(); // OTA升级
  server.handleClient();
}