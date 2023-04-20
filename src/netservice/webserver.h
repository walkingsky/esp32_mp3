#ifndef __WEBSERVER_H__
#define __WEBSERVER_H_

#include "components_define.h"
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#ifdef _COMPONENT_OTA
#include <ArduinoOTA.h>
#endif
#ifdef _COMPONENT_SDCARD
#include "SD.h"
#include "FS.h"
#endif
void webServiceBegin();
#ifdef _COMPONENT_OTA
void setUpOverTheAirProgramming(); // OAT升级
#endif

#ifdef _COMPONENT_SDCARD

void append_page_header();
void append_page_footer();
void HomePage();
void File_Download();
void SD_file_download(String filename);
void File_Upload();
void handleFileUpload();
void SD_dir();
void printDirectory(const char *dirname, uint8_t levels);
void File_Stream();
void SD_file_stream(String filename);
void File_Delete();
void SD_file_delete(String filename);
void SendHTML_Header();
void SendHTML_Content();
void SendHTML_Stop();
void SelectInput(String heading1, String command, String arg_calling_name);
void ReportSDNotPresent();
void ReportFileNotPresent(String target);
void ReportCouldNotCreateFile(String target);
String file_size(int bytes);
#endif

#endif