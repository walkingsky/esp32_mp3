#include "wifi.h"
#include "oled/oled.h"

extern EepromConf eepromConf;

bool wifiConnected = false;    // wifi 是否连接上的标记
uint8_t wifi_connect_cnt = 60; // wifi连接重试次数。配合500毫秒的间隔，实际时间为60*0.5s = 30秒钟
uint8_t SmartConfigStatus = 0; // 是否在smartconfig自动配网状态，0，未开始，1，等待配置 2，收到配置，连接wifi，3 连接wifi配置错误 ，4。wifi正常连接
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
void connect_wifi() // 联网
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(eepromConf.wifi_ssid, eepromConf.wifi_password); // 用固定的账号密码连接网络
    uint8_t cnt = 0;
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(9, 55);
    u8g2.print("connect to wifi");
    u8g2.sendBuffer();
    // 连接wifi的时间控制计数
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        u8g2.setCursor(100, 55);
        if (cnt % 4 == 1)
            u8g2.print(".");
        else if (cnt % 4 == 2)
            u8g2.print("..");
        else if (cnt % 4 == 3)
            u8g2.print("...");
        else
        {
            u8g2.setDrawColor(0);
            u8g2.drawBox(100, 43, 25, 14);
            u8g2.setDrawColor(1);
        }
        cnt = cnt + 1;
        // Serial.print("cnt:");
        // Serial.println(cnt);
        if (cnt > wifi_connect_cnt)
        {
            Serial.println("超过重试次数");
            break;
        }
        u8g2.sendBuffer();
    }
    if (cnt > wifi_connect_cnt)
    {

        u8g2.clear();
        u8g2.drawXBMP(43, 12, 40, 28, wifi);
        u8g2.setCursor(7, 60);
        u8g2.print("SmartConfig to wifi");
        u8g2.sendBuffer();
        SmartConfigStatus = 0; // 设置mart config状态
        while (true)
        {
            bool success = smart_config();
            if (success == true)
                break;
        }
    }
    SmartConfigStatus = 4;
    wifiConnected = true; // 设置wifi连接状态

    Serial.print("WiFi connected to: ");
    Serial.println(eepromConf.wifi_ssid);
    Serial.print("password: ");
    Serial.println(eepromConf.wifi_password);
    Serial.print("IP:   ");
    Serial.println(WiFi.localIP()); // 得到IP地址
    // local_IP = WiFi.localIP().toString();
}

/* 自动配网  */
bool smart_config()
{
    uint8_t cnt = 1;
    WiFi.mode(WIFI_STA);     // 这里一定要将WIFI设置为客户端模式才能进行配网
    WiFi.beginSmartConfig(); // 将esp8266设置为智能配网模式
    Serial.println("Waiting for SmartConfig.");
    SmartConfigStatus = 1;
    while (!WiFi.smartConfigDone())
    {
        delay(500);
        // Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");
    SmartConfigStatus = 2;

    // Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        cnt = cnt + 1;
        if (cnt > wifi_connect_cnt / 2)
        {
            SmartConfigStatus = 3;
            WiFi.stopSmartConfig(); // 停止smartconfig，为下一轮配置准备
            return false;
        }
        // Serial.print(".");
    }

    Serial.println("WiFi Connected.");
    SmartConfigStatus = 4;
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); // 串口输出现在的IP地址
    // local_IP = WiFi.localIP().toString();
    //  WiFi.mode(WIFI_AP_STA);         // 这里将模式设置回AP和STA双模式，不设置亲测也是可以的，但是不能只设置为AP模式，要不然联网后没办法连上互联网。
    delay(5);
    strcpy(eepromConf.wifi_ssid, WiFi.SSID().c_str());
    strcpy(eepromConf.wifi_password, WiFi.psk().c_str());
    // Serial.printf("wifi ssid:%s password:%s \n", eepromConf.wifi_ssid, eepromConf.wifi_password);
    writeEEpromConf();
    return true;
}