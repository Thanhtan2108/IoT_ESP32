#include <Arduino.h>
#include <Common_Config.h>
#include <Connect_Wifi.h>
#include <DHT11_Sensor.h>
#include <OLED_I2C.h>
#include <Send_Data_Firebase.h>

static float current_temperature = 0.0;
static float current_humidity = 0.0;
static unsigned long last_sensor_read_time = 0;
static unsigned long last_display_update_time = 0;

void manager_status_connect_wifi();
void read_data_sensor();
void update_display();

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n=== ESP32 Temperature & Humidity Monitor ===");
  // Initialize oled_i2c
  oled_init();

  // Initialize WiFi
  wifi_init();

  // Initialize Firebase (sau khi WiFi đã được khởi tạo)
  firebase_init();

  // Initialize dht_sensor
  dht_init();

  Serial.println("System initialization complete!");
}

void loop() {
  manager_status_connect_wifi();
  read_data_sensor();
  update_display();
}

void manager_status_connect_wifi() {
  pinMode(LED_STATUS_WIFI, OUTPUT);
  digitalWrite(LED_STATUS_WIFI, LOW);
  
  if (wifi_is_connected()) {
    digitalWrite(LED_STATUS_WIFI, HIGH);
  } else {
    digitalWrite(LED_STATUS_WIFI, LOW);
    wifi_reconnect();
  }
  delay(2000);
}

void read_data_sensor() {
  unsigned long current_time = millis();

  if (current_time - last_sensor_read_time >= SENSOR_READ_INTERVAL) {
    float temperature = dht_read_temperature();
    float humidity = dht_read_humidity();

    if (dht_is_valid_read(temperature, humidity)) {
      current_temperature = temperature;
      current_humidity = humidity;

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print("°C, Humidity: ");
      Serial.print(humidity);
      Serial.println("%");

      // Gửi dữ liệu lên Firebase (theo kiểu thủ tục C)
      firebase_send_sensor_data(current_temperature, current_humidity);
    } else {
      Serial.println("Invalid sensor reading!");
    }
        
    last_sensor_read_time = current_time;
  }
}

void update_display() {
  unsigned long current_time = millis();
  if (current_time - last_display_update_time >= 1000) { // Update every second
    if (dht_is_valid_read(current_temperature, current_humidity)) {
      oled_display_data(current_temperature, current_humidity, 0, 0);
    } else {
      oled_display_message("Invalid sensor reading!", 0, 0);
    }

    last_display_update_time = current_time; 
  }
}
