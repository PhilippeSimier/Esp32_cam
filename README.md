# ESP32-cam

## Introduction
Ce dépôt propose des programmes pour tester la carte esp32-cam.

![ESP32-cam](/img/board_ESP32_cam.png)

Les programmes sont développés sous Linux Mint avec Netbeans et Platformio.

## Camera Specifications

L'ESP32-CAM comprend une caméra **OV2640**. La carte prend également en charge la caméra OV7670.

L'**OV2640** a les spécifications suivantes :

-   2 Megapixel sensor
-   Array size UXGA 1622×1200
-   Output formats include YUV422, YUV420, RGB565, RGB555 and JPEG
-   Image transfer rate of 15 to 60 fps

## Tests de la led flash et de la liaison série

**La liaison série ne fonctionne pas avec putty** (problème avec les ligne dtr et rts).
Vous devez utiliser à la place le moniteur série de platformio avec les options suivantes :
```
pio device monitor -p /dev/ttyUSB0 -b 115200 --dtr 0 --rts 0
```
![screen shot](/img/ESP32_cam_serial.png)

La LED rouge est branchée à la broche GPIO 33. Elle est allumée quand la broche GPIO 33 est au niveau logique bas, et éteinte quand elle est au niveau logique haut.
La LED blanche est liée à la broche GPIO 4.

## Prendre une photo et l'enregistrer sur la carte SD

le fichier ``` esp_camera.h``` : donne le prototype des fonctions pour initialiser la caméra et en obtenir des images.

Lorsqu'on  capture une image avec la caméra, le tampon d'image la contiendra dans le format sélectionné. Attention tous les formats ne sont pas supportés. 

 - **GRAYSCALE** signifie des valeurs de 0 à 255 pour chaque pixel 
 - **RGB565** signifie qu'une valeur de 16 bits est par pixel et comprend 5 bits de rouge, 6 bits de vert et 5 bits de bleu.  
 - **~~RGB888~~**  **(non supporté)** signifie qu'il y a 3 octets par pixel, un pour le rouge, un pour le vert et un pour le bleu. Chacun varie entre 0 et 255.  
 - **YUV** signifie que chaque pixel a son canal Y comme un octet séparé (0 à 255) et que chacun des deux pixels adjacents partage ses valeurs U et V. 
 -  **JPEG** signifie que l'image est encodée au format JPEG soit par l'appareil photo lui-même.
 
```cpp
	camera_config_t config;
	config.pixel_format = PIXFORMAT_JPEG;
```
La taille de l'image peut aussi être configurée. SVGA (800×600).
```cpp
	config.frame_size = FRAMESIZE_SVGA;
```
### Obtenir une photo

```cpp
camera_fb_t *fb = NULL;  // déclaration d'une structure frame buffer
```
Une structure de  type **camera_fb_t** contient les informations suivantes (parmi quelques paramètres supplémentaires) :  

 - **fb->buf** Un pointeur vers un tampon qui contient les données de pixel de l'image (uint8_t *) 
 - **fb->len** la longueur du tampon, en octets 
 - **fb->width** la largeur
 - **fb->height**  la hauteur du tampon, en pixels.

```cpp
camera_fb_t *fb = NULL;  // déclaration d'une structure frame buffer

fb = esp_camera_fb_get(); // remplissage du frame buffer avec une image
if (!fb) {
	Serial.println("Camera capture failed");
	return false;
}
```
### Enregistrer la photo dans un fichier sur la carte SD

Pour enregistrer l'image JPEG sur la carte SD, il suffit d'écrire le contenu du frame buffer dans un fichier ouvert en écriture.

```cpp
	file.write(fb->buf, fb->len);
```

```esp_camera_fb_return(fb); ``` libère la mémoire tampon afin qu'elle puisse être réutilisée.

![philippe](/img/picture7.jpg)
