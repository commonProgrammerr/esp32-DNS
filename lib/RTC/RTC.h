#ifndef __RTC_H_
#define __RTC_H_

#include <Arduino.h>
#include <Wire.h>

#define DS3231RTC_I2C_ADDRESS 0x68
#define I2C_SDA 35
#define I2C_SCL 37

String getTimeStamp();

// Read the RTC Time Registers                     //30
void readRTCTime(byte *Second, byte *Minute, byte *Hour, byte *Day, byte *Date, byte *Month, byte *Year);

// Set the RTC Time Registers
void setRTCTime(byte Second, byte Minute, byte Hour, byte Day, byte Date, byte Month, byte Year);
// Convert from decimal to binary coded decimal
byte decToBCD(byte val);

// Convert from binary coded decimal to decimal
byte bcdToDec(byte val);

#endif