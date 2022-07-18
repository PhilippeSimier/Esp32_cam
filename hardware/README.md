# Guide de brochage ESP32-CAM AI-Thinker

## Introduction
**ESP32-CAM** est une carte de développement avec une puce ESP32-S, une caméra **OV2640**, ume mémoire SPRAM, un emplacement pour carte microSD et quelques GPIO pour connecter des périphériques. 

Toutes les broches GPIO du module ESP-32S sont utilisées par la camera, la carte SD, la mémoire PSRAM et l'interface série pour la programmation.   Les broches I2C **TWI_SDA** et **TWI_SCK** ne sont pas exposées et cela paralysent considérablement les applications possibles. 

Dans cette section, nous examinerons les GPIO exposés sur P1 et P2.

### Identification des broches ( Carte SD reliée au bus MMC )
![brochage MMC](/hardware/Brochage_MMC.png)

### Identification des broches ( Carte SD reliée au bus SPI )
![brochage SPI](/hardware/Brochage_SPI.png)

## Broches d'alimentation

L'ESP32-CAM est livré avec deux broches de masse (en noir) et une broche d'**alimentation en entrée** (en rouge) 5V.
Il y a aussi  une broche d'alimentation en sortie. Elle délivre une tension de  3.3V ou 5V suivant la présence d'une résistance R1 ou R2 (0 Ohm).
La consommation de la carte est de **100 mA** environ. On remarque que la mise sous tension de la caméra est commandés par  GPIO 32 **CAM_PWR**. Il est donc possible de réinitialiser la camera par programme.

![alimentation](/hardware/alimentation.png)

## Broche Reset ESP32

La broche 1 du connecteur P1 est connectée à E32_RST. 

## Connexions de la carte MicroSD

la bibliothèque SD fonctionne sur SPI et le SD_MMC utilise le [bus matériel SDMMC](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/sdmmc_host.html) de l'ESP32 . Ainsi, en fonction de la configuration matérielle, vous devez choisir la bonne bibliothèque. Pour la carte ESP32-Cam, vous pouvez utiliser la bibliothèque  SD_MMC ou la bibliothèque SD. 

| SD_MMC |  SD SPI |ESP32 |
|-------------------|-----|----|
| CLK | SCK | GPIO 14 |
| CMD | MOSI | GPIO 15 |
| DATA0 | MISO | GPIO 2 | 
| DATA1 / Flash | Flash | GPIO 4 | 
| DATA2 |  | GPIO 12 |  
| DATA3 | CS | GPIO 13 |

Comme indiqué ci-dessus, l'ESP32-CAM utilise les broches  GPIO02, GPIO04, GPIO12, GPIO13, GPIO14 et GPIO15 pour lire et écrire des données sur la carte SD. Pour utiliser la carte SD en mode '1-Bit' ou '1-Wire', il suffit d'initialiser la carte SD avec le code suivant (cf. Espressif Arduino core pour l'ESP32) : Le prototype de la méthode begin est :
```
bool begin(const char * mountpoint="/sdcard", bool mode1bit=false); 
```
cf. [SD_MMC.h](https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/src/SD_MMC.h)

```cpp
SD_MMC.begin("/sdcard", true)
```
En définissant le paramètre mode1bit sur ```true```, l'ESP32-CAM utilise simplement le **DATA 0 GPIO02** pour lire et écrire des données sur la carte SD, ce qui libère les broches GPIO GPIO04 (DATA1), GPIO12 (DATA2) et GPIO13 (DATA3) comme GPIO supplémentaire d'entrée ou de sortie. Pour utiliser une ou toutes ces broches GPIO  comme entrée ou  sortie, vous devez définir la broche GPIO en conséquence **après l'initialisation de la carte SD**. Sinon, l'initialisation de la carte SD reconfigurerait à nouveau les broches GPIO. Par exemple, l'extrait de code suivant configure la broche  GPIO12 en tant que broche de sortie et définit l'état sur LOW :
```cpp

pinMode(12, OUTPUT); 
digitalWrite(12, LOW);

#define BUILTIN_LED 4
pinMode(BUILTIN_LED, OUTPUT); 
digitalWrite(BUILTIN_LED, LOW);
```
Remarque La lampe FLASH ne s’allumera pas lors des écritures sur la carte SD.



## Led Flash  (GPIO 4)
L'ESP32-CAM dispose d'une LED intégrée très lumineuse qui peut fonctionner comme un flash lors de la prise de  photos. Cette LED est connectée en interne à GPIO 4.
Etat logique low correspond à la led Flash éteinte et l'état High à allumée. 

## GPIO12
La broche GPIO12 peut être **utilisée en sortie**. ne pas utiliser en entrée car il y a échec au démarrage si la résistance de tirage vers le haut est activée. 

## UART2 RX (GPIO16)
La broche GPIO 16 ne peut pas être utilisée ni en entrée ni
en sortie car elle est connectée à une résistance pull-up de 10Kohm au niveau de la PSRAM. 

## LED rouge intégrée (GPIO 33)

À côté du bouton RST, il y a une LED rouge intégrée. Cette LED est connectée en interne à **GPIO 33**. on peut utiliser cette LED pour indiquer que quelque chose se passe.

Cette LED **fonctionne avec une logique inversée**, donc vous envoyez un état LOW pour l'allumer et un état HIGH pour l'éteindre.



