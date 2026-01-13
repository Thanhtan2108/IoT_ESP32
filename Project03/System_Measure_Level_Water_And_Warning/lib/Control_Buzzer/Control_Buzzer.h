#ifndef CONTROL_BUZZER_H
#define CONTROL_BUZZER_H

#include "Common_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BUZZER_OFF = 0,
    BUZZER_ON
} buzzer_status_t;

typedef struct {
    uint8_t pin;
} buzzer_t;

void buzzer_init(buzzer_t *buzzer);
void buzzer_on(buzzer_t *buzzer);
void buzzer_off(buzzer_t *buzzer);
int buzzer_read_level_logic(buzzer_t *buzzer);

#ifdef __cplusplus
}
#endif

#endif
