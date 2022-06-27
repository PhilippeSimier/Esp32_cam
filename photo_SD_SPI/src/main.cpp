/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard washington Le Mans
 *
 * Created on 25 juin 2022, 18:05
 */


#include <Arduino.h>
#include <Camera.h>


Camera *laCamera;

void setup() {
    Serial.begin(115200);

    pinMode(LED_ROUGE, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(16, OUTPUT); 
    
    digitalWrite(LED_ROUGE, LOW);

    laCamera = new Camera;

    if (!laCamera->init(PIXFORMAT_JPEG, FRAMESIZE_SVGA)) {
        Serial.println("Problème Camera ou carte SD");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("Setup done");
    digitalWrite(LED_ROUGE, HIGH);
}

void loop() {
    digitalWrite(LED_ROUGE, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(16, HIGH);
    delay(500);
    laCamera->capturePhotoSaveSD("photo");
    digitalWrite(LED_ROUGE, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(16, LOW);
    delay(2000);

    //laCamera->flash(1);
}

