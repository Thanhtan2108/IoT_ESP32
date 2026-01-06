#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include "DHT.h"

// select DHT sensor type
#define DHT_TYPE DHT11

void dht_init();
float dht_read_temperature();
float dht_read_humidity();
bool dht_is_valid_read(float temperature, float humidity);

#endif
