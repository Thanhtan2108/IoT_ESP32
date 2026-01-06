#include <Arduino.h>
#include <Common_Config.h>
#include <Connect_Wifi.h>
#include <DHT11_Sensor.h>

static unsigned long last_sensor_read_time = 0;

void manager_status_connect_wifi();
void read_data_sensor();

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n=== ESP32 Temperature & Humidity Monitor ===");
    
  // Initialize WiFi
  wifi_init();

  // Initialize dht_sensor
  dht_init();

  Serial.println("System initialization complete!");
}

void loop() {
  manager_status_connect_wifi();
  read_data_sensor();
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
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print("°C, Humidity: ");
      Serial.print(humidity);
      Serial.println("%");
    } else {
      Serial.println("Invalid sensor reading!");
    }
        
    last_sensor_read_time = current_time;
  }
}
