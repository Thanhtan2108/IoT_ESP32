#include "DHT11_Sensor.h"
#include "Common_Config.h"

// initial DHT object from DHT Class
static DHT dht_sensor(DHT_PIN, DHT_TYPE);

void dht_init() {
    dht_sensor.begin();
    Serial.println("DHT11 sensor initialize");
}

float dht_read_temperature() {
    float temperature = dht_sensor.readTemperature();

    if (isnan(temperature)) {
        Serial.println("Failed to read temperature from DHT11!");
        return -999.0;
    }

    return temperature;
}

float dht_read_humidity() {
    float humidity = dht_sensor.readHumidity();

    if (isnan(humidity)) {
        Serial.println("Failed to read humidity from DHT11!");
        return -999.0;
    }

    return humidity;
}

bool dht_is_valid_read(float temperarure, float humidity) {
    return !(isnan(temperarure) || isnan(humidity) || temperarure < -40.0 || temperarure > 80.0 || humidity < 0 || humidity > 100); // true
}
