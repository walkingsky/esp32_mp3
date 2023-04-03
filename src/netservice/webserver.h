#ifndef __WEBSERVER_H__
#define __WEBSERVER_H_

#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ArduinoOTA.h>

void webServiceBegin();
void setUpOverTheAirProgramming(); // OAT升级

#endif