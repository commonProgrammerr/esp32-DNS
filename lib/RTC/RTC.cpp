#include "RTC.h"

String getTimeStamp()
{
    byte Second, Minute, Hour, Day, Date, Month, Year;
    readRTCTime(&Second, &Minute, &Hour, &Day, &Date, &Month, &Year);

    //"DD-MM-YYYY HH:mm:ss"
    String timeStamp;
    if (Date < 10)
        timeStamp += "0";
    timeStamp += (String)Date;
    timeStamp += "-";

    if (Month < 10)
        timeStamp += "0";
    timeStamp += (String)Month;
    timeStamp += "-";

    timeStamp += "20"; // alterar isso para que seja uma variavel alteravel pelo usuario
    if (Year < 10)
        timeStamp += "0";
    timeStamp += (String)Year;
    timeStamp += " ";

    if (Hour < 10)
        timeStamp += "0";
    timeStamp += (String)Hour;
    timeStamp += ":";
    if (Minute < 10)
        timeStamp += "0";
    timeStamp += (String)Minute;
    timeStamp += ":";
    if (Second < 10)
        timeStamp += "0";
    timeStamp += (String)Second;

    return timeStamp;

    // Serial.print("   Day of the week: ");
    // switch (Day)
    // {
    // case 1:
    //     Serial.println("Monday");
    //     break;
    // case 2:
    //     Serial.println("Tuesday");
    //     break;
    // case 3:
    //     Serial.println("Wednesday");
    //     break;
    // case 4:
    //     Serial.println("Thursday");
    //     break;
    // case 5:
    //     Serial.println("Friday");
    //     break;
    // case 6:
    //     Serial.println("Saturday");
    //     break;
    // case 7:
    //     Serial.println("Sunday");
    //     break;
    // }
}

// Read the RTC Time Registers                     //30
void readRTCTime(byte *Second, byte *Minute, byte *Hour, byte *Day, byte *Date, byte *Month, byte *Year)
{
    Wire.begin(I2C_SDA, I2C_SCL);

    Wire.beginTransmission(DS3231RTC_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(DS3231RTC_I2C_ADDRESS, 7);
    *Second = bcdToDec(Wire.read());
    *Minute = bcdToDec(Wire.read());
    *Hour = bcdToDec(Wire.read());
    *Day = bcdToDec(Wire.read());
    *Date = bcdToDec(Wire.read());
    *Month = bcdToDec(Wire.read());
    *Year = bcdToDec(Wire.read());
    Wire.end();
}

// Set the RTC Time Registers
void setRTCTime(byte Second, byte Minute, byte Hour, byte Day, byte Date, byte Month, byte Year)
{
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.beginTransmission(DS3231RTC_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(decToBCD(Second));
    Wire.write(decToBCD(Minute));
    Wire.write(decToBCD(Hour));
    Wire.write(decToBCD(Day));
    Wire.write(decToBCD(Date));
    Wire.write(decToBCD(Month));
    Wire.write(decToBCD(Year));
    Wire.endTransmission();
    Wire.end();
}

// Convert from decimal to binary coded decimal
byte decToBCD(byte val)
{ // 16
    return (((val / 10) << 4) + val % 10);
}

// Convert from binary coded decimal to decimal
byte bcdToDec(byte val)
{ // 17
    return (10 * (val >> 4) + val % 16);
}
