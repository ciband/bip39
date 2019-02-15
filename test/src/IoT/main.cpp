#if (defined PLATFORMIO && defined UNIT_TEST)

#include "gtest/gtest.h"

#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    while (!Serial) { delay(100); };
    // ^for the Arduino Leonardo/Micro only

    testing::InitGoogleTest();

    delay(1000);
}

void loop() { RUN_ALL_TESTS(); }

#endif
