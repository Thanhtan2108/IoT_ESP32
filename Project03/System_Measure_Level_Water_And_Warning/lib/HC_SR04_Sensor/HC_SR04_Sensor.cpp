#include "HC_SR04_Sensor.h"

long duration, distance;

void hc_sr04_sensor_init(uint8_t trig_pin, uint8_t echo_pin) {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

float hc_sr04_sensor_measureDistanceCM(uint8_t trig_pin, uint8_t echo_pin) {
    // Make sure that trigger pin is LOW.
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    duration = pulseIn(echo_pin, HIGH);
    distance = duration / 58.2;
    
    if (distance < 0) return -1;

    return distance;
}
