#ifndef __WM8978_H__
#define __WM8978_H__

#include <WM8978.h> /* https://github.com/CelliesProjects/wm8978-esp32 */
#include <Audio.h>  /* https://github.com/schreibfaul1/ESP32-audioI2S */
#include "gpios.h"

void wm8978Init();
bool wm8978_sdcard();
#endif