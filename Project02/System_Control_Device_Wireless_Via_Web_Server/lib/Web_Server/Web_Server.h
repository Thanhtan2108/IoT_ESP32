#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <pgmspace.h>
#include "Common_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool ledState;
extern bool fanState;
extern bool pumpState;

String generateMainPage();
void handleRoot();
void setPinAndState(int pin, bool on);
void handleLedToggle();
void handleFanToggle();
void handlePumpToggle();
void handleStatus();
void handleNotFound();
void register_for_route_web_server();
void run_web_server();

#ifdef __cplusplus
}
#endif

#endif
