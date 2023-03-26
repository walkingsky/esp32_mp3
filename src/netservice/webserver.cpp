#include "webserver.h"

/* 404 返回 */
void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}

void handleRoot()
{
    String htmlCode = "<!DOCTYPE html>\n";
    htmlCode += " <html>\n";
    htmlCode += "   <head>\n";
    htmlCode += "     <meta charset=\"UTF-8\"/>\n";
    htmlCode += "     <title>ESP32 mp3 控制</title>\n";
    htmlCode += "   </head>\n";
    htmlCode += "   <body>\n<div style=\"width:600px;margin:0 auto;\">\n";
    htmlCode += "     </div>\n";
    htmlCode += "   </body>\n";
    htmlCode += "</html>\n";
    server.send(200, "text/html", htmlCode); // NodeMCU将调用此函数。
}

/* web 服务开启 */
void webServiceBegin()
{
    server.begin(80);
    server.onNotFound(handleNotFound);
    server.on("/", handleRoot);
    httpUpdater.setup(&server);
}