#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <Preferences.h>
#include <nvs_flash.h>

/* eeprom 参数 */
struct EepromConf
{
    char wifi_ssid[50];
    char wifi_password[50];
    // u_int8_t gif_mode;
    // uint16_t frontColor;
};

void readEEpromConf();
void writeEEpromConf();

#endif