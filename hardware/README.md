# Guide de brochage ESP32-CAM AI-Thinker

## Introduction
**ESP32-CAM** est une carte de développement avec une puce ESP32-S, une caméra **OV2640**, un emplacement pour carte microSD et plusieurs GPIO pour connecter des périphériques. Dans cette section, nous examinerons les GPIO ESP32-CAM.
![brochage](/hardware/Brochage.png)

## Broches d'alimentation

L'ESP32-CAM est livré avec trois broches de masse (colorées en noir) et une broche d'**alimentation en entrée** (colorée en rouge) 5V.
Il y a aussi  une broche d'alimentation en sortie. Elle délivre une tension de  3.3V.

![alimentation](/hardware/alimentation.png)

## Connexions de la carte MicroSD

la bibliothèque SD fonctionne sur SPI et le SD_MMC utilise le [bus matériel SDMMC](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/sdmmc_host.html) de l'ESP32 . Ainsi, en fonction de la configuration matérielle, vous devez choisir la bonne bibliothèque. Pour la carte ESP32-Cam, vous devez utiliser la bibliothèque  SD_MMC. 

| Carte Micro SD |  ESP32 |
|-------------------|-----|
| CLK | GPIO 14 |
| CMD | GPIO 15 |
| DONNÉE S0 | GPIO 2 |
| DATA1 / lampe | GPIO 4 |
| DONNÉE S2 | GPIO 12 |
| DONNÉE S3 | GPIO 13 |

Comme indiqué ci-dessus, l'ESP32-CAM utilise les broches GPIO GPIO02, GPIO04, GPIO13 et GPIO15 pour lire et écrire des données sur la carte SD. Pour utiliser la carte SD en mode '1-Bit' ou '1-Wire', il suffit d'initialiser la carte SD avec le code suivant (cf. Espressif Arduino core pour l'ESP32) : Le prototype de la méthode begin est :
```
bool begin(const char * mountpoint="/sdcard", bool mode1bit=false); 
```
cf. [SD_MMC.h](https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/src/SD_MMC.h)

```cpp
SD_MMC.begin("/sdcard", true)
```
En définissant le paramètre mode1bit sur ```true```, l'ESP32-CAM utilise simplement le **GPIO02** pour lire et écrire des données sur la carte SD, ce qui libère les broches GPIO GPIO04 (HS_DATA1), GPIO12 (HS_DATA2) et GPIO13 (HS_DATA3) comme GPIO supplémentaire d'entrée ou de sortie. Pour utiliser une ou toutes ces broches GPIO  comme entrée ou  sortie, vous devez définir la broche GPIO en conséquence **après l'initialisation de la carte SD**. Sinon, l'initialisation de la carte SD reconfigurerait à nouveau les broches GPIO. Par exemple, l'extrait de code suivant configure la broche  GPIO12 en tant que broche de sortie et définit l'état sur LOW :
```cpp

pinMode(12, OUTPUT); 
digitalWrite(12, LOW);

#define BUILTIN_LED 4
pinMode(BUILTIN_LED, OUTPUT); 
digitalWrite(BUILTIN_LED, LOW);
```
Remarque La lampe ne s’allumera pas lors des écritures sur la carte SD.



## Lampe  (GPIO 4)
L'ESP32-CAM dispose d'une LED intégrée très lumineuse qui peut fonctionner comme un flash lors de la prise de  photos. Cette LED est connectée en interne à GPIO 4.


