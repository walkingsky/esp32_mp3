#include "units.h"

// 从SD卡 读取 网络收音的 m3u 列表
void getM3uList(struct FM_M3U8_LIST **head)
{
    struct FM_M3U8_LIST *m3ulist = NULL;
    File m3ufile = SD.open("/m3u/fm_m3u.txt");
    if (!m3ufile)
    {
        log_e("open m3u config file failed!");
        return;
    }
    if (*head != NULL)
    {
        struct FM_M3U8_LIST *p = *head;
        while (p->next != NULL)
        {
            p = p->next;
            free(*head);
            *head = p;
        }
        free(*head);
    }
    m3ulist = (struct FM_M3U8_LIST *)malloc(sizeof(struct FM_M3U8_LIST));
    m3ulist->pre = NULL;
    m3ulist->next = NULL;
    struct FM_M3U8_LIST *p = m3ulist;
    while (m3ufile.available())
    {
        char lineBuffer[50];
        int length = m3ufile.readBytesUntil(',', lineBuffer, sizeof(lineBuffer) - 1);

        if (length > 1)
        {
            lineBuffer[length] = 0;
            // log_e("%s", lineBuffer);
            p->name = (char *)malloc(length);
            strcpy(p->name, lineBuffer);
            length = m3ufile.readBytesUntil('\n', lineBuffer, sizeof(lineBuffer) - 1);
            if (length > 1 && lineBuffer[length - 1] == '\r')
            {
                length--; // to remove \r if it is there
            }
            lineBuffer[length] = 0;
            p->url = (char *)malloc(length);
            strcpy(p->url, lineBuffer);
            log_e("%s", p->url);
            struct FM_M3U8_LIST *newlist = (struct FM_M3U8_LIST *)malloc(sizeof(struct FM_M3U8_LIST));
            if (!newlist)
            {
                log_e("malloc memory failed!");
                break;
            }
            p->next = newlist;
            newlist->pre = p;
            p = newlist;
        }
    }
    // 释放掉最后一个节点
    p = p->pre;
    free(p->next);
    p->next = NULL;

    m3ufile.close();
    *head = m3ulist;
}