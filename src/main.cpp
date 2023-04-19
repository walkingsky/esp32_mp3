// #include <Arduino.h>
#include "main.h"

extern EepromConf eepromConf;
#ifdef _COMPONENT_NTP
extern WiFiUDP Udp;
#endif
extern int localPort;
#ifdef _COMPONENT_HTTPSERVER
extern WebServer server;
#endif
#ifdef _COMPONENT_WM8978_AUDIO
extern Audio audio;
#endif
#ifdef _COMPONENT_SDCARD
extern struct dirList *fileList; // 目录链表
extern struct dirList *selected_file;
#endif

int8_t menu_key = 0;
#ifdef _COMPONENT_SDCARD
bool sdcard_inited = false;
#endif
bool wm8978_inited = false;
#ifdef _COMPONENT_LED
unsigned long LastTime1;
#endif
#ifdef _COMPONENT_DHT11
unsigned long LastTime2;
#endif
unsigned long LastTimeKey;

uint8_t key_value; // 按键值
uint8_t old_key_value;
#ifdef _COMPONENT_BLUETOOTH
// --btaudio
btAudio bt_audio = btAudio("ESP_Speaker");
#endif
void setup()
{
#ifdef _COMPONENT_LED
  LastTime1 = 0;
#endif
#ifdef _COMPONENT_DHT11
  LastTime2 = 0;
#endif
  LastTimeKey = 0;
  key_value = 0;
  old_key_value = 0;
  Serial.begin(115200);
  readEEpromConf();
  Serial.printf("eepromConfig wifi ssid:%s password:%s \n\r", eepromConf.wifi_ssid, eepromConf.wifi_password);

  oled_init();
  oled_loop();

  connect_wifi(); // 联网处理
#ifdef _COMPONENT_OTA
  setUpOverTheAirProgramming(); // 开启OTA升级服务
#endif
#ifdef _COMPONENT_NTP
  ntpBegin();
  // 连接时间服务器
  Serial.println("Starting UDP"); // 连接时间服务器
  Udp.begin(localPort);
  // Serial.print("Local port: ");
  // Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
#endif
  pinMode(ADC_KEY, ANALOG);
#ifdef _COMPONENT_HTTPSERVER
  webServiceBegin(); // http 服务
#endif
#ifdef _COMPONENT_LED
  led_init();
#endif
#ifdef _COMPONENT_DHT11
  dht_init();
#endif
#ifdef _COMPONENT_SDCARD
  sdcard_inited = sdcard_init();

  if (sdcard_inited)
  {
    // wm8978_inited = wm8978_sdcard();
    selected_file = fileList;
  }
  file_menu_display();
#endif
#ifdef _COMPONENT_WM8978
  wm8978Init();
#endif
#ifdef _COMPONENT_WM8978_AUDIO
  wm8978_i2s_init();
  audio.connecttohost("http://192.168.1.7/2603174988.mp3");
#endif

  // a2dp_setup();
#ifdef _COMPONENT_BLUETOOTH
  //--------btaudio 蓝牙音频初始化-------
  bt_audio.begin();
  bt_audio.reconnect();

  bt_audio.I2S(I2S_BCK, I2S_DOUT, I2S_WS);
  bt_audio.i2s_mclk_pin_select(3);
#endif
}

void loop()
{
  // put your main code here, to run repeatedly:
  // if (wm8978_inited)
#ifdef _COMPONENT_WM8978_AUDIO
  audio.loop();
#endif
#ifdef _COMPONENT_OTA
  ArduinoOTA.handle(); // OTA升级
#endif
#ifdef _COMPONENT_HTTPSERVER
  server.handleClient();
#endif
#ifdef _COMPONENT_LED
  led_loop();
#endif
#ifdef _COMPONENT_DTH11
  dht_loop();
#endif
  menu_key = key_loop();
  mainMenu(menu_key);
  delay(10);
}