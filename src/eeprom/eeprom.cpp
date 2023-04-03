#include "eeprom.h"

EepromConf eepromConf;
static Preferences preferences;

/* 读取wifi配置 */
void readEEpromConf()
{
    // nvs_flash_init(); // initializes
    bool temp = preferences.begin("esp32-mp3", true);
    if (temp == false) // 失败就重新格式化存储
    {
        nvs_flash_erase(); // reformats
        nvs_flash_init();  // initializes
        Serial.print("重新格式化nvs flash\n");
        while (true)
            ;
    }
    strcpy(eepromConf.wifi_ssid, preferences.getString("wifi_ssid").c_str());
    // eepromConf.wifi_ssid = preferences.getString("wifi_ssid");
    strcpy(eepromConf.wifi_password, preferences.getString("wifi_password").c_str());
    preferences.end();
}

/* 保存wifi配置 */
void writeEEpromConf()
{
    preferences.begin("esp32-mp3", false);
    preferences.putString("wifi_ssid", String(eepromConf.wifi_ssid));
    preferences.putString("wifi_password", String(eepromConf.wifi_password));
    preferences.end();
}