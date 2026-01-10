#include "Control_Led_Single.h"

void led_single_init(led_single_t *led_single) {
    if (led_single == NULL) return;

    pinMode(led_single->pin, OUTPUT);
    digitalWrite(led_single->pin, LED_OFF);
}

int led_single_read_level_logic(led_single_t *led_single) {
    if (led_single == NULL) return -1;

    return digitalRead(led_single->pin);
}

void led_single_on(led_single_t *led_single) {
    if (led_single == NULL) return;

    digitalWrite(led_single->pin, LED_ON);
}

void led_single_off(led_single_t *led_single) {
    if (led_single == NULL) return;

    digitalWrite(led_single->pin, LED_OFF);
}

void led_single_blink(led_single_t *led_single, uint32_t time_ms) {
    if (led_single == NULL) return;

    digitalWrite(led_single->pin, LED_ON);
    Serial.println(led_single_read_level_logic(led_single));
    delay(time_ms);
    digitalWrite(led_single->pin, LED_OFF);
    Serial.println(led_single_read_level_logic(led_single));
    delay(time_ms);
}
