#include "Connect_Wifi.h"
#include "Common_Config.h"

static unsigned long last_reconnect_attempt = 0;
static const unsigned long RECONNECT_INTERVAL = 10000;

void wifi_init() {
    // Khởi tạo chế độ kết nối WiFi STA để kết nối WiFi được phát từ Router/Điện thoại
    WiFi.mode(WIFI_STA);

    // Kết nối vào mạng WiFi
    WiFi.begin(SSID_WIFI, PASSWORD_WIFI);

    // Tiến trình xử lý
    Serial.print("Connecting to WiFi");
    unsigned long start_time = millis();

    // Nếu chưa kết nối WiFi được
    while (WiFi.status() != WL_CONNECTED && millis() - start_time < RECONNECT_INTERVAL) {
        delay(500);
        Serial.print(".");
    }

    // Khi đã kết nối được 
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("Name WiFi connected: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi connection failed!");
    }
}

bool wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void wifi_reconnect() {
    unsigned long current_time = millis();
    
    if (!wifi_is_connected() && current_time - last_reconnect_attempt >= RECONNECT_INTERVAL) {
        Serial.println("Attempting WiFi reconnection...");
        WiFi.disconnect();
        WiFi.reconnect();
        last_reconnect_attempt = current_time;
    }
}
