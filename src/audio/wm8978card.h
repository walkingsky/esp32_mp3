#ifndef __WM8978CARD_H__
#define __WM8978CARD_H__

#include "components_define.h"
#include "gpios.h"

#ifdef _COMPONENT_WM8978
#include "WM8978.h" /* https://github.com/CelliesProjects/wm8978-esp32 */
#endif

#ifdef _COMPONENT_WM8978
#include "esp32_audioi2s/Audio.h" /* https://github.com/schreibfaul1/ESP32-audioI2S */
#endif

#ifdef _COMPONENT_WM8978
void wm8978Init();
#endif
#ifdef _COMPONENT_WM8978_AUDIO
void wm8978_i2s_init();

void wm8978_i2s_init();
bool wm8978_sdcard();
void wm8978_record(char *path,bool long_record = false);
void wm8978_stop_record(bool long_record = false);
#endif

#endif