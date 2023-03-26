#ifndef __WEBSERVER_H__
#define __WEBSERVER_H_

#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <ArduinoOTA.h>

static WebServer server(80);
static HTTPUpdateServer httpUpdater;

void webServiceBegin();
void setUpOverTheAirProgramming(); // OAT升级

#endif