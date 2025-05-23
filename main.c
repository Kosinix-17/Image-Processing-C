#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"


void afficherMenuPrincipal() {
    printf("Veuillez choisir une option :\n");
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n");
    printf(">>> Votre choix : ");
}

void afficherMenuFiltres() {
    printf("Veuillez choisir un filtre :\n");
    printf("1. Negatif\n");
    printf("2. Luminosite\n");
    printf("3. Binarisation\n");
    printf("4. Flou\n");
    printf("5. Flou gaussien\n");
    printf("6. Nettete\n");
    printf("7. Contours\n");
    printf("8. Relief\n");
    printf("9. Retourner au menu precedent\n");
    printf(">>> Votre choix : ");
}

int main() {
    t_bmp8* image = NULL;
    char filename[256];
    int choix, choixFiltre;

    do {
        afficherMenuPrincipal();
        scanf("%d", &choix);
        getchar(); // Pour consommer le retour chariot

        switch(choix) {
            case 1: // Ouvrir une image
                printf("Chemin du fichier : ");
                scanf("%255s", filename);
                getchar();
                image = bmp8_loadImage(filename);
                break;

                // Dans le case 2 du switch principal, remplacer le code existant par :
            case 2: // Sauvegarder une image
                if (!image) {
                    printf("Erreur : Aucune image chargee\n");
                    break;
                }
                printf("Nom de la nouvelle image (sans .bmp) : ");
                scanf("%245s", filename);  // 245 pour laisser de la place pour ".bmp"
                getchar();
                snprintf(filename + strlen(filename), 5, ".bmp");  // 5 pour inclure le caractère nul
                bmp8_saveImage(filename, image);
                break;

            case 3: // Appliquer un filtre
                if (!image) {
                    printf("Erreur : Aucune image chargee\n");
                    break;
                }
                do {
                    afficherMenuFiltres();
                    scanf("%d", &choixFiltre);
                    getchar();

                    switch(choixFiltre) {
                        case 1: // Négatif
                            bmp8_negative(image);
                            break;
                        case 2: // Luminosité
                            printf("Valeur de luminosite (-255 à 255) : ");
                            int valeur;
                            scanf("%d", &valeur);
                            getchar();
                            bmp8_brightness(image, valeur);
                            break;
                        case 3: // Binarisation
                            printf("Seuil (0-255) : ");
                            int seuil;
                            scanf("%d", &seuil);
                            getchar();
                            bmp8_threshold(image, seuil);
                            break;
                        // Ajoutez les autres cas pour les filtres restants
                        case 9: // Retour au menu principal
                            break;
                        default:
                            printf("Option invalide\n");
                            break;
                    }
                    if (choixFiltre >= 1 && choixFiltre <= 8) {
                        printf("Filtre applique avec succes !\n");
                        break;
                    }
                } while (choixFiltre != 9);
                break;

            case 4: // Afficher les informations
                if (!image) {
                    printf("Erreur : Aucune image chargee\n");
                    break;
                }
                bmp8_printInfo(image);
                break;

            case 5: // Quitter
                if (image) {
                    bmp8_free(image);
                }
                break;

            default:
                printf("Option invalide\n");
                break;
        }
    } while (choix != 5);

    return 0;
}