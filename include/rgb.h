#ifndef __rgb_h__
#define __rgb_h__
#include <Arduino.h>
#include "Freenove_WS2812_Lib_for_ESP32.h" //biblioteca do led RGB do DEVKIT Solo

#define LEDS_COUNT 8
#define LEDS_PIN 18
#define CHANNEL 0

#define RGB_RED 255, 0, 0
#define RGB_BLUE 0, 0, 255
#define RGB_GREEN 0, 255, 0
#define RGB_WHITE 255, 255, 255

Freenove_ESP32_WS2812 RGB = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

#define SETUP_RGB() \
  RGB.begin();      \
  RGB.setBrightness(255)

#define RGB_COLOR(COLOR)         \
  RGB.setLedColorData(0, COLOR); \
  RGB.show()

#define RGB_OFF() RGB_COLOR((0, 0, 0))
#endif