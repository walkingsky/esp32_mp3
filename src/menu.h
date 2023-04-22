#ifndef __MENU_H__
#define __MENU_H__

#include "main.h"
#include "oled/oled.h"
#include <TimeLib.h>
#ifdef _COMPONENT_WM8978_AUDIO
#include "audio/wm8978card.h"
#endif

void mainMenu(int key);
#ifdef _COMPONENT_SDCARD
void file_menu_display();
#endif
void main_menu_display(); // 主菜单（时间）

#endif