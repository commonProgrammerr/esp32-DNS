
#include <Arduino.h>
// #include "Freenove_WS2812_Lib_for_ESP32.h"
/*
- implementar led de status + buzzer
   - led verde + buzzer se RTC ok; duração 1s
   - led vermelho + buzzer se falhar RTC; duração 1s
   - apagar led e buzzer
   - passar pra teste de imput humano
- teste de input humano
   - regra dos 3 segundos (esperar intervenção humana)
   - entradas 1 a 4 (3 segundos cada)
      - passar p proximo sempre que imput = ok
      - esperar 3 segundos pra falhar
   - upload to server
      - led azul se ok; duração 1s
      - led vermelho se erroç duração 1s
- adicionar quinta saida (flush)
  - teste de multiplas entradas
*/
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
   RGB.begin();     \
   RGB.setBrightness(255)

#define RGB_COLOR(COLOR)          \
   RGB.setLedColorData(0, COLOR); \
   RGB.show()

#define RGB_OFF() RGB_COLOR((0, 0, 0))

void setup()
{
   Serial.begin(115200);
   SETUP_RGB();
}

void loop()
{
   RGB_COLOR(RGB_WHITE);
   delay(1000);
   RGB_OFF();
   delay(1000);
}