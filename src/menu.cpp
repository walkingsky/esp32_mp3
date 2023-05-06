#include "menu.h"
#include "gpios.h"

#ifdef _COMPONENT_SDCARD
struct dirList *fileList = NULL; // 目录链表
struct dirList *selected_file = NULL;
struct FM_M3U8_LIST *m3u8list = NULL; // m3u8 播放列表
struct FM_M3U8_LIST *m3u8 = NULL;
#endif
#ifdef _COMPONENT_WM8978_AUDIO
extern Audio audio;
#endif

#define ISLONGRECORD 1
#define NOTLONGRECORD 0

// extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
// extern U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2;

struct MenuTree main_time;
struct MenuTree main_cdcard;
struct MenuTree main_record;
struct MenuTree main_longrec;
struct MenuTree main_fmradio;
struct MenuTree l2_cdcard;
struct MenuTree l2_record;
struct MenuTree l2_longrecord;
struct MenuTree l2_fmradio;
struct MenuTree *menu_p = NULL;

Ticker clock_timer;
uint8_t _last_min, _last_day;

uint32_t _timer_counter = 0;
bool fmradio_firstrun = true;

uint8_t vol = 12; // 音量

void doMenu(uint8_t key) // 操作菜单
{
    bool menu_changed = false;
    // if (NULL == menu_p->menu_op && key == 0)
    if (key == 0)
        return;

    if (menu_p->menu_op == NULL && key > 0)
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
                menu_p = menu_p->current_child; // some as key_ok
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

    if (menu_changed)
        displayMenu();
}

void menuInit()
{
    // 一级菜单，显示日期时间
    main_time.parent = NULL;
    main_time.menu_No = 1;
    main_time.menu_disp = main_time_display;
    main_time.left = &main_fmradio;
    main_time.right = &main_cdcard;
    main_time.current_child = NULL;
    main_time.menu_op = main_time_op;
    // 一级菜单，显示sd卡icon，有下级菜单
    main_cdcard.parent = NULL;
    main_cdcard.menu_No = 2;
    main_cdcard.menu_disp = main_sdcard_display;
    main_cdcard.left = &main_time;
    main_cdcard.right = &main_record;
    main_cdcard.current_child = &l2_cdcard;
    main_cdcard.menu_op = NULL;
    // 一级菜单，录音icon ，有下级菜单
    main_record.parent = NULL;
    main_record.menu_No = 3;
    main_record.menu_disp = main_record_display;
    main_record.left = &main_cdcard;
    main_record.right = &main_longrec;
    main_record.current_child = &l2_record;
    main_record.menu_op = NULL;
    // 一级菜单，监听录音icon，有下级菜单
    main_longrec.parent = NULL;
    main_longrec.menu_No = 4;
    main_longrec.menu_disp = main_longrec_display;
    main_longrec.left = &main_record;
    main_longrec.right = &main_fmradio;
    main_longrec.current_child = &l2_longrecord;
    main_longrec.menu_op = NULL;

    // 一级菜单，网络收音机，有下级菜单
    main_fmradio.parent = NULL;
    main_fmradio.menu_No = 8;
    main_fmradio.menu_disp = main_fmradio_display;
    main_fmradio.left = &main_longrec;
    main_fmradio.right = &main_time;
    main_fmradio.current_child = &l2_fmradio;
    main_fmradio.menu_op = NULL;

    // 二级菜单，sd卡 音乐播放界面
    l2_cdcard.parent = &main_cdcard;
    l2_cdcard.menu_No = 5;
    l2_cdcard.menu_disp = m_sdcard_display;
    l2_cdcard.left = NULL;
    l2_cdcard.right = NULL;
    l2_cdcard.current_child = NULL;
    l2_cdcard.menu_op = m_sdcard_content;

    // 二级菜单，录音操作界面
    l2_record.parent = &main_record;
    l2_record.menu_No = 6;
    l2_record.menu_disp = m_record_display;
    l2_record.left = NULL;
    l2_record.right = NULL;
    l2_record.current_child = NULL;
    l2_record.menu_op = m_record_content;

    // 二级菜单，监听操作界面
    l2_longrecord.parent = &main_longrec;
    l2_longrecord.menu_No = 7;
    l2_longrecord.menu_disp = m_longrecord_display;
    l2_longrecord.left = NULL;
    l2_longrecord.right = NULL;
    l2_longrecord.current_child = NULL;
    l2_longrecord.menu_op = m_longrecord_content;

    // level2 menu fm radio
    l2_fmradio.parent = &main_fmradio;
    l2_fmradio.menu_No = 9;
    l2_fmradio.menu_disp = m_fmraido_display;
    l2_fmradio.left = NULL;
    l2_fmradio.right = NULL;
    l2_fmradio.current_child = NULL;
    l2_fmradio.menu_op = m_fmraido_content;

    menu_p = &main_time;

    _last_min = minute();
    _last_day = day();
}

// 显示菜单
void displayMenu()
{
    (*menu_p->menu_disp)();
    if (menu_p->menu_No == 1)                           // 主菜单触发页面时间更新计时器
        clock_timer.attach(1, main_time_display_clock); // 1秒触发 1000000微秒
    else
        clock_timer.detach(); // 停用计时器

    if ((menu_p->menu_No == 6 || menu_p->menu_No == 7) && audio.isRunning())
        audio.stopSong();
}

void m_sdcard_display()
{

    listDir(SD, "/", &fileList);
    selected_file = fileList;
    file_menu_display(selected_file);
}

void main_time_op(uint8_t key)
{
    if (key == KEY_UP)
    {
        if (vol < 21)
        {
            vol++;
            audio.setVolume(vol);
        }
    }
    else if (key == KEY_DOWN)
    {
        if (vol > 0)
        {
            vol--;
            audio.setVolume(vol);
        }
    }
    else if (key == KEY_LEFT)
        menu_p = menu_p->left;
    else if (key == KEY_RIGHT)
        menu_p = menu_p->right;

    if (key == KEY_DOWN || key == KEY_UP)
    {
        u8g2.setDrawColor(0);
        u8g2.drawBox(121, 1, 4, 10);
        u8g2.setDrawColor(1);
        uint8_t h = vol * 10 / 21;
        u8g2.drawBox(121, 11 - h, 4, h);
        u8g2.sendBuffer();
    }

    if (key == KEY_LEFT || key == KEY_RIGHT)
        displayMenu();
}

void m_sdcard_content(uint8_t key)
{

    if (NULL == selected_file)
        return;

#ifdef _COMPONENT_SDCARD

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
            // 添加自动循环播放；
            // sd_music_loop();
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
            // 添加自动循环播放；
            // sd_music_loop();
        }
#endif
    }

    if (key == KEY_OK) // ok键
    {
    }

    if (key == KEY_LONG_UP) // 长按向上箭头返回
    {
        // 释放资源
        free_fileList();
        selected_file = NULL;
        menu_p = menu_p->parent;
        displayMenu();
    }

#endif
}

void main_time_display() // 主菜单（时间）
{
    uint8_t _second, _minute, _day;
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
    u8g2.printf("%02d-%02d", month(), day());
    u8g2.drawXBMP(5, 41, 13, 14, icon_temperature);
    u8g2.drawXBMP(100, 41, 13, 14, icon_humidity);
    u8g2.drawXBMP(110, 1, 6, 10, icon_sound);
    uint8_t h = vol * 10 / 21;
    u8g2.drawBox(121, 11 - h, 4, h);
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
    else
    {
        u8g2.setFont(u8g2_font_6x12_tf); // width 6,height 12,captial A 7
        u8g2.setCursor(1, 64);
        u8g2.print(" -- °C");
        u8g2.setCursor(92, 64);
        u8g2.print(" -- %");
    }
#else
    u8g2.setFont(u8g2_font_6x12_tf); // width 6,height 12,captial A 7
    u8g2.setCursor(1, 64);
    u8g2.print(" -- °C");
    u8g2.setCursor(92, 64);
    u8g2.print(" -- %%");
#endif
    u8g2.sendBuffer();
}

void main_time_display_clock() // 主时间页面的时钟和温湿度数据定时更新
{
    uint8_t _second = second();
    uint8_t _minute, _day;
    u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
    u8g2.setDrawColor(0);
    u8g2.drawBox(106, 16, 22, 20);
    u8g2.setDrawColor(1);
    u8g2.setCursor(106, 33);
    u8g2.printf("%02d", _second);
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
        else
        {
            u8g2.setFont(u8g2_font_6x12_tf); // width 6,height 12,captial A 7
            u8g2.setCursor(1, 64);
            u8g2.print(" -- °C");
            u8g2.setCursor(92, 64);
            u8g2.print(" -- %");
        }
#endif
        _day = day();
        if (_day != _last_day)
        {
            main_time_display();
        }
        _last_day = day();
    }
    u8g2.sendBuffer();
}

void m_record_content(uint8_t key) // 录音操作页面
{
    switch (key)
    {
    case KEY_UP:
        wm8978_stop_record();
        clock_timer.detach();
        _timer_counter = 0;
        menu_p = menu_p->parent;
        displayMenu();
        break;
    case KEY_LEFT:
        if (audio.isRedording())
            break;
        else
        {
            wm8978_record((char *)"/record.wav");
            u8g2.setFont(u8g2_font_6x12_tf);
            u8g2.setCursor(52, 32);
            u8g2.print("Stop");
            u8g2.sendBuffer();
            _timer_counter = 0;
            clock_timer.attach(1, record_count_timer);
        }
        break;
    case KEY_RIGHT:
        clock_timer.detach();
        _timer_counter = 0;
        u8g2.setDrawColor(0);
        u8g2.drawBox(52, 20, 27, 13);
        u8g2.setDrawColor(1);
        u8g2.sendBuffer();
        audio.connecttoFS(SD, "/record.wav");
        break;
    case KEY_OK:
        if (audio.isRedording())
        {
            clock_timer.detach();
            _timer_counter = 0;
            wm8978_stop_record();
            u8g2.setDrawColor(0);
            u8g2.drawBox(52, 20, 25, 13);
            u8g2.setDrawColor(1);
            u8g2.sendBuffer();
            clock_timer.detach();
            _timer_counter = 0;
        }
        break;
    }
}
void m_longrecord_content(uint8_t key) // 监听录音操作页面
{
    switch (key)
    {
    case KEY_UP:
        wm8978_stop_record();
        clock_timer.detach();
        _timer_counter = 0;
        menu_p = menu_p->parent;
        displayMenu();
        break;
    case KEY_LEFT:
        if (audio.isLRRedording())
            break;
        else
        {
            // audio.LongRecord();
            wm8978_record(NULL, true);
            _timer_counter = 0;
            clock_timer.attach(1, record_count_timer);
        }
        break;
    case KEY_RIGHT:
        wm8978_stop_record();
        clock_timer.detach();
        _timer_counter = 0;
        break;
    }
}
void m_fmraido_content(uint8_t key) // fm 收音机操作
{
    switch (key)
    {
    case KEY_UP:
        // if (audio.isRunning())  //返回广播不停
        //     audio.stopSong();
        menu_p = menu_p->parent;
        displayMenu();
        free_m3u8list();
        fmradio_firstrun = true;
        break;
    case KEY_LEFT:
        if (NULL != m3u8 && NULL != m3u8->pre)
        {
            m3u8 = m3u8->pre;
        }
        fmradio_firstrun = false;
        m_fmraido_display();
        break;
    case KEY_RIGHT:
        if (NULL != m3u8 && NULL != m3u8->next)
        {
            m3u8 = m3u8->next;
        }
        else
        {
            m3u8 = m3u8list;
        }
        fmradio_firstrun = false;
        m_fmraido_display();
        break;
    }
}

void main_sdcard_display() // cd card file list
{
    u8g2.clear();
    u8g2.drawXBMP(44, 10, 40, 40, img_sdcard);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(5, 36);
    u8g2.print("<");
    u8g2.setCursor(122, 36);
    u8g2.print(">");
    u8g2.sendBuffer();
}
void main_record_display() // 录音子菜单
{
    u8g2.clear();
    u8g2.drawXBMP(44, 10, 40, 40, img_record);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(5, 36);
    u8g2.print("<");
    u8g2.setCursor(122, 36);
    u8g2.print(">");
    u8g2.sendBuffer();
}
void main_longrec_display() // 监听录音菜单
{
    u8g2.clear();
    u8g2.drawXBMP(44, 10, 40, 40, img_longrec);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(5, 36);
    u8g2.print("<");
    u8g2.setCursor(122, 36);
    u8g2.print(">");
    u8g2.sendBuffer();
}
void main_fmradio_display() // fm raidon image
{
    u8g2.clear();
    u8g2.drawXBMP(44, 10, 40, 40, img_fmRadio);
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(5, 36);
    u8g2.print("<");
    u8g2.setCursor(122, 36);
    u8g2.print(">");
    u8g2.sendBuffer();
}

void m_record_display() // 录音操作界面初始页面
{
    u8g2.clear();
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(60, 13);
    u8g2.print("^");
    u8g2.setCursor(45, 18);
    u8g2.print("Return");
    u8g2.setCursor(1, 32);
    u8g2.print("<REC");
    u8g2.setCursor(90, 32);
    u8g2.print("Play>");

    u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
    u8g2.setCursor(38, 60);
    u8g2.print("00:00");
    u8g2.sendBuffer();
}

void m_fmraido_display() // fm 收音显示页面，也就是初始化
{
    u8g2.clear();
    u8g2.setFont(u8g2_font_6x12_tf);
    if (fmradio_firstrun)
    {

        getM3uList(&m3u8list);
        if (m3u8list)
        {
            uint8_t len = strlen(m3u8list->name);
            u8g2.setCursor(1, 36);
            u8g2.print("<");
            if (len <= 13)
            {
                u8g2.setCursor((127 - 6 * strlen(m3u8list->name)) / 2, 36);
                u8g2.printf("%s", m3u8list->name);
            }
            else
            {

                char split_char[] = "-_ ";
                if ((strpbrk(m3u8list->name, split_char)) != NULL)
                {
                    char *p = strpbrk(m3u8list->name, split_char);
                    u8g2.setCursor((127 - 6 * strlen(p)) / 2, 44);
                    u8g2.printf("%s", p);
                    char *name = (char *)malloc(sizeof(char) * (len - strlen(p) + 1));
                    strncpy(name, m3u8list->name, sizeof(char) * (len - strlen(p)));
                    name[len - strlen(p)] = '\0'; // 末尾补结束符
                    u8g2.setCursor((127 - 6 * strlen(name)) / 2, 28);
                    u8g2.printf("%s", name);
                    free(name);
                }
                else
                {
                    // 强行对劈
                    char p[len + 1];
                    char chr[len + 1];
                    strcpy(chr, m3u8list->name);
                    strncpy(p, m3u8list->name, len / 2);
                    u8g2.setCursor((127 - 6 * len) / 2, 28);
                    u8g2.printf("%s", p);
                    u8g2.setCursor((127 - 6 * len) / 2, 44);
                    u8g2.printf("%s", chr + len / 2);
                }
            }
            u8g2.setCursor(125, 36);
            u8g2.print(">");
            wm8978_playm3u(m3u8list->url);

            m3u8 = m3u8list;
        }
        else
        {
            u8g2.setCursor(18, 36);
            u8g2.print("No FM List fond");
        }
    }
    else
    {

        if (m3u8 == NULL)
        {
            u8g2.setCursor(18, 36);
            u8g2.print("No FM List fond");
        }
        else
        {
            uint8_t len = strlen(m3u8->name);
            u8g2.setCursor(1, 36);
            u8g2.print("<");
            if (len <= 13)
            {
                u8g2.setCursor((127 - 6 * strlen(m3u8->name)) / 2, 36);
                u8g2.printf("%s", m3u8->name);
            }
            else
            {

                char split_char[] = "-_ ";
                if ((strpbrk(m3u8->name, split_char)) != NULL)
                {
                    char *p = strpbrk(m3u8->name, split_char);
                    u8g2.setCursor((127 - 6 * strlen(p)) / 2, 44);
                    u8g2.printf("%s", p);
                    char *name = (char *)malloc(sizeof(char) * (len - strlen(p) + 1));
                    strncpy(name, m3u8->name, sizeof(char) * (len - strlen(p)));
                    name[len - strlen(p)] = '\0'; // 末尾补结束符
                    u8g2.setCursor((127 - 6 * strlen(name)) / 2, 28);
                    u8g2.printf("%s", name);
                    free(name);
                }
                else
                {
                    // 强行对劈
                    char p[len + 1];
                    char chr[len + 1];
                    strcpy(chr, m3u8->name);
                    strncpy(p, m3u8->name, len / 2);
                    u8g2.setCursor((127 - 6 * len) / 2, 28);
                    u8g2.printf("%s", p);
                    u8g2.setCursor((127 - 6 * len) / 2, 44);
                    u8g2.printf("%s", chr + len / 2);
                }
            }
            u8g2.setCursor(125, 36);
            u8g2.print(">");
            // log_e("name:%s\turl:%s", m3u8->name, m3u8->url);
            wm8978_playm3u(m3u8->url);
        }
    }

    u8g2.sendBuffer();
}

void m_longrecord_display() // 监听录音操作界面初始页面
{
    u8g2.clear();
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(60, 13);
    u8g2.print("^");
    u8g2.setCursor(45, 18);
    u8g2.print("Return");
    u8g2.setCursor(1, 32);
    u8g2.print("<REC");
    u8g2.setCursor(60, 32);
    u8g2.print("L");
    u8g2.setCursor(90, 32);
    u8g2.print("Stop>");

    u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
    u8g2.setCursor(38, 60);
    u8g2.print("00:00");
    u8g2.sendBuffer();
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

void record_count_timer() // 录音时间计时器显示
{
    uint8_t min, sec;
    min = _timer_counter / 60;
    sec = (_timer_counter - min * 60);

    u8g2.setFont(u8g2_font_t0_22_tn); // with:10 Height:19 capital a:17
    u8g2.setDrawColor(0);
    u8g2.drawBox(38, 41, 55, 20);
    u8g2.setDrawColor(1);
    u8g2.setCursor(38, 60);
    u8g2.printf("%02d:%02d", min, sec);
    u8g2.sendBuffer();
    _timer_counter++;
}

void free_fileList()
{
    selected_file = fileList;
    while (selected_file->next != NULL)
    {
        selected_file = selected_file->next;
        free(fileList);
        fileList = selected_file;
    }
    free(fileList);
    // free(selected_file);
}

void free_m3u8list()
{
    m3u8 = m3u8list;
    while (m3u8->next != NULL)
    {
        m3u8 = m3u8->next;
        free(m3u8list);
        m3u8list = m3u8;
    }
    // free(m3u8list);//保留一个变量不清空，后面还要循环使用
}

void sd_music_loop()
{
    while (audio.isRunning())
    {
        delay(1000);
    }
    if (selected_file->next != NULL)
        selected_file = selected_file->next;
    else
        selected_file = fileList;
    if (selected_file != NULL)
    {
        while (selected_file->filetype != TYPE_FILE && selected_file->next != NULL)
        {
            selected_file = selected_file->next;
        }
        if (selected_file->filetype != TYPE_FILE) // 到最后的一个文件是目录，就跳转回第一个
            selected_file = fileList;

        audio.connecttoFS(SD, selected_file->name);

        if (menu_p->menu_No == 5)
            file_menu_display(selected_file);
    }
}