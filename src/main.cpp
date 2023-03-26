// #include <Arduino.h>
#include "main.h"

void setup()
{

  Serial.begin(115200);

  EEPROM.begin(512); // 读取eeprom配置
  readEEpromConf();

  connect_wifi(); // 联网处理

  setUpOverTheAirProgramming(); // 开启OTA升级服务

  ntpBegin(); // 连接时间服务器

  webServiceBegin(); // http 服务
}

void loop()
{
  // put your main code here, to run repeatedly:
  audio.loop();
  ArduinoOTA.handle(); // OTA升级
  server.handleClient();
}