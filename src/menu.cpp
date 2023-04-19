#include "main.h"
#include "oled/oled.h"
#ifdef _COMPONENT_WM8978_AUDIO
#include "audio/wm8978card.h"
#endif
// extern uint8_t key_value;
extern struct dirList *fileList; // 目录链表
#ifdef _COMPONENT_WM8978_AUDIO
extern Audio audio;
#endif
// extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
// extern U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2;
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
#ifdef _COMPONENT_WM8978_AUDIO
        if (audio.isRunning())
            audio.stopSong();
#endif
#ifdef _COMPONENT_SDCARD
        // selected_file = fileList;
        if (selected_file->pre != NULL)
            selected_file = selected_file->pre;
        else
            selected_file = fileList; // 暂时停在第一个
        if (selected_file != NULL)
        {
            while (selected_file->filetype != TYPE_FILE && selected_file->pre != NULL)
            {
                Serial.printf("while file:%s\r\n", selected_file->name);
                selected_file = selected_file->pre;
            }
            if (selected_file->filetype != TYPE_FILE) // 到最后的一个文件是目录，就跳转回第一个
                selected_file = fileList;
#ifdef _COMPONENT_WM8978_AUDIO
            audio.connecttoFS(SD, selected_file->name);
            Serial.printf("audio play SD file:%s\r\n", selected_file->name);
#endif
            file_menu_display();
        }
#endif
    }

    if (key == 2) // 右键
    {
        if (vol < 21)
            vol++;
#ifdef _COMPONENT_WM8978_AUDIO
        audio.setVolume(vol);
        Serial.printf("audio volume:%d\r\n", vol);
#endif
    }

    if (key == 3) // 左键
    {
        if (vol > 0)
            vol--;
#ifdef _COMPONENT_WM8978_AUDIO
        audio.setVolume(vol);
        Serial.printf("audio volume:%d\r\n", vol);
#endif
    }

    if (key == 4) // 下键
    {
#ifdef _COMPONENT_WM8978_AUDIO
        if (audio.isRunning())
            audio.stopSong();
#endif
#ifdef _COMPONENT_SDCARD
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
#ifdef _COMPONENT_WM8978_AUDIO
            audio.connecttoFS(SD, selected_file->name);
            Serial.printf("audio play SD file:%s\r\n", selected_file->name);
#endif

            file_menu_display();
        }
#endif
    }

    if (key == 5) // ok键
    {
        /*
        if (audio.isRunning() && !audio.isRedording())
            audio.pauseResume();
        else if (!audio.isRunning() && !audio.isRedording())
            audio.pauseResume();
        else if (!audio.isRunning() && !audio.isRedording())
            audio.RecordToSD();
        else if (!audio.isRunning() && audio.isRedording())
            audio.StopRecord();
        */
#ifdef _COMPONENT_WM8978_AUDIO
        if (audio.isRedording())
            wm8978_stop_record();
        else if (!audio.isRedording())
            wm8978_record((char *)"record.wav");
            // Serial.printf("Bit Rate:%ld \r\n", audio.getBitRate());
#endif
    }
}

#ifdef _COMPONENT_SDCARD
void file_menu_display() // 文件列表菜单展示
{
    // struct dirList *p = fileList;
    struct dirList *p = selected_file;
    uint8_t y = 15;
    // drawUTF8String(0, 0, "", true); // 清屏
    u8g2.clear();
    u8g2.setFont(u8g2_font_6x12_m_symbols);
    while (p != NULL)
    {
        if (p->filetype == TYPE_FILE)
        {
            u8g2.drawGlyph(0, y, 9707);  // 画个文件图标
            u8g2.drawGlyph(10, y, 9657); // 画个播放按钮0x25B9 0x25B8
            u8g2.setCursor(20, y);
            u8g2.printf("%d%s", p->num, strrchr(p->name, '.') == NULL ? "" : strrchr(p->name, '.'));
        }
        else
        {
            u8g2.drawGlyph(0, y, 9706); // 画个文件夹图标
            u8g2.setCursor(20, y);
            u8g2.print(p->name);
        }

        y = y + 15;
        if (y > 64)
            break;
        p = p->next;
    }
    u8g2.sendBuffer();
}
#endif