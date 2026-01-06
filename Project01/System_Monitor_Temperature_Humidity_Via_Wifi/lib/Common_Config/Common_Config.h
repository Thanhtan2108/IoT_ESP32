#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

extern const char* SSID_WIFI;
extern const char* PASSWORD_WIFI;

/*
Thay vì dùng extern + .cpp, dùng từ khóa constexpr để không cần tạo .cpp
constexpr const char* SSID_WIFI = "Thanhtan";
constexpr const char* PASSWORD_WIFI = "12345678";
*/

#define LED_STATUS_WIFI 2

#define DHT_PIN 4

#endif
