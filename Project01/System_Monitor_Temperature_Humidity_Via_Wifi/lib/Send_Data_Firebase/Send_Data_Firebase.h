#ifndef SEND_DATA_FIREBASE_H
#define SEND_DATA_FIREBASE_H

#include <Arduino.h>
#include "Common_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo Firebase, gọi 1 lần trong setup() (sau khi WiFi đã kết nối)
void firebase_init();

// Gửi dữ liệu nhiệt độ và độ ẩm lên Firebase
// Trả về true nếu gửi thành công, false nếu lỗi / chưa sẵn sàng
bool firebase_send_sensor_data(float temperature, float humidity);

#ifdef __cplusplus
}
#endif

#endif
