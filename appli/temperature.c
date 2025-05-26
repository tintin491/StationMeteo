/*
 * temperature.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */


/*
 * temperature.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#include "bmp180/bmp180.h"
#include "macro_types.h"  // Pour bool_e

// D�claration des variables globales
extern BMP180_t BMP180_Data;
extern volatile bool_e flag;

/**
 * @brief  Initialisation du capteur de temp�rature BMP180
 */
void Temperature_Init(void) {

    flag = TRUE;
    BMP180_Init(&BMP180_Data);
}

/**
 * @brief  R�cup�re la temp�rature depuis le capteur BMP180
 * @return La temp�rature en degr�s Celsius (�C)
 */
float Temperature_get(void) {
    // Démarrer la mesure de température
    BMP180_StartTemperature(&BMP180_Data);

    // Attendre le délai nécessaire (en millisecondes)
    HAL_Delay(BMP180_Data.Delay);

    // Lire la température
    BMP180_ReadTemperature(&BMP180_Data);

    return BMP180_Data.Temperature;
}

/**
 * @brief  Convertit la temp�rature en cha�ne format�e
 * @param  temperature La temp�rature � convertir (�C)
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Temperature_to_string(float temperature, char* buffer) {
    sprintf(buffer, "Temperature: %.1f C", temperature);
}


