#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "Common_Config.h"
#include <WebServer.h>
#include <WiFi.h>
#include <pgmspace.h>

// dùng chung biến đo khoảng cách
extern float distanceCM;

// ngưỡng chỉnh từ web
extern int threshold_cm;

String generateMainPage();
void handleRoot();
void handleNotFound();
void register_for_route_web_server();
void run_web_server();

#endif
