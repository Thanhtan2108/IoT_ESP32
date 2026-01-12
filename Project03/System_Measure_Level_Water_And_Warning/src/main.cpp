#include <Arduino.h>
#include "Common_Config.h"
#include "OLED_I2C.h"
#include "Control_Led_Single.h"

led_single_t led_warning {
  .pin = LED_WARNING
};

void setup() {
  Serial.begin(115200);
  delay(100);

  led_single_init(&led_warning);
}

void loop() {
  led_single_blink(&led_warning, 500);
}