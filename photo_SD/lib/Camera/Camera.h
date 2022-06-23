/* 
 * File:   Camera.h
 * Author: philippe SIMIER Lycée Touchard Washington Le Mans
 *
 * Created on 23 juin 2022, 14:08
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include <esp_camera.h>
#include <soc/soc.h>          // Disable brownour problems
#include <soc/rtc_cntl_reg.h> // Disable brownour problems
#include <FS.h>     // SD Card ESP32
#include <SD_MMC.h> // SD Card ESP32


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
#define LED_ROUGE 33


class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();
    
    bool init();
    bool capturePhotoSaveSD(String name = "picture");
    void flash(int N);
    
    
private:
    int count;

};

#endif /* CAMERA_H */

