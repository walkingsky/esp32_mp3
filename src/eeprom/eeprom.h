#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <EEPROM.h>

/* eeprom 参数 */
struct EepromConf
{
    char wifi_ssid[50];
    char wifi_password[50];
    // Make sure that there is a 0
    // that terminatnes the c string
    // if memory is not initalized yet.
    char cstr_terminator = 0; // makse sure
    // u_int8_t gif_mode;
    // uint16_t frontColor;
};

static EepromConf eepromConf;

void readEEpromConf();
void writeEEpromConf();

#endif