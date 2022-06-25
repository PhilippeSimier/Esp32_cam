/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard Washington le Mans
 *
 * Created on 21 juin 2022, 11:35
 * 
 * Take a photo and save it on the SD card
 * 
 */

#include <Arduino.h>
#include <Camera.h>


Camera *laCamera;

void setup() {
    Serial.begin(115200);
    
    
    pinMode(LED_ROUGE, OUTPUT);
    digitalWrite(LED_ROUGE, LOW);
    
    
    laCamera = new Camera;
    if (!laCamera->init()){
        Serial.println("Problème Camera ou carte SD");
        while(1){
            delay(1000);
        }
    }
    Serial.println("Setup done");
    digitalWrite(LED_ROUGE, HIGH);
}

void loop() {
    digitalWrite(LED_ROUGE, LOW);
    delay(500);
    laCamera->capturePhotoSaveSD("photo");
    digitalWrite(LED_ROUGE, HIGH);
    delay(2000);
    
    //laCamera->flash(1);
}

