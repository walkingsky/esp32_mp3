#ifndef __WEBSERVER_H__
#define __WEBSERVER_H_

#include <WebServer.h>
#include <HTTPUpdateServer.h>

static WebServer server(80);
static HTTPUpdateServer httpUpdater;

void webServiceBegin();

#endif