#ifndef HC_SR04_SENSOR_H
#define HC_SR04_SENSOR_H

#include "Common_Config.h"

void hc_sr04_sensor_init(uint8_t trig_pin, uint8_t echo_pin);
float hc_sr04_sensor_measureDistanceCM(uint8_t trig_pin, uint8_t echo_pin);

#endif
