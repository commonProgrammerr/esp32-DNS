
#ifndef __routine_test_h__
#define __routine_test_h__
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include "rgb.h"
#include "RTC.h"

#define buttonPin1 5  // 4 // 11
#define buttonPin2 6  // 5 // 10
#define buttonPin3 7  // 6 // 3
#define buttonPin4 15 // 7 // 20
#define flushButton 4 // 15 // 12

#define flushButton 4  // 15 // 12
#define flushOutput 13 // 9 // 21

// entradas de contato seco
#define inputPinDryContact1 19 // 7
#define inputPinDryContact2 20 // 15
#define inputPinDryContact3 40 // 16
#define inputPinDryContact4 41 // 19

// entradas de contato em tensão
#define inputPinWetContact1 19 // 7
#define inputPinWetContact2 20 // 15
#define inputPinWetContact3 40 // 16
#define inputPinWetContact4 41 // 19

#define pump1 12 // 13 // 1
#define pump2 11 // 12 // 2
#define pump3 10 // 11 // 38
#define pump4 9  // 10 // 37

#define buzzer 14 // 14
#define buzzerCh 0

#define ssidWifiTestRoutine "GVT-8D59-2G"
#define passWifiTestRoutine "Arer3366547@"
#define serverTest "http://192.168.1.38:3000/api/log"

extern byte tests_buttons[];
extern byte pinInDryContact[];
extern byte pinInWetContact[];
extern byte pump[];

// typedef struct
// {
//   bool (*test)(void *args);
//   void *parms;
//   const char *test_name;
// } unit_test_t;

bool input_check(byte port, byte expect);
bool await_click(uint8_t pin, int value, uint32_t timeout);
void testRoutineInOut(bool testRoutineFlag);
#endif