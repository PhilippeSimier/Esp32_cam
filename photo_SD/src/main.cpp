/* 
 * File:   main.cpp
 * Author: philippe SIMIER Touchard Washington le Mans
 *
 * Created on 21 juin 2022, 11:35
 * 
 * Take a photo and save it on the SD card
 * 
 */

#include "Arduino.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"

#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"


#include "FS.h"     // SD Card ESP32
#include "SD_MMC.h" // SD Card ESP32
#include <EEPROM.h> // read and write from flash memory

// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#define FLASH_GPIO_NUM 4

// ledPin refers to ESP32-CAM GPIO 4 (flashlight)
#define FLASH_GPIO_NUM 4

// turn on the flash for N seconds

int count = 0;

void flash(int N) {
    digitalWrite(FLASH_GPIO_NUM, HIGH);
    delay(N * 1000);
    digitalWrite(FLASH_GPIO_NUM, LOW);
}

void initCamera() {
    // Turn-off the 'brownout detector'
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    // OV2640 camera module
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;


    config.frame_size =  FRAMESIZE_SVGA; // FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        ESP.restart();
    }
}

// Capture Photo and Save it to SD card

void capturePhotoSaveSD(void) {
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }




    // Path where new picture will be saved in SD Card

    String path = "/picture" + String(count) + ".jpg";

    fs::FS &fs = SD_MMC;
    Serial.printf("Picture file name: %s\n", path.c_str());

    File file = fs.open(path.c_str(), FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file in writing mode");
    } else {
        file.write(fb->buf, fb->len); // payload (image), payload length
        Serial.printf("Saved file to path: %s\n", path.c_str());
        count++;
    }
    file.close();
    esp_camera_fb_return(fb);
}

void setup() {
    Serial.begin(115200);
    // initialize the flash light pin as an output:
    pinMode(FLASH_GPIO_NUM, OUTPUT);

    if (!SD_MMC.begin()) {
        Serial.println("SD Card Mount Failed");
        return;
    }

    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD Card attached");
        return;
    }


    initCamera();
    
}

void loop() {
    flash(1);
    capturePhotoSaveSD();
    delay(2000);
}

