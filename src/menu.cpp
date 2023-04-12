#include "main.h"
#include "oled/oled.h"

// extern uint8_t key_value;
extern struct dirList *fileList; // 目录链表

extern Audio audio;

extern struct dirList *selected_file;
uint8_t vol = 12;

void mainMenu(int key) // 主菜单
{
    if (fileList == NULL)
        return;

    if (key == 0)
        return;

    Serial.printf("key:%d\r\n", key);

    if (key == 1) // 上键
    {
        if (audio.isRunning())
            audio.stopSong();
    }

    if (key == 2) // 右键
    {
        if (vol < 21)
            vol++;
        audio.setVolume(vol);
        Serial.printf("audio volume:%d\r\n", vol);
    }

    if (key == 3) // 左键
    {
        if (vol > 0)
            vol--;
        audio.setVolume(vol);
        Serial.printf("audio volume:%d\r\n", vol);
    }

    if (key == 4) // 下键
    {
        if (audio.isRunning())
            audio.stopSong();
        // selected_file = fileList;
        if (selected_file->next != NULL)
            selected_file = selected_file->next;
        else
            selected_file = fileList;
        if (selected_file != NULL)
        {
            while (selected_file->filetype != TYPE_FILE && selected_file->next != NULL)
            {
                Serial.printf("while file:%s\r\n", selected_file->name);
                selected_file = selected_file->next;
            }
            if (selected_file->filetype != TYPE_FILE) // 到最后的一个文件是目录，就跳转回第一个
                selected_file = fileList;

            audio.connecttoFS(SD, selected_file->name);
            Serial.printf("audio play SD file:%s\r\n", selected_file->name);
        }
    }

    if (key == 5) // ok键
    {
        if (audio.isRunning())
            audio.pauseResume();
        else
            audio.pauseResume();

        Serial.printf("Bit Rate:%ld \r\n", audio.getBitRate());
    }
}

void file_menu_display() // 文件列表菜单展示
{
    struct dirList *p = fileList;
    uint8_t y = 15;
    drawUTF8String(0, 0, "", true); // 清屏
    while (p->next != NULL)
    {
        if (p->filetype == TYPE_FILE)
        {
            drawUTF8String(0, y, (String)p->name, false);
            y = y + 15;
        }
        if (y > 64)
            break;
        p = p->next;
    }
}
