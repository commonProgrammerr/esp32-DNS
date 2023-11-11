#ifndef __RGB_H__
#define __RGB_H__
#include <Arduino.h>
#include "Freenove_WS2812_Lib_for_ESP32.h" //biblioteca do led RGB do DEVKIT Solo

#define LEDS_COUNT 8
#define LEDS_PIN 18
#define CHANNEL 0

#define RGB_RED 255, 0, 0
#define RGB_BLACK 0, 0, 0
#define RGB_BLUE 0, 0, 255
#define RGB_GREEN 0, 255, 0
#define RGB_YELLOW 255, 255, 0
#define RGB_WHITE 255, 255, 255

extern Freenove_ESP32_WS2812 RGB;

#define SETUP_RGB(Brightness) \
  RGB.begin();                \
  RGB.setBrightness(Brightness)

#define RGB_COLOR(COLOR)         \
  delayMicroseconds(300);        \
  RGB.setLedColorData(0, COLOR); \
  RGB.show();                    \
  delayMicroseconds(300)

#define RGB_OFF() RGB_COLOR(RGB_BLACK)

#endif