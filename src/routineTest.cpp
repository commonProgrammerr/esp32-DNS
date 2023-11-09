#include "routineTest.h"
// check expected routine
byte tests_buttons[] = {buttonPin1, buttonPin2, buttonPin3, buttonPin4, flushButton};

bool input_check(byte port, byte expect) { return digitalRead(port) == expect; }

bool await_click(uint8_t pin, int value, uint32_t timeout = 3000UL)
{
    for (uint32_t start_time = millis(); (millis() - start_time) < timeout;)
        if (digitalRead(pin) == value)
            return true;
        else
            delay(1);
    return false;
}

void testRoutineInOut(bool testRoutineFlag)
{
    if (testRoutineFlag)
    {
        bool testResult = false;
        bool testFail = false;
        String payload = "";
        Serial.println();
        Serial.println("ENTROU NA ROTINA DE TESTE");
        WiFi.begin(ssidWifiTestRoutine, passWifiTestRoutine);
        Serial.println("Connecting");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());

        int pass_count = 0;
        // const int totalTests = static_cast<int>(tests.size);
        const int totalPumps = 4;
        const int totalButtons = 4;
        const int totalTests = totalPumps + totalButtons; //
        String test_log = "";

        String dateTimeTest = "29-11-2021 20:32:00";             // data apenas pra teste do RTC
        setRTCTime(0,                                            // segundo
                   dateTimeTest.substring(14, 16).toInt(),       // minuto
                   (dateTimeTest.substring(11, 13).toInt() - 3), // hora (-3 por causa de Greenwich)
                   0,                                            // dia da semana
                   dateTimeTest.substring(0, 2).toInt(),         // data
                   dateTimeTest.substring(3, 5).toInt(),         // mes
                   dateTimeTest.substring(8, 10).toInt());       // ano
        Serial.print("Actual dateTime RTC-esp == ");             //+"code"
        Serial.println(getTimeStamp().substring(0, 16));

        Serial.print("Starting tests routine: ");
        Serial.println(totalTests);

        testResult = dateTimeTest.substring(0, 10).equals(getTimeStamp().substring(0, 10));
        test_log = (test_log + "RTC:") + (testResult ? "OK " : "FAIL ");

        if (testResult)
        {
            pass_count++;
            RGB_COLOR(RGB_GREEN);
            toneBuzzer(buzzerCh, 2000, 1000, 1);
        }
        else
        {
            testFail = true;
            RGB_COLOR(RGB_RED);
            toneBuzzer(buzzerCh, 2000, 15, 20); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
        }
        delay(1000);
        RGB_OFF();

        for (int i = 0; i < toot; i++)
        {
            bool teste_result = await_click(button[i], HIGH);
            if (testResult)
            {
                pass_count++;
                RGB_COLOR(RGB_GREEN);
            }
            else
            {
                testFail = true;
                RGB_COLOR(RGB_RED);
                toneBuzzer(buzzerCh, 2000, 15, 20); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
            }
            test_log = ((test_log + "Button") + (i + 1)) + (testResult ? ":OK " : ":FAIL ");
            delay(1000);
            RGB_OFF();
        }

        for (int i = 0; i < totalPumps; i++)
        {
            digitalWrite(pump[i], HIGH);
            delay(100);
            testResult = input_check(pinInWetContact[i], HIGH);
            digitalWrite(pump[i], LOW);

            if (testResult)
                pass_count++;
            else
            {
                testFail = true;
                toneBuzzer(buzzerCh, 2000, 15, 20); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
            }
            test_log = ((test_log + "In-Out") + (i + 1)) + (testResult ? ":OK " : ":FAIL ");
        }

        Serial.println(test_log);

        HTTPClient http;
        http.begin(serverTest);
        http.addHeader("test_number", String(totalTests));
        http.addHeader("pass_number", String(pass_count));

        String mac = WiFi.macAddress();
        mac.replace(":", "");
        http.addHeader("mac", mac);

        int httpResponseCode = http.POST(test_log);
        yield();
        if (httpResponseCode > 0)
        {
            RGB_COLOR(RGB_RED);
            payload = http.getString();
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
            Serial.println(
                http.addHeader("test_number", String(totalTests));
                http.addHeader("pass_number", String(pass_count)); getTimeStamp().substring(0, 16));
            Serial.print("RTC-Server === "); //+"code"
            Serial.println(payload.substring(0, 16));
            Serial.print("HTTP "); //+"code"

            if (!testFail)
                toneBuzzer(buzzerCh, 1000, 50, 3);
        }
        else
        {
            RGB_COLOR(RGB_BLUE);
            Serial.println();
            Serial.print("HTTP Error ");        //+"code"
            toneBuzzer(buzzerCh, 2000, 15, 20); // toneBuzzer(byte buzzer, int frequency, int bzDelay, int numberOfRepetitions)
        }
        delay(1000);
        RGB_OFF();
        Serial.print("code: ");
        Serial.println(httpResponseCode);
        http.end();

        while (1)
            ;
    }
}
