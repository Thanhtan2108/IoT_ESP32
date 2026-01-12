#ifndef CONTROL_LED_SINGLE_H
#define CONTROL_LED_SINGLE_H

#include "Common_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LED_OFF = 0,
    LED_ON
} led_status_t;

typedef struct {
    uint8_t pin;
} led_single_t;

void led_single_init(led_single_t *led_single);
void led_single_on(led_single_t *led_single);
void led_single_off(led_single_t *led_single);
void led_single_blink(led_single_t *led_single, uint32_t time_ms);
int led_single_read_level_logic(led_single_t *led_single);

#ifdef __cplusplus
}
#endif

#endif
