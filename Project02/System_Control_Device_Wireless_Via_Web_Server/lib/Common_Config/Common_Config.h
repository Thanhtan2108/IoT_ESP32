#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

constexpr const char* SSID_WIFI = "Thanhtan";
constexpr const char* PASSWORD_WIFI = "12345678";

// ================================
// Cấu hình Firebase dùng chung cho toàn firmware
// Link Firebase Realtime Database : 
// GG Account: nthanhtan2004@gmail.com
// ================================

// extern const char* FIREBASE_API_KEY;
// extern const char* FIREBASE_DATABASE_URL;
// extern const char* FIREBASE_USER_EMAIL;
// extern const char* FIREBASE_USER_PASSWORD;

// // Đường dẫn gốc để lưu dữ liệu trên Realtime Database
// #define FIREBASE_ROOT_PATH         "/"

#define LED_PIN 2

#define RELAY_FAN_PIN 16
#define RELAY_PUMP_PIN 17

#define TIMEOUT_WEBSERVER 15000 // 15s

#ifdef __cplusplus
}
#endif

#endif
