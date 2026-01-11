#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

constexpr const char* SSID_WIFI = "Thanhtan";
constexpr const char* PASSWORD_WIFI = "12345678";

#define LED_PIN 2

#define RELAY_FAN_PIN 16
#define RELAY_PUMP_PIN 17

#define TIMEOUT_WEBSERVER 15000 // 15s

#ifdef __cplusplus
}
#endif

#endif
