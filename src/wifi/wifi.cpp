#include "wifi.h"

void connect_wifi() // 联网
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(eepromConf.wifi_ssid, eepromConf.wifi_password); // 用固定的账号密码连接网络
    uint8_t cnt = 0;                                            // 连接wifi的时间控制计数
    while (WiFi.status() != WL_CONNECTED)
    { // 未连接上的话
        for (uint8_t n = 0; n < 10; n++)
        { // 每500毫秒检测一次状态
          // PowerOn_Loading(50);
        }
        cnt = cnt + 1;
        // Serial.print("cnt:");
        // Serial.println(cnt);
        if (cnt > wifi_connect_cnt)
        {
            Serial.print("\n超过重试次数");
            break;
        }
    }
    if (cnt > wifi_connect_cnt)
    {
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
    while (loadNum < 194)
    { // 让动画走完
      // PowerOn_Loading(1);
    }

    Serial.print("\nWiFi connected to: ");
    Serial.println(eepromConf.wifi_ssid);
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
        // delay(500);
        for (uint8_t n = 0; n < 10; n++)
        { // 每500毫秒检测一次状态
          // PowerOn_Loading(50);
        }
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");
    SmartConfigStatus = 2;

    // Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        // delay(500);
        for (uint8_t n = 0; n < 10; n++)
        { // 每500毫秒检测一次状态
          // PowerOn_Loading(50);
        }
        cnt = cnt + 1;
        if (cnt > wifi_connect_cnt / 2)
        {
            SmartConfigStatus = 3;
            WiFi.stopSmartConfig(); // 停止smartconfig，为下一轮配置准备
            return false;
        }
        Serial.print(".");
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
    writeEEpromConf();
    return true;
}