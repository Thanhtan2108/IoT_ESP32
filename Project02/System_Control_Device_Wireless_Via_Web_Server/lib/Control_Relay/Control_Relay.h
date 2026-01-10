#ifndef CONTROL_RELAY_H
#define CONTROL_RELAY_H

#include "Common_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Kiểu kích relay */
typedef enum {
    RELAY_ACTIVE_LOW = 0,
    RELAY_ACTIVE_HIGH
} relay_active_mode_t;

/* Cấu trúc relay */
typedef struct {
    uint8_t pin;              // GPIO điều khiển
    relay_active_mode_t active_mode;    // Kiểu kích
} relay_t;

/* API */
void relay_init(relay_t *relay);
void relay_on(relay_t *relay);
void relay_off(relay_t *relay);

#ifdef __cplusplus
}
#endif

#endif
