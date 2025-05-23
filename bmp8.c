#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_bmp8* bmp8_loadImage(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    t_bmp8* img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        fclose(file);
        return NULL;
    }

    // Lecture de l'en-tête
    fread(img->header, sizeof(unsigned char), 54, file);

    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned int*)&img->header[28];

    // Calcul correct de la taille des données
    // Pour une image 8 bits, chaque pixel occupe 1 octet
    // La largeur doit être alignée sur 4 octets (padding)
    int padding = (4 - (img->width % 4)) % 4;
    img->dataSize = (img->width + padding) * img->height;

    if (img->colorDepth != 8) {
        printf("Erreur: L'image doit etre en 8 bits\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lecture de la table des couleurs
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    // Vérification de l'allocation
    img->data = (unsigned char*)malloc(img->dataSize);
    if (!img->data) {
        printf("Erreur: Impossible d'allouer la memoire pour les donnees\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lecture des données avec vérification
    size_t bytesRead = fread(img->data, sizeof(unsigned char), img->dataSize, file);
    if (bytesRead != img->dataSize) {
        printf("Attention: Nombre d'octets lus (%zu) différent de la taille attendue (%u)\n",
               bytesRead, img->dataSize);
    }

    fclose(file);
    return img;
}

void bmp8_saveImage(const char* filename, t_bmp8* img) {
    char new_path[256] = "C:\\Users\\arket\\CLionProjects\\Image-Processing-C\\image\\";
    strcat(new_path, filename);  // Ajoute le nom du fichier après "Image Cree/"

    FILE* file = fopen(new_path, "wb");
    if (!file) {
        printf("Erreur: Impossible de sauvegarder dans %s\n", new_path);
        return;

    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}

void bmp8_free(t_bmp8* img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

void bmp8_printInfo(t_bmp8* img) {
    printf("Image Info:\n");
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);
}

void bmp8_negative(t_bmp8* img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8* img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        img->data[i] = (unsigned char)newValue;
    }
}

void bmp8_threshold(t_bmp8* img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

void bmp8_applyFilter(t_bmp8* img, float** kernel, int kernelSize) {
    unsigned char* tempData = (unsigned char*)malloc(img->dataSize);
    if (!tempData) return;

    memcpy(tempData, img->data, img->dataSize);

    int n = kernelSize / 2;

    for (unsigned int y = n; y < img->height - n; y++) {
        for (unsigned int x = n; x < img->width - n; x++) {
            float sum = 0.0f;

            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    unsigned int pos = (y + i) * img->width + (x + j);
                    sum += tempData[pos] * kernel[i + n][j + n];
                }
            }

            // Limiter les valeurs entre 0 et 255
            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            img->data[y * img->width + x] = (unsigned char)sum;
        }
    }

    free(tempData);
}