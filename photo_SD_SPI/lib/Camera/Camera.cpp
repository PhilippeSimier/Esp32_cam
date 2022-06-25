/* 
 * File:   Camera.cpp
 * Author: philippe SIMIER Lycée Touchard Washington Le Mans
 * 
 * Created on 23 juin 2022, 14:08
 */

#include "Camera.h"

Camera::Camera() :
count(0) {
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

bool Camera::init(pixformat_t pixFormat, framesize_t size) {

    camera_config_t config;
    bool retour;

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
    config.pixel_format = pixFormat; //PIXFORMAT_YUV422; //PIXFORMAT_RGB565;
    config.frame_size = size; //FRAMESIZE_QVGA 320x240;  FRAMESIZE_SVGA 800x600;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        retour = false;
    } else {
        retour = true;
    }

    // Initialisation du bus SPI
    SPI.begin(14, 2, 15, 13);
    delay(10);

    // Initialisation de la carte SD
    if (SDFileSystem.begin(13)) {

        Serial.println("Wiring is correct and a card is present.");
        Serial.println();

        Serial.print("Card type: ");
        switch (SDFileSystem.cardType()) {
            case CARD_NONE:
                Serial.println("NONE");
                break;
            case CARD_MMC:
                Serial.println("MMC");
                break;
            case CARD_SD:
                Serial.println("SD");
                break;
            case CARD_SDHC:
                Serial.println("SDHC");
                break;
            default:
                Serial.println("Unknown");
        }


        uint64_t cardSize = SDFileSystem.cardSize() / (1024 * 1024);
        Serial.printf("SD Card Size: %llu Mb\r\n", cardSize);

        uint64_t totalBytes = SDFileSystem.totalBytes() / (1024 * 1024);
        Serial.printf("Total bytes : %llu Mb\r\n", totalBytes);

        uint64_t usedBytes = SDFileSystem.usedBytes() / 1024;
        Serial.printf("Used bytes  : %llu Kb\r\n\n", usedBytes);

        retour = true;


    } else {
        Serial.println("Initialization card failed.");
        retour = false;
    }

    // Configuration du flash
    pinMode(FLASH_GPIO_NUM, OUTPUT);
    digitalWrite(FLASH_GPIO_NUM, LOW);

    return retour;
}

bool Camera::capturePhotoSaveSD(String name) {

    camera_fb_t *fb = NULL;

    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return false;
    }

    // Path where new picture will be saved in SD Card

    String path = "/" + name + String(count) + ".jpg";
    Serial.printf("Picture file name: %s\n", path.c_str());

    File file = SDFileSystem.open(path.c_str(), FILE_WRITE);
    
    if (!file) {
        Serial.println("Failed to open file in writing mode");
        return false;
    } else {
        file.write(fb->buf, fb->len); // payload (image), payload length
        Serial.printf("Saved file to path: %s\n", path.c_str());
        count++;
    }
    file.close();
    esp_camera_fb_return(fb);
    return true;
}

/**
 * @brief turn on the flash for N seconds
 * @param N nombre de secondes
 */
void Camera::flash(int N) {
    digitalWrite(FLASH_GPIO_NUM, HIGH);
    delay(N * 1000);
    digitalWrite(FLASH_GPIO_NUM, LOW);
}

