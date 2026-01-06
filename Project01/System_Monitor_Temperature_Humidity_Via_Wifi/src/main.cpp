#include <Arduino.h>
#include <Common_Config.h>
#include <Connect_Wifi.h>


void manager_status_connect_wifi();

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(LED_STATUS_WIFI, OUTPUT);
  digitalWrite(LED_STATUS_WIFI, LOW);

  Serial.println("\n=== ESP32 Temperature & Humidity Monitor ===");
    
  // Initialize WiFi
  wifi_init();

  Serial.println("System initialization complete!");
}

void loop() {
  manager_status_connect_wifi();
}

void manager_status_connect_wifi() {
  if (wifi_is_connected()) {
    digitalWrite(LED_STATUS_WIFI, HIGH);
  } else {
    digitalWrite(LED_STATUS_WIFI, LOW);
    wifi_reconnect();
  }
  delay(2000);
}
