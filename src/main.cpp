// #include <Arduino.h>
#include "main.h"

extern EepromConf eepromConf;
extern WiFiUDP Udp;
extern int localPort;
extern WebServer server;
extern Audio audio;

void setup()
{

  Serial.begin(115200);
  readEEpromConf();
  Serial.printf("eepromConfig wifi ssid:%s password:%s \n\r", eepromConf.wifi_ssid, eepromConf.wifi_password);

  connect_wifi(); // 联网处理

  setUpOverTheAirProgramming(); // 开启OTA升级服务

  // ntpBegin(); // 连接时间服务器
  Serial.println("Starting UDP"); // 连接时间服务器
  Udp.begin(localPort);
  // Serial.print("Local port: ");
  // Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  pinMode(ADC_KEY, ANALOG);

  webServiceBegin(); // http 服务
  led_init();
  dht_init();
  wm8978Init();
  // sdcard_init();
  oled_init();
  // a2dp_setup();
}

void loop()
{
  // put your main code here, to run repeatedly:
  audio.loop();
  ArduinoOTA.handle(); // OTA升级
  server.handleClient();
  led_loop();
  dht_loop();
  key_loop();
  oled_loop();
  oled_fps_loop();
  // delay(10);
}