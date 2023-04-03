#ifndef __NTP_H__
#define __NTP_H__

#include <WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

void ntpBegin();
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

#endif