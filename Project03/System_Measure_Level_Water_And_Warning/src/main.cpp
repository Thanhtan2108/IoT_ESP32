#include <Arduino.h>
#include "Common_Config.h"
#include "OLED_I2C.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  oled_init();
  oled_clear();
}

void loop() {
  for (float i = 0 ; i < 10.0 ; i++) {
    oled_display_data(i, 0, 20);
    delay(500);
    if (i == 9.0) {
      i = 0;
    }
  }
}