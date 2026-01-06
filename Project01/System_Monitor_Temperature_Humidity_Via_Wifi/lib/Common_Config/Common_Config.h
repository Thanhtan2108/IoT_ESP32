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

#define SDA_PIN 21
#define SCL_PIN 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Timeout
#define SENSOR_READ_INTERVAL 2000UL  // 2 giây

#endif
