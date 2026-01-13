#ifndef CONNECT_WIFI_H
#define CONNECT_WIFI_H

#include <WiFi.h>

#ifdef __cplusplus
extern "C" {
#endif

void wifi_init();
bool wifi_is_connected();
void wifi_reconnect();

#ifdef __cplusplus
}
#endif

#endif