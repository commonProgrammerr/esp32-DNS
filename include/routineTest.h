
#ifndef __routine_test_h__
#define __routine_test_h__
#include <Arduino.h>
#include "rgb.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "rtc_clk_common.h"

#define buttonPin1 5  // 4 // 11
#define buttonPin2 6  // 5 // 10
#define buttonPin3 7  // 6 // 3
#define buttonPin4 15 // 7 // 20
#define flushButton 4 // 15 // 12

bool input_check(byte port, byte expect);
bool await_click(uint8_t pin, int value, uint32_t timeout);
void testRoutineInOut(bool testRoutineFlag);
#endif