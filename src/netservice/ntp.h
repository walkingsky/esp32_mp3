#ifndef __NTP_H__
#define __NTP_H__

#include <WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

static const char ntpServerName[] = "ntp1.aliyun.com"; // NTP服务器
const int timeZone = 8;                                // 时区，东八区为北京时间
static WiFiUDP Udp;
static unsigned int localPort = 8888;      // 连接时间服务器的本地端口号
static time_t prevDisplay = 0;             // 上一次获取到的时间
const int NTP_PACKET_SIZE = 48;            // NTP发送数据包长度
static byte packetBuffer[NTP_PACKET_SIZE]; // NTP数据包缓冲区

void ntpBegin();
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

#endif