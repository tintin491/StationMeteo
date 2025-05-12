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

// Déclaration des variables globales
extern BMP180_t BMP180_Data;
extern volatile bool_e flag;

/**
 * @brief  Initialisation du capteur de température BMP180
 */
void Temperature_Init(void) {
    flag = TRUE;
    BMP180_Init(&BMP180_Data);
}

/**
 * @brief  Récupère la température depuis le capteur BMP180
 * @return La température en degrés Celsius (°C)
 */
float Temperature_get(void) {
    if (flag) {
        BMP180_StartTemperature(&BMP180_Data);
        HAL_Delay(BMP180_Data.Delay/1000 + 1);
        BMP180_ReadTemperature(&BMP180_Data);
        return BMP180_Data.Temperature;
    }
    return 0.0f;
}

/**
 * @brief  Convertit la température en chaîne formatée
 * @param  temperature La température à convertir (°C)
 * @param  buffer Le buffer où stocker la chaîne
 */
void Temperature_to_string(float temperature, char* buffer) {
    sprintf(buffer, "Temp: %.1f C", temperature);
}


