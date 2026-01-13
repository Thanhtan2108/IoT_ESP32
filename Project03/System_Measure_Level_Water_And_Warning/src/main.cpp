#include <Arduino.h>
#include "Common_Config.h"
#include "OLED_I2C.h"
#include "Control_Led_Single.h"
#include "Control_Buzzer.h"
#include "HC_SR04_Sensor.h"

float distanceCM;


void setup() {
  Serial.begin(115200);
  delay(100);

  oled_init();
  hc_sr04_sensor_init(TRIG_SR04_PIN, ECHO_SR04_PIN);
}

void loop() {
  distanceCM = hc_sr04_sensor_measureDistanceCM(TRIG_SR04_PIN, ECHO_SR04_PIN);
  oled_display_data(distanceCM, 0, 0);
  Serial.print("Distance: ");
  Serial.print(distanceCM);
  Serial.println(" cm");
  delay(500);
}
