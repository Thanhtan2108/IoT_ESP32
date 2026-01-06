#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include <WiFi.h>

void wifi_init();
bool wifi_is_connected();
void wifi_reconnect();

#endif