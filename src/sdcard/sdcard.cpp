#include "sdcard.h"

struct dirList *fileList; // 目录链表
struct dirList *selected_file;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

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

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

// 初始化列表
struct dirList *list_init()
{ // 分配堆空间
    struct dirList *head = (struct dirList *)malloc(sizeof(struct dirList));
    if (NULL == head)
    {
        printf("malloc head fail\n");
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
void listDir(fs::FS &fs, const char *dirname)
{
    enum fileType filetype;
    Serial.printf("Listing directory: %s\r\n", dirname);

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
        list_free(fileList);
    else
        fileList = list_init();
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            filetype = TYPE_DIR;
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            filetype = TYPE_FILE;
        }
        if (fileList->num == 0)
        {
            fileList->filetype = filetype;
            strcpy(fileList->name, file.name());
            fileList->num = 1;
            Serial.println("填写文件列表链表第一个数据");
        }
        else
            list_insert(fileList, filetype, file.name());
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char *path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("Failed to open file for reading");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

bool sdcard_init()
{
    if (!SD.begin(CS_PIN))
    {
        Serial.print("sdcard init failed! \r\n");
        return false;
    }
    listDir(SD, "/");
    return true;
    /*
    Serial.println("SD card Ready!");
    Serial.printf("SD.cardSize = %lld \r\n", SD.cardSize());
    Serial.printf("SD.totalBytes = %lld \r\n", SD.totalBytes());
    Serial.printf("SD.usedBytes = %lld \r\n", SD.usedBytes());
    Serial.printf("SD.cardType = %d \r\n", SD.cardType());
    Serial.printf("is there /test.txt? :%d \r\n", SD.exists("/sd/doc1/test.txt"));
    Serial.println(SD.mkdir("/doc1"));
    Serial.printf("is there /doc1? :%d \r\n", SD.exists("/doc1"));
    Serial.printf("is there /test.txt? :%d \r\n", SD.exists("/test.txt"));
    File file = SD.open("/test.txt", FILE_WRITE);
    Serial.printf("is there /test.txt? :%d \r\n", SD.exists("/test.txt"));
    file.printf("hello!!!");
    file.close();
    file = SD.open("/test.txt", FILE_READ);
    Serial.println(file.readString());
    file.close();
    Serial.printf("is there /doc1/test1.txt? :%d \r\n", SD.exists("/doc1/test1.txt"));
    File file2 = SD.open("/doc1/test1.txt", FILE_WRITE);
    Serial.printf("is there /doc1/test1.txt? :%d \r\n", SD.exists("/doc1/test1.txt"));
    file2.printf("hello!!!");
    file2.close();
    file2 = SD.open("/test.txt", FILE_READ);
    Serial.println(file2.readString());
    file2.close();

    listDir(SD, "/", 0);
    createDir(SD, "/mydir");
    listDir(SD, "/", 0);
    writeFile(SD, "/hello.txt", "Hello ");
    appendFile(SD, "/hello.txt", "World!\n");
    readFile(SD, "/hello.txt");
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    testFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    SD.end();
    */
}

void sdcard_end()
{
    SD.end();
}