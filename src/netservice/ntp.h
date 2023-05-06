#ifndef __NTP_H__
#define __NTP_H__

// #include <WiFi.h>
//  #include <WiFiUdp.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <sntp.h>
#include <stdlib.h>

void ntpBegin();
time_t set_time();
// time_t getNtpTime();
// void sendNTPpacket(IPAddress &address);

#endif