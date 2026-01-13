#include <Arduino.h>
#include "Common_Config.h"
#include "OLED_I2C.h"
#include "Control_Led_Single.h"
#include "Control_Buzzer.h"

led_single_t led_warning {
  .pin = LED_WARNING
};

buzzer_t buzzer_warning {
  .pin = BUZZER_WARNING
};

void setup() {
  Serial.begin(115200);
  delay(100);

  buzzer_init(&buzzer_warning);
}

void loop() {
  buzzer_on(&buzzer_warning);
  Serial.println(buzzer_read_level_logic(&buzzer_warning));
  delay(2000);
  buzzer_off(&buzzer_warning);
  Serial.println(buzzer_read_level_logic(&buzzer_warning));
  delay(3000);
}
