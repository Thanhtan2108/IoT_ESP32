#ifndef OLED_I2C_H
#define OLED_I2C_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifdef __cplusplus
extern "C" {
#endif

void oled_init();
void oled_display_data(float temperature, float humidity, uint8_t row, uint8_t col);
void oled_display_message(const char* message, uint8_t row, uint8_t col);
void oled_clear();

#ifdef __cplusplus
}
#endif

#endif
