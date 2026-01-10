#include "Control_Relay.h"

void relay_init(relay_t *relay) {
    if(relay == NULL) return;

    pinMode(relay->pin, OUTPUT);

    if(relay->active_mode == RELAY_ACTIVE_LOW) {
        digitalWrite(relay->pin, HIGH);
    } else {
        digitalWrite(relay->pin, LOW);
    }
}

void relay_on(relay_t *relay) {
    if (relay == NULL) return;

    if (relay->active_mode == RELAY_ACTIVE_LOW) {
        digitalWrite(relay->pin, LOW);
    } else {
        digitalWrite(relay->pin, HIGH);
    } 
}

void relay_off(relay_t *relay) {
    if (relay == NULL) return;

    if (relay->active_mode == RELAY_ACTIVE_LOW) {
        digitalWrite(relay->pin, HIGH);
    } else {
        digitalWrite(relay->pin, LOW);
    }
}
