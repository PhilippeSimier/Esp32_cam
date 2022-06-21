/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 *
 * Created on 21 juin 2022, 13:33
 */

#include <Arduino.h>

#include "FS.h"     // SD Card ESP32
#include "SD_MMC.h" // SD Card ESP32

//fs::FS &fs = SD_MMC;

void setup() {

    Serial.begin(115200);

    if (!SD_MMC.begin()) {
        Serial.println("SD Card Mount Failed");
        return;
    }

    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD Card attached");
        return;
    }

    File file = SD_MMC.open("/photo", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file in writing mode");
    } else {

        Serial.printf("Open file with success");
        file.close();
    }

}

void loop() {

}



