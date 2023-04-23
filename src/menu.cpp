#include "menu.h"

#include "gpios.h"

#ifdef _COMPONENT_SDCARD
struct dirList *fileList = NULL; // 目录链表
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

/*
extern struct MenuTree *main_time;
extern struct MenuTree *main_cdcard;
extern struct MenuTree *main_record;
extern struct MenuTree *main_longrec;
extern struct MenuTree *m_cdcard;
extern struct MenuTree *menu_p;
*/
struct MenuTree main_time;
struct MenuTree main_cdcard;
struct MenuTree main_record;
struct MenuTree main_longrec;
struct MenuTree m_cdcard;
struct MenuTree *menu_p = NULL;

uint8_t vol = 12;

// 主菜单显示时间用的变量
uint8_t _last_second, _last_min, _last_day;
bool menu_changed = false;

void doMenu(uint8_t key) // 主菜单
{
    // if (NULL == menu_p->menu_op && key == 0)
    if (key == 0)
    {
        (*menu_p->menu_disp)();
    }
    else if (menu_p->menu_op == NULL && key > 0)
    {
        switch (key)
        {
        case KEY_RIGHT:
            if (menu_p->right != NULL)
            {
                menu_changed = true;
                menu_p = menu_p->right;
            }
            break;
        case KEY_LEFT:
            if (menu_p->left != NULL)
            {
                menu_changed = true;
                menu_p = menu_p->left;
            }
            break;
        case KEY_UP:
            if (menu_p->parent != NULL)
            {
                menu_changed = true;
                menu_p = menu_p->parent;
            }
            break;
        case KEY_DOWN:
            if (menu_p->current_child != NULL)
            {
                menu_changed = true;
                menu_p = menu_p->current_child;
            }
            break;
        case KEY_OK:
            if (menu_p->current_child != NULL)
            {
                menu_changed = true;
                menu_p = menu_p->current_child; // 切换到下级子菜单
            }
            break;
        }
    }
    else if (menu_p->menu_op != NULL && key > 0)
    {

        (*menu_p->menu_op)(key);
    }
}

void menuInit()
{
    /*
    main_time = (struct MenuTree)malloc(sizeof(struct MenuTree));
    main_cdcard = (struct MenuTree)malloc(sizeof(struct MenuTree));
    main_record = (struct MenuTree)malloc(sizeof(struct MenuTree));
    main_longrec = (struct MenuTree)malloc(sizeof(struct MenuTree));

    m_cdcard = (struct MenuTree *)malloc(sizeof(struct MenuTree));
    */

    main_time.parent = NULL;
    // main_time.menu_No = 1;
    main_time.menu_disp = main_time_display;
    main_time.left = &main_longrec;
    main_time.right = &main_cdcard;
    main_time.current_child = NULL;
    main_time.menu_op = NULL;

    main_cdcard.parent = NULL;
    // main_cdcard.menu_No = 2;
    main_cdcard.menu_disp = main_sdcard_display;
    main_cdcard.left = &main_time;
    main_cdcard.right = &main_record;
    main_cdcard.current_child = &m_cdcard;
    main_cdcard.menu_op = NULL;

    main_record.parent = NULL;
    // main_record.menu_No = 3;
    main_record.menu_disp = main_record_display;
    main_record.left = &main_cdcard;
    main_record.right = &main_longrec;
    main_record.current_child = NULL;
    main_record.menu_op = NULL;

    main_longrec.parent = NULL;
    // main_longrec.menu_No = 4;
    main_longrec.menu_disp = main_longrec_display;
    main_longrec.left = &main_record;
    main_longrec.right = &main_time;
    main_longrec.current_child = NULL;
    main_longrec.menu_op = NULL;

    m_cdcard.parent = &main_cdcard;
    // m_cdcard.menu_No = 5;
    m_cdcard.menu_disp = m_sdcard_display;
    m_cdcard.left = NULL;
    m_cdcard.right = NULL;
    m_cdcard.current_child = NULL;
    m_cdcard.menu_op = m_sdcard_content;

    menu_p = &main_time;
    menu_changed = true;
}

void m_sdcard_display()
{
    if (menu_changed)
    {
        listDir(SD, "/", &fileList);
        selected_file = fileList;
        file_menu_display(selected_file);
        menu_changed = false;
    }
}

void m_sdcard_content(uint8_t key)
{
    log_e("3");
    if (NULL == selected_file)
        return;

#ifdef _COMPONENT_SDCARD

    if (key > 0)
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
                file_menu_display(selected_file);
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

                file_menu_display(selected_file);
            }
#endif
        }

        if (key == 5) // ok键
        {
        }
    }

#endif
}

void main_time_display() // 主菜单（时间）
{
    uint8_t _second, _minute, _day;
    if (menu_changed)
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
        menu_changed = false;
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
                    menu_changed = true; // 全部重刷
                    main_time_display();
                }
            }
            u8g2.sendBuffer();
        }
    }

#ifdef _COMPONENT_DHT11

#endif
}

void main_sdcard_display() // cd card file list
{
    if (menu_changed)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_sdcard);
        u8g2.sendBuffer();
        menu_changed = false;
    }
}
void main_record_display() // 录音子菜单
{
    if (menu_changed)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_record);
        u8g2.sendBuffer();
        menu_changed = false;
    }
}
void main_longrec_display() // 监听录音菜单
{
    if (menu_changed)
    {
        u8g2.clear();
        u8g2.drawXBMP(2, 2, 60, 60, img_longrec);
        u8g2.sendBuffer();
        menu_changed = false;
    }
}

void m_record_content(uint8_t key) // 录音操作页面
{
}
void m_longrec_content(uint8_t key) // 监听录音操作页面
{
}

#ifdef _COMPONENT_SDCARD
void file_menu_display(struct dirList *p) // 文件列表菜单展示
{
    // struct dirList *p = fileList;
    // struct dirList *p = selected_file;
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