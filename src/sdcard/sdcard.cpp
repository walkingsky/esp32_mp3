#include "sdcard.h"

// 初始化列表
struct dirList *list_init()
{ // 分配堆空间
    struct dirList *head = (struct dirList *)malloc(sizeof(struct dirList));
    if (NULL == head)
    {
        // printf("malloc head fail\n");
        return NULL;
    }
    head->pre = NULL;
    head->next = NULL;
    head->num = 0;
    // head->name = NULL;
    return head;
}
// 插入数据（尾插）
void list_insert(struct dirList *head, enum fileType filetype, const char *name)
{ // 准备新节点
    struct dirList *newlist = (struct dirList *)malloc(sizeof(struct dirList));
    strcpy(newlist->name, name);
    newlist->filetype = filetype;
    newlist->next = NULL;

    // 找到最后一个节点
    struct dirList *p = head;
    while (p->next != NULL)
        p = p->next;

    p->next = newlist; // 挂在末尾
    newlist->pre = p;
    newlist->num = p->num + 1;
}
// 释放存储
void list_free(struct dirList *head)
{
    // 找到最后一个节点
    struct dirList *p = head;
    while (p->next != NULL)
    {
        p = p->next;
        free(head);
        head = p;
    }
    free(head);
}

/*列表单层目录文件*/
void listDir(fs::FS &fs, const char *dirname, struct dirList **p)
{
    struct dirList *fileList = NULL;

    enum fileType filetype;
    // Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }
    if (fileList != NULL)
    {
        list_free(fileList);
    }
    else
        fileList = list_init();
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            // Serial.print("  DIR : ");
            // Serial.println(file.name());
            filetype = TYPE_DIR;
            // file = root.openNextFile(); // 列表里跳过目录
            // continue;                   // 列表里跳过目录
        }
        else
        {
            // Serial.print("  FILE: ");
            // Serial.print(file.name());
            // Serial.print("  SIZE: ");
            // Serial.println(file.size());
            filetype = TYPE_FILE;
        }
        if (fileList->num == 0)
        {
            fileList->filetype = filetype;
            strcpy(fileList->name, file.name());
            fileList->num = 1;
            // Serial.println("填写文件列表链表第一个数据");
        }
        else
            list_insert(fileList, filetype, file.name());
        file = root.openNextFile();
    }
    *p = fileList;
    return;
}

bool sdcard_init()
{
    if (!SD.begin(CS_PIN))
    {
        // Serial.print("sdcard init failed! \r\n");
        return false;
    }
    // listDir(SD, "/");
    return true;
}

void sdcard_end()
{
    SD.end();
}