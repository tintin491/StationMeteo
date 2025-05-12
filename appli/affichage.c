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

static char buffer[30]; // Ajout du buffer pour l'affichage

void AFFICHEUR_init(void)
{
    // Initialisation de l'écran TFT
    ILI9341_Init();

    // Effacer l'écran avec une couleur de fond
    ILI9341_Fill(ILI9341_COLOR_WHITE);

    // Afficher un titre
    ILI9341_Puts(10, 10, "Weather Station", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}


void AFFICHEUR_setTemperature(float temperature)
{
    // Afficher la température du BMP180
    Temperature_to_string(temperature, buffer);
    ILI9341_Puts(10, 110, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void AFFICHEUR_setPression(float pression)
{
    // Afficher la pression
    Pression_to_string(pression, buffer);
    ILI9341_Puts(10, 80, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

    // Afficher une indication graphique de la pression
    // Niveau de pression normal ~1013 hPa
    char * pression_info;
    if(pression > 1020) {
        pression_info = "Haute pression";
    } else if(pression < 1000) {
        pression_info = "Basse pression";
    } else {
        pression_info = "Pression normale";
    }

    ILI9341_Puts(10, 200, pression_info, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
}

void AFFICHEUR_setHumidite(float humidite){

    // Afficher l'humidité  du DHT11
    Humidite_to_string(humidite, buffer);
    ILI9341_Puts(10, 140, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void AFFICHEUR_setLuminosite(uint16_t luminosite){
	Luminosite_to_string(luminosite, buffer);
    ILI9341_Puts(10, 140, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

