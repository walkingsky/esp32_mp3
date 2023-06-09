#include "main.h"

extern EepromConf eepromConf;

#ifdef _COMPONENT_HTTPSERVER
extern WebServer server;
#endif
#ifdef _COMPONENT_WM8978_AUDIO
extern Audio audio;
#endif

#ifdef _COMPONENT_LED
unsigned long LastTime1;
#endif

unsigned long LastTimeKey;

#ifdef _COMPONENT_BLUETOOTH
// --btaudio
btAudio bt_audio = btAudio("ESP_Speaker");
#endif

void setup()
{
#ifdef _COMPONENT_LED
  LastTime1 = 0;
#endif

  LastTimeKey = 0;
  Serial.begin(115200);
  readEEpromConf();
  Serial.printf("eepromConfig wifi ssid:%s password:%s \n\r", eepromConf.wifi_ssid, eepromConf.wifi_password);

  oled_init();
  oled_logo();

  connect_wifi(); // 联网处理
#ifdef _COMPONENT_OTA
  setUpOverTheAirProgramming(); // 开启OTA升级服务
#endif
#ifdef _COMPONENT_NTP
  ntpBegin();
#endif
  pinMode(ADC_KEY, ANALOG);

#ifdef _COMPONENT_LED
  led_init();
#endif
#ifdef _COMPONENT_DHT11
  dht_init();
#endif
#ifdef _COMPONENT_SDCARD
  sdcard_init();
#endif
#ifdef _COMPONENT_WM8978
  wm8978Init();
#endif
#ifdef _COMPONENT_WM8978_AUDIO
  wm8978_i2s_init();
#endif

  // a2dp_setup();
#ifdef _COMPONENT_BLUETOOTH
  //--------btaudio 蓝牙音频初始化-------
  bt_audio.begin();
  bt_audio.reconnect();

  bt_audio.I2S(I2S_BCK, I2S_DOUT, I2S_WS);
  bt_audio.i2s_mclk_pin_select(3);
#endif

#ifdef _COMPONENT_HTTPSERVER
  webServiceBegin(); // http 服务
#endif
  menuInit();
  displayMenu();
}

void loop()
{
  // put your main code here, to run repeatedly:
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
  key_loop();
}
