#ifndef __SDCARD_H__
#define __SDCARD_H__

#include "SD.h"

enum fileType // 类型，0 dir，1 file
{
    TYPE_DIR = 1, // 目录
    TYPE_FILE     // 文件
};
// 存放文件列表的单链表
struct dirList
{
    char name[100]; // 目录或文件名
    enum fileType filetype;
    struct dirList *next;
};

bool sdcard_init();
#endif
