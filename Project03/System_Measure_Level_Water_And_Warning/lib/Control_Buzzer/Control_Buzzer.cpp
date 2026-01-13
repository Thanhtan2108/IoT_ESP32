#include "Control_Buzzer.h"

void buzzer_init(buzzer_t *buzzer) {
    if (buzzer == NULL) return;

    pinMode(buzzer->pin, OUTPUT);
    digitalWrite(buzzer->pin, BUZZER_OFF);
}

int buzzer_read_level_logic(buzzer_t *buzzer) {
    if (buzzer == NULL) return -1;

    return digitalRead(buzzer->pin);
}

void buzzer_on(buzzer_t *buzzer) {
    if (buzzer == NULL) return;

    digitalWrite(buzzer->pin, BUZZER_ON);
}

void buzzer_off(buzzer_t *buzzer) {
    if (buzzer == NULL) return;

    digitalWrite(buzzer->pin, BUZZER_OFF);
}


