#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char* SSID_WIFI;
extern const char* PASSWORD_WIFI;

#define LED_WARNING 4

#define BUZZER_WARNING 13

#define ECHO_SR04_PIN 18
#define TRIG_SR04_PIN 19

#define SDA_PIN 21
#define SCL_PIN 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Threshold


#ifdef __cplusplus
}
#endif

#endif
