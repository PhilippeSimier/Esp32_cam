/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard Washington
 *
 * Created on 21 juin 2022, 08:50
 * 
 * test program for
 * Turn ON and OFF the Flash led on an ESP32-CAM board
 * 
 * Pour tester la liaison série il faut nécessairement de désativer dtr et rts
 * voir la doc de https://docs.platformio.org/en/latest/projectconf/section_env_monitor.html#monitor-rts
 * 
 * pio device monitor -p /dev/ttyUSB0 -b 115200 --dtr 0 --rts 0
 * 
 */

#include <Arduino.h>

// ledPin refers to ESP32-CAM GPIO 4 (flashlight)
#define FLASH_GPIO_NUM 4
#define LED_ROUGE 33

void setup() {
    Serial.begin(115200);
    
    Serial.println("test led flash attention les yeux !!!");
    pinMode(FLASH_GPIO_NUM, OUTPUT);
    pinMode(LED_ROUGE, OUTPUT);

    digitalWrite(FLASH_GPIO_NUM, HIGH);
    delay(2000);
    digitalWrite(FLASH_GPIO_NUM, LOW);
    delay(2000);
}

void loop() {
    
    digitalWrite(LED_ROUGE, HIGH);
    delay(1000);
    digitalWrite(LED_ROUGE, LOW);
    delay(1000);
    
}



