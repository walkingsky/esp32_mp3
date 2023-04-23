#include "menu.h"

#include "gpios.h"

#ifdef _COMPONENT_SDCARD
struct dirList *fileList; // 目录链表
struct dirList *selected_file = NULL;
#endif
#ifdef _COMPONENT_WM8978_AUDIO
extern Audio audio;
#endif

#define ISLONGRECORD 1
#define NOTLONGRECORD 0

// extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
// extern U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2;

uint8_t vol = 12;

// 主菜单显示时间用的变量
bool _main_menu_first_display = true;
uint8_t _last_second, _last_min, _last_day;
// 菜单级别
uint8_t menu_level = 0;
uint8_t menu_selected = 0;

void mainMenu(int key) // 主菜单
{

    if (menu_level == MENU_LEVEL_MAIN) // 一级菜单
    {
        if (key == KEY_RIGHT)
        {
            if (menu_selected < MENU_LEVEL_MAIN_SUB_CNT - 1)
                menu_selected++;
            else
                menu_selected = 0;
            _main_menu_first_display = true;
        }
        else if (key == KEY_LEFT && menu_selected != MENU_L0_TIME) // 主菜单不支持ok键
        {
            if (menu_selected > 0)
                menu_selected--;
            else
                menu_selected = MENU_LEVEL_MAIN_SUB_CNT - 1;
            _main_menu_first_display = true;
        }
        else if (key == KEY_OK)
        {
            menu_level = MENU_LEVEL_SBU;
            _main_menu_first_display = true;
        }
        switch (menu_selected)
        {
        case MENU_L0_TIME:
            main_time_display();
            break;
        case MENU_L0_CDCARD:
            main_cdcard_display();
            break;
        case MENU_L0_RECORD:
            main_record_display();
            break;
        case MENU_L0_LONGREC:
            main_longrec_display();
            break;
        }
    }
    else if (menu_level == MENU_LEVEL_SBU) // 进入二级菜单
    {
        switch (menu_selected)
        {
        // case MENU_L0_TIME:
        //     main_time_display();
        //     break;
        case MENU_L0_CDCARD:
            main_sdcard_content(key);
            break;
        case MENU_L0_RECORD:
            main_record_display();
            break;
        case MENU_L0_LONGREC:
            main_longrec_display();
            break;
        }
    }
    return;
}

void main_sdcard_content(int key)
{
    log_e("key:%d _main_menu_first_display:%d", key, _main_menu_first_display);
#ifdef _COMPONENT_SDCARD
    if (_main_menu_first_display)
    {
        fileList = listDir(SD, "/");
        selected_file = fileList;
        file_menu_display();
        _main_menu_first_display = false;
    }
    else if (key > 0)
    {
        if (key == KEY_UP) // 上键
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

        if (key == KEY_RIGHT) // 右键
        {
        }
        if (key == KEY_LEFT) // 左键
        {
        }

        if (key == KEY_DOWN) // 下键
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
        }
    }

#endif

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
#ifdef _COMPONENT_WM8978_AUDIO
        if (1)
        {
            if (audio.isLRRedording())
                wm8978_stop_record(ISLONGRECORD);
            else if (!audio.isRedording())
                wm8978_record(NULL, ISLONGRECORD);
        }
        else
        {
            if (audio.isRedording())
                wm8978_stop_record(NOTLONGRECORD);
            else if (!audio.isRedording())
                wm8978_record((char *)"record.wav", NOTLONGRECORD);
        }

#endif
    }
}

void main_time_display() // 主菜单（时间）
{
    uint8_t _second, _minute, _day;
    if (_main_menu_first_display)
    {
        u8g2.clear();
        u8g2.drawHLine(0, 40, 128);
        u8g2.drawVLine(39, 40, 29);
        u8g2.drawVLine(96, 40, 29);
        u8g2.setFont(u8g2_font_7_Seg_33x19_mn); // with:19 Height:31 capital a:-2
        u8g2.setCursor(4, 3);
        u8g2.printf("%02d:%02d", hour(), minute());
        u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
        u8g2.setCursor(106, 33);
        u8g2.printf("%02d", second());

        u8g2.setCursor(40, 62);
        u8g2.printf("%02d-%2d", month(), day());
#ifdef _COMPONENT_DHT11
        // 获取温度
        struct DHT_result dht_res = dht_loop();
        if (dht_res.humidity > 0)
        {
            u8g2.setFont(u8g2_font_6x12_tf); // width 6,height 12,captial A 7
            u8g2.setCursor(1, 64);
            u8g2.printf("%4.1f°C", dht_res.temperature);
            u8g2.setCursor(92, 64);
            u8g2.printf("%5.1f%%", dht_res.humidity);
        }
#endif
        _main_menu_first_display = false;
        _last_day = day();
        _last_min = minute();
        _last_second = second();
        u8g2.sendBuffer();
    }
    else
    {
        _second = second();
        if (_second != _last_second)
        {
            u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
            u8g2.setDrawColor(0);
            u8g2.drawBox(106, 16, 22, 20);
            u8g2.setDrawColor(1);
            u8g2.setCursor(106, 33);
            u8g2.printf("%02d", _second);
            _last_second = _second;
            _minute = minute();

            if (_minute != _last_min)
            {
                u8g2.setFont(u8g2_font_7_Seg_33x19_mn); // with:19 Height:31 capital a:-2
                u8g2.setDrawColor(0);
                u8g2.drawBox(1, 1, 96, 33);
                u8g2.setDrawColor(1);
                u8g2.setCursor(4, 3);
                u8g2.printf("%02d:%02d", hour(), minute());
                _last_min = _minute;
#ifdef _COMPONENT_DHT11
                // 获取温度
                struct DHT_result dht_res = dht_loop();
                u8g2.setFont(u8g2_font_6x12_tf); // width 6,height 12,captial A 7
                u8g2.setDrawColor(0);
                u8g2.drawBox(1, 52, 37, 15);
                u8g2.drawBox(97, 52, 37, 15);
                u8g2.setDrawColor(1);
                if (dht_res.humidity > 0)
                {
                    u8g2.setCursor(1, 64);
                    u8g2.printf("%4.1f°C", dht_res.temperature);
                    u8g2.setCursor(98, 64);
                    u8g2.printf("%4.1f%%", dht_res.humidity);
                }
#endif
                _day = day();
                if (_day != _last_day)
                {
                    _main_menu_first_display = true;
                    main_time_display();
                }
            }
            u8g2.sendBuffer();
        }
    }

#ifdef _COMPONENT_DHT11

#endif
}

void main_cdcard_display() // cd card file list
{
    if (_main_menu_first_display)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_sdcard);
        u8g2.sendBuffer();
        _main_menu_first_display = false;
    }
}
void main_record_display() // 录音子菜单
{
    if (_main_menu_first_display)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_record);
        u8g2.sendBuffer();
        _main_menu_first_display = false;
    }
}
void main_longrec_display() // 监听录音菜单
{
    if (_main_menu_first_display)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_longrec);
        u8g2.sendBuffer();
        _main_menu_first_display = false;
    }
}

void main_record_content(int key) // 录音操作页面
{
}
void main_longrec_content(int key) // 监听录音操作页面
{
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