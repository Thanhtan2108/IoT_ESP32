#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char* SSID_WIFI;
extern const char* PASSWORD_WIFI;

/*
Thay vì dùng extern + .cpp, dùng từ khóa constexpr để không cần tạo .cpp
constexpr const char* SSID_WIFI = "Thanhtan";
constexpr const char* PASSWORD_WIFI = "12345678";
*/

// ================================
// Cấu hình Firebase dùng chung cho toàn firmware
// Link Firebase Realtime Database : https://console.firebase.google.com/project/system-monitor-temp-and-humi/database/system-monitor-temp-and-humi-default-rtdb/data
// GG Account: nthanhtan2004@gmail.com
// ================================

extern const char* FIREBASE_API_KEY;
extern const char* FIREBASE_DATABASE_URL;
extern const char* FIREBASE_USER_EMAIL;
extern const char* FIREBASE_USER_PASSWORD;

// Đường dẫn gốc để lưu dữ liệu trên Realtime Database
#define FIREBASE_ROOT_PATH         "/sensorData"

#define LED_STATUS_WIFI 2

#define DHT_PIN 4

#define SDA_PIN 21
#define SCL_PIN 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Timeout
#define SENSOR_READ_INTERVAL 2000UL  // 2 giây
#define FIREBASE_SEND_INTERVAL_MS  5000UL // 5 giây

// Threshold
// Ngưỡng thay đổi tối thiểu để coi là "có thay đổi" (tránh nhiễu cảm biến)
#define TEMPERATURE_THRESHOLD 0.1f  // 0.1°C
#define HUMIDITY_THRESHOLD 0.1f     // 0.1%

#ifdef __cplusplus
}
#endif

#endif
