
#include <Arduino.h>
#include "routineTest.h"
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

void setup()
{
   Serial.begin(115200);
   SETUP_RGB(255);
   pinMode(flushButton, INPUT_PULLUP);

   for (int i = 0; i < 4; i++)
   {
      pinMode(pinInDryContact[i], INPUT);
      pinMode(pinInWetContact[i], INPUT);
      pinMode(tests_buttons[i], INPUT_PULLUP);
      pinMode(pump[i], OUTPUT);
   }
   testRoutineInOut(true);
}

void loop()
{
   for (int i = 0; i < 5; i++)
   {
      Serial.print("Button");
      Serial.print(i + 1);
      if (digitalRead(tests_buttons[i]) == LOW)
      {
         Serial.print(" HIGH ");
         RGB_COLOR(RGB_GREEN);
      }
      else
      {
         Serial.print(" LOW ");
         RGB_COLOR(RGB_RED);
      }
   }
   Serial.println();
   delay(1000);
}