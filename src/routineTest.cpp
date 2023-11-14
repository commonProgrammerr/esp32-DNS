#include "routineTest.h"
// check expected routine

void toneBuzzer(byte bzChannel, int frequency, int bzDelay, int numberOfRepetitions)
{
    for (int i = 0; i < numberOfRepetitions; i++)
    {                                        // toca o buzzer x vezes
        ledcWriteTone(bzChannel, frequency); // tone(buzzer, frequency);//função do arduino
        delay(bzDelay);
        ledcWriteTone(bzChannel, 0); // noTone(bz);//função do arduino
        delay(bzDelay);
    }
}

byte pinInDryContact[] = {inputPinDryContact1, inputPinDryContact2, inputPinDryContact3, inputPinDryContact4};
byte pinInWetContact[] = {inputPinWetContact1, inputPinWetContact2, inputPinWetContact3, inputPinWetContact4};
byte pump[] = {pump1, pump2, pump3, pump4};
Freenove_ESP32_WS2812 RGB = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

byte tests_buttons[] = {flushButton, buttonPin1, buttonPin2, buttonPin3, buttonPin4};
const int totalPumps = 4;
const int totalButtons = 5;
const int totalTests = 1 + totalPumps + totalButtons; //

bool input_check(byte port, byte expect) { return digitalRead(port) == expect; }

bool await_click(uint8_t pin, int value, uint32_t timeout = 4000UL)
{
    uint32_t start_time = millis();
    while ((millis() - start_time) <= timeout)
    {
        if (input_check(pin, value))
            return true;
        RGB_COLOR(RGB_WHITE);
        delay(100);
        RGB_OFF();
        delay(100);
    }
    return false;
}

bool testRTC()
{
    // data apenas pra teste do RTC
    setRTCTime(0,   // segundo
               32,  // minuto
               20,  // hora
               0,   // dia da semana
               29,  // data
               11,  // mes
               21); // ano

    Serial.print("Actual dateTime RTC-esp == "); //+"code"
    Serial.println(getTimeStamp().substring(0, 16));

    bool isEqual = getTimeStamp().substring(0, 16).equals("29-11-2021 20:32");

    if (isEqual)
    {
        RGB_COLOR(RGB_PINK);
        toneBuzzer(OK_SOUND);
    }
    else
    {
        RGB_COLOR(RGB_RED);
        toneBuzzer(FAIL_SOUND);
    }
    delay(500);
    RGB_OFF();
    return isEqual;
}

bool testButton(byte button, byte value)
{
    bool teste_result = await_click(button, value);
    if (teste_result)
    {
        RGB_COLOR(RGB_GREEN);
        toneBuzzer(OK_SOUND); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
    }
    else
    {
        RGB_COLOR(RGB_RED);
        toneBuzzer(FAIL_SOUND); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
    }
    RGB_OFF();

    return teste_result;
}

bool testInOut(byte in_pin, byte out_pin)
{
    digitalWrite(out_pin, HIGH);
    delay(100);
    bool testResult = input_check(in_pin, HIGH);
    digitalWrite(out_pin, LOW);

    if (testResult)
    {
        RGB_COLOR(RGB_GREEN);
        toneBuzzer(OK_SOUND); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
    }
    else
    {
        RGB_COLOR(RGB_RED);
        toneBuzzer(FAIL_SOUND); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
    }
    delay(300);
    return testResult;
}

void testRoutineInOut(bool testRoutineFlag)
{
    if (testRoutineFlag)
    {
        byte failCount = 0;
        Serial.println();
        Serial.println("ENTROU NA ROTINA DE TESTE");
        WiFi.begin(ssidWifiTestRoutine, passWifiTestRoutine);
        Serial.println("Connecting");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            RGB_COLOR(RGB_BLUE);
            delay(250);
            RGB_OFF();
            delay(250);
        }
        RGB_COLOR(RGB_BLUE);
        toneBuzzer(OK_SOUND);
        delay(500);
        RGB_OFF();
        delay(1000);

        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());

        int pass_count = 0;
        String test_log = "";

        Serial.print("Starting tests routine: ");
        Serial.println(totalTests);

        test_log += "RTC:";
        if (testRTC())
        {
            pass_count++;
            test_log += "OK";
        }
        else
        {
            failCount++;
            test_log += "FAIL";
        }

        Serial.println("Startin human input test!");
        for (int i = 0; i < totalButtons; i++)
        {
            test_log += " Button";
            test_log += (i + 1);
            if (testButton(tests_buttons[i], LOW))
            {
                pass_count++;
                test_log += ":OK";
            }
            else
            {
                test_log += ":FAIL";
                failCount++;
            }
        }

        for (int i = 0; i < totalPumps; i++)
        {
            test_log += " In-Out";
            test_log += (i + 1);

            if (testInOut(pinInWetContact[i], pump[i]))
            {
                test_log += ":OK";
                pass_count++;
            }
            else
            {
                test_log += ":FAIL";
                failCount++;
            }
        }

        Serial.println(test_log);

        HTTPClient http;
        RGB_COLOR(RGB_BLUE);
        http.begin(serverTest);
        http.addHeader("test_number", String(totalTests));
        http.addHeader("pass_number", String(pass_count));

        String mac = WiFi.macAddress();
        mac.replace(":", "");
        http.addHeader("mac", mac);

        int httpResponseCode = http.POST(test_log);
        yield();
        if (httpResponseCode == 200)
        {
            String payload = http.getString();
            Serial.print("HTTP Response: ");
            // payload = getTimeStamp(); // remover essa linha assim que o servidor estiver respondendo a hr
            Serial.println(payload);
            setRTCTime(0,                                       // segundo
                       payload.substring(14, 16).toInt(),       // minuto
                       (payload.substring(11, 13).toInt() - 3), // hora (-3 por causa de Greenwich)
                       0,                                       // dia da semana
                       payload.substring(0, 2).toInt(),         // data
                       payload.substring(3, 5).toInt(),         // mes
                       payload.substring(8, 10).toInt());       // ano

            Serial.print("RTC-esp ====== "); //+"code"
            Serial.println(getTimeStamp().substring(0, 16));
            Serial.print("RTC-Server === "); //+"code"
            Serial.println(payload.substring(0, 16));
            Serial.print("HTTP "); //+"code"
            toneBuzzer(OK_SOUND);  // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
        }
        else
        {
            RGB_COLOR(RGB_RED);
            Serial.println();
            Serial.print("HTTP Error "); //+"code"
            toneBuzzer(FAIL_SOUND);      // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
        }
        Serial.print("code: ");
        Serial.println(httpResponseCode);
        http.end();
        delay(1000);
        RGB_OFF();

        if (!failCount)
        {
            toneBuzzer(OK_SOUND);
            RGB_COLOR(RGB_GREEN);
        }
        else
        {
            toneBuzzer(FAIL_SOUND);
            RGB_COLOR(RGB_RED);
        }

        while (1)
            ;
    }
}
