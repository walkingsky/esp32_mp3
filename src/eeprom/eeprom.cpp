#include "eeprom.h"

/* 读取wifi配置 */
void readEEpromConf()
{
    // Read wifi conf from flash
    for (int i = 0; i < sizeof(eepromConf); i++)
    {
        ((char *)(&eepromConf))[i] = char(EEPROM.read(i));
    }
    // Make sure that there is a 0
    // that terminatnes the c string
    // if memory is not initalized yet.
    eepromConf.cstr_terminator = 0;
}

/* 保存wifi配置 */
void writeEEpromConf()
{
    for (int i = 0; i < sizeof(eepromConf); i++)
    {
        EEPROM.write(i, ((char *)(&eepromConf))[i]);
    }
    EEPROM.commit();
}