/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard washington Le Mans
 *
 * Created on 25 juin 2022, 18:05
 */


#include <Arduino.h>
#include <Camera.h>

Camera *laCamera;
int flipflop = 0;


void setup() {
    Serial.begin(115200);

    pinMode(LED_ROUGE, OUTPUT);
    digitalWrite(LED_ROUGE, LOW);

    laCamera = new Camera;

    if (!laCamera->init(PIXFORMAT_JPEG, FRAMESIZE_VGA)) {
        Serial.println("Camera init failed");
        while (1) {
            delay(1000);
        }
    }

    // laCamera->vflip();
    // laCamera->hmirror();
    // laCamera->setContrast(2);


    if (!laCamera->SDinit()) {
        Serial.println("SD card init failed");
        while (1) {
            delay(1000);
        }
    }

    Serial.println("Setup done");
    digitalWrite(LED_ROUGE, HIGH);
}

void loop() {
    digitalWrite(LED_ROUGE, LOW);
    
    

    if (flipflop == 0) {
        laCamera->reset();
        laCamera->init(PIXFORMAT_RGB565, FRAMESIZE_SVGA);

        flipflop = 1;
        Serial.println("RGB565 SVGA");
        
        if (laCamera->capturePhoto()) {
            laCamera->SaveSD("photo", "565");
            Serial.printf("len  : %d octets \n", laCamera->getLen());
            Serial.printf("size : %d x %d \n", laCamera->getwidth(), laCamera->getheight());
        }
        delay(1000);


    } else {
        laCamera->reset();
        laCamera->init(PIXFORMAT_JPEG, FRAMESIZE_VGA);
        laCamera->hmirror();
        
        flipflop = 0;
        Serial.println("JPEG VGA");
        
        if (laCamera->capturePhoto()) {
            laCamera->SaveSD("photo", "jpg");
            Serial.printf("len  : %d octets \n", laCamera->getLen());
            Serial.printf("size : %d x %d \n", laCamera->getwidth(), laCamera->getheight());
        }
        delay(1000);
    }

}

