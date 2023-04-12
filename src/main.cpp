// #include <Arduino.h>
#include "main.h"

extern EepromConf eepromConf;
extern WiFiUDP Udp;
extern int localPort;
extern WebServer server;
extern Audio audio;

extern struct dirList *fileList; // 目录链表
extern struct dirList *selected_file;

int8_t menu_key = 0;
bool sdcard_inited = false;
bool wm8978_inited = false;

unsigned long LastTime1;
unsigned long LastTime2;
unsigned long LastTimeKey;

uint8_t key_value; // 按键值
uint8_t old_key_value;

void setup()
{
  LastTime1 = 0;
  LastTime2 = 0;
  LastTimeKey = 0;
  key_value = 0;
  old_key_value = 0;
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
  sdcard_inited = sdcard_init();
  wm8978Init();
  if (sdcard_inited)
  {
    wm8978_inited = wm8978_sdcard();
    selected_file = fileList;
  }

  oled_init();
  oled_loop();
  file_menu_display();
  // a2dp_setup();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (wm8978_inited)
    audio.loop();
  ArduinoOTA.handle(); // OTA升级
  server.handleClient();
  led_loop();
  dht_loop();
  menu_key = key_loop();
  mainMenu(menu_key);
  delay(10);
}