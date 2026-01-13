#include <Arduino.h>
#include "Common_Config.h"
#include "OLED_I2C.h"
#include "Control_Led_Single.h"
#include "Control_Buzzer.h"
#include "HC_SR04_Sensor.h"
#include "Connect_Wifi.h"
#include "Web_Server.h"

float distanceCM;

led_single_t led_warning {
  .pin = LED_WARNING
};

buzzer_t buzzer_warning {
  .pin = BUZZER_WARNING
};

void setup() {
  Serial.begin(115200);
  delay(100);

  led_single_init(&led_warning);
  buzzer_init(&buzzer_warning);
  oled_init();
  hc_sr04_sensor_init(TRIG_SR04_PIN, ECHO_SR04_PIN);
  wifi_init();
  register_for_route_web_server();
}

void loop() {
  distanceCM = hc_sr04_sensor_measureDistanceCM(TRIG_SR04_PIN, ECHO_SR04_PIN);
  if (distanceCM > 0 && distanceCM < 100) {
    oled_display_data(distanceCM, 0, 0);
    if (distanceCM <= threshold_cm) {
      led_single_on(&led_warning);
      buzzer_on(&buzzer_warning);
    } else {
      led_single_off(&led_warning);
      buzzer_off(&buzzer_warning);     
    }
  } else {
    oled_display_message("Distance Exceeded", 2, 0, 0);
  }
  delay(500);

  run_web_server();
}
