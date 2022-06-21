# ESP32-cam

## Introduction
Ce dépôt propose des programmes pour tester la carte esp32-cam.
![ESP32-cam](/img/board_ESP32_cam.png)
Les programmes sont développés sous linux mint avec netbeans et platformio.

## Tests de la led flash et de la liaison série

**La liaison série ne fonctionne pas avec putty** (problème avec les ligne dtr et rts).
Vous devez utiliser à la place le moniteur série de platformio avec les options suivantes :
```
pio device monitor -p /dev/ttyUSB0 -b 115200 --dtr 0 --rts 0
```
![screen shot](/img/ESP32_cam_serial.png)

## Prendre une photo et l'enregistrer sur la carte SD



