/*
 * Affichage.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#include "affichage.h"
#include "temperature.h"
#include "pression.h"
#include "humidite.h"
#include "luminosite.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"

static char buffer[30]; // Buffer pour afficher les valeurs numériques

void drawTempIcon(uint16_t x, uint16_t y) {
    // Thermomètre : ligne + cercle
    ILI9341_DrawLine(x + 3, y, x + 3, y + 14, ILI9341_COLOR_RED);
    ILI9341_DrawFilledCircle(x + 3, y + 16, 3, ILI9341_COLOR_RED);
}

void drawPressureIcon(uint16_t x, uint16_t y) {
    // Baromètre : cercle avec aiguille
    ILI9341_DrawCircle(x + 6, y + 6, 6, ILI9341_COLOR_BLUE);
    ILI9341_DrawLine(x + 6, y + 6, x + 10, y + 2, ILI9341_COLOR_BLUE); // Aiguille oblique
}

void drawHumidityIcon(uint16_t x, uint16_t y) {
    // Goutte d'eau stylisée
    ILI9341_DrawFilledCircle(x + 4, y + 4, 4, ILI9341_COLOR_CYAN);
    ILI9341_DrawLine(x + 4, y + 6, x, y + 12, ILI9341_COLOR_CYAN); // Courbe bas gauche
    ILI9341_DrawLine(x + 4, y + 6, x + 8, y + 12, ILI9341_COLOR_CYAN); // Courbe bas droite
}

void AFFICHEUR_init(void)
{
    ILI9341_Init();
    ILI9341_Fill(ILI9341_COLOR_WHITE);

    const char *titre = "Meteo d'aujourd'hui";
    uint16_t titre_largeur = strlen(titre) * 11;
    uint16_t x_centre = (240 - titre_largeur) / 2;
    uint16_t y_texte = 10;
    uint16_t hauteur_texte = 18;

    // Titre centré avec effet gras
    ILI9341_Puts(x_centre, y_texte, titre, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_Puts(x_centre + 1, y_texte, titre, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_DrawLine(x_centre, y_texte + hauteur_texte + 1, x_centre + titre_largeur, y_texte + hauteur_texte + 1, ILI9341_COLOR_BLACK);
}


void AFFICHEUR_setTemperature(float temperature)
{
    drawTempIcon(5, 105);
    Temperature_to_string(temperature, buffer);
    ILI9341_Puts(20, 110, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void AFFICHEUR_setPression(float pression)
{
    drawPressureIcon(5, 82);
    Pression_to_string(pression, buffer);
    ILI9341_Puts(20, 80, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    const char *pression_info;
    if (pression > 1020)
        pression_info = "Haute pression";
    else if (pression < 1000)
        pression_info = "Basse pression";
    else
        pression_info = "Pression normale";

    ILI9341_Puts(20, 200, pression_info, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
}

void AFFICHEUR_setHumidite(float humidite)
{
    drawHumidityIcon(5, 142);
    Humidite_to_string(humidite, buffer);
    ILI9341_Puts(20, 140, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}
