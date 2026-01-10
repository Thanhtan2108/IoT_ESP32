#include <Arduino.h>
#include <Wire.h>
#include <Control_Led_Single.h>
#include "Control_Relay.h"

led_single_t led {
  .pin = LED_PIN
};

relay_t relayFan {
  .pin = RELAY_FAN_PIN,
  .active_mode = RELAY_ACTIVE_LOW
};

relay_t relayPump {
  .pin = RELAY_PUMP_PIN,
  .active_mode = RELAY_ACTIVE_HIGH
};

void setup() {
  Serial.begin(115200);
  delay(100);

  led_single_init(&led);

  relay_init(&relayFan);
  relay_init(&relayPump);
}

void loop() {
  led_single_on(&led);
  Serial.println("LED ON");
  delay(2000);

  for (int i = 0 ; i < 5 ; i ++) {
    Serial.printf("LED blink time: %d\n", i + 1);
    led_single_blink(&led, 500);
  }
  led_single_off(&led);
  Serial.println("Finish");
  delay(1000);

  relay_on(&relayFan);
  Serial.println("FAN on");
  relay_off(&relayPump);
  Serial.println("PUMP off");
  delay(3000);

  relay_off(&relayFan);
  Serial.println("FAN off");
  relay_on(&relayPump);
  Serial.println("PUMP on");
  delay(3000);
}
