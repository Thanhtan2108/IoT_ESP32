#include "Send_Data_Firebase.h"

#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <math.h>

#include "Connect_Wifi.h"

// ================================
// Biến Firebase toàn cục (dùng theo kiểu thủ tục C, không dùng class riêng)
// ================================

static FirebaseData fbdo;
static FirebaseAuth auth;
static FirebaseConfig config;

// Lưu giá trị cũ để so sánh và chỉ gửi khi có thay đổi
static float last_temperature = -999.0f;  // Giá trị khởi tạo không hợp lệ
static float last_humidity = -999.0f;      // Giá trị khởi tạo không hợp lệ

// ================================
// Hàm nội bộ
// ================================

// In ra lỗi Firebase nếu có
static void firebase_print_error()
{
    Serial.print("Firebase Error: ");
    Serial.println(fbdo.errorReason());
}

// ================================
// Hàm public (được khai báo trong .h)
// ================================

void firebase_init()
{
    Serial.println("\n[Firebase] Initializing...");

    // Cho phép tự động reconnect WiFi khi mất kết nối
    Firebase.reconnectWiFi(true);

    // Cấu hình API key & URL Realtime Database
    config.api_key = FIREBASE_API_KEY;
    config.database_url = FIREBASE_DATABASE_URL;

    // Cấu hình tài khoản user (Email/Password)
    auth.user.email = FIREBASE_USER_EMAIL;
    auth.user.password = FIREBASE_USER_PASSWORD;

    // Callback để in trạng thái token (có sẵn trong thư viện)
    config.token_status_callback = tokenStatusCallback;

    // Bắt đầu Firebase
    Firebase.begin(&config, &auth);

    // Đợi Firebase sẵn sàng một chút
    unsigned long start = millis();
    while (!Firebase.ready() && millis() - start < 10000UL)
    {
        Serial.print(".");
        delay(200);
    }
    Serial.println();

    if (Firebase.ready())
    {
        Serial.println("[Firebase] Initialized successfully!");
        Serial.print("[Firebase] Token: ");
        Serial.println(auth.token.uid.c_str());
    }
    else
    {
        Serial.println("[Firebase] Initialization failed!");
        Serial.print("[Firebase] Error: ");
        Serial.println(fbdo.errorReason());
    }
}

bool firebase_send_sensor_data(float temperature, float humidity)
{
    // Nếu Firebase chưa sẵn sàng hoặc WiFi chưa kết nối thì không gửi
    if (!Firebase.ready())
    {
        Serial.println("[Firebase] Not ready, skip sending.");
        return false;
    }

    if (!wifi_is_connected())
    {
        Serial.println("[Firebase] WiFi disconnected, skip sending.");
        return false;
    }

    // Kiểm tra xem dữ liệu có thay đổi không
    float temp_diff = fabs(temperature - last_temperature);
    float humi_diff = fabs(humidity - last_humidity);
    
    // Nếu giá trị cũ chưa được khởi tạo (lần đầu tiên) hoặc có thay đổi đáng kể
    bool has_changed = false;
    if (last_temperature < -900.0f || last_humidity < -900.0f)
    {
        // Lần đầu tiên, luôn gửi để khởi tạo giá trị trên Firebase
        has_changed = true;
        Serial.println("[Firebase] First time sending data, initializing Firebase...");
    }
    else if (temp_diff >= TEMPERATURE_THRESHOLD || humi_diff >= HUMIDITY_THRESHOLD)
    {
        // Có thay đổi đáng kể
        has_changed = true;
        Serial.print("[Firebase] Data changed: Temp diff=");
        Serial.print(temp_diff);
        Serial.print("°C, Humi diff=");
        Serial.print(humi_diff);
        Serial.println("%");
    }
    else
    {
        // Không có thay đổi đáng kể, không gửi
        return false;
    }

    // Tạo path trực tiếp: /sensorData/temperature và /sensorData/humidity
    char tempPath[128];
    char humiPath[128];
    snprintf(tempPath, sizeof(tempPath), "%s/temperature", FIREBASE_ROOT_PATH);
    snprintf(humiPath, sizeof(humiPath), "%s/humidity", FIREBASE_ROOT_PATH);

    Serial.print("[Firebase] Updating data at: ");
    Serial.println(FIREBASE_ROOT_PATH);

    // Gửi nhiệt độ
    bool ok_temp = Firebase.setFloat(fbdo, tempPath, temperature);
    if (!ok_temp)
    {
        Serial.print("[Firebase] Failed to send temperature. Error: ");
        Serial.println(fbdo.errorReason());
        Serial.print("[Firebase] Error code: ");
        Serial.println(fbdo.errorCode());
        return false;
    }

    // Gửi độ ẩm
    bool ok_humi = Firebase.setFloat(fbdo, humiPath, humidity);
    if (!ok_humi)
    {
        Serial.print("[Firebase] Failed to send humidity. Error: ");
        Serial.println(fbdo.errorReason());
        Serial.print("[Firebase] Error code: ");
        Serial.println(fbdo.errorCode());
        return false;
    }

    Serial.println("[Firebase] Data updated successfully!");

    // Lưu giá trị mới để so sánh lần sau
    last_temperature = temperature;
    last_humidity = humidity;
    
    return true;
}