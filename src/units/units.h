#ifndef __UNITS_H__
#define __UNITS_H__

#include <Arduino.h>
#include <SD.h>
#include <FS.h>

struct FM_M3U8_LIST
{
    char *name;
    char *url;
    struct FM_M3U8_LIST *next;
    struct FM_M3U8_LIST *pre;
};

void getM3uList(struct FM_M3U8_LIST **head); // 从SD卡 读取 网络收音的 m3u 列表

#endif