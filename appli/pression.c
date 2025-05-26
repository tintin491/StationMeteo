/*
 * pression.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#include "bmp180/bmp180.h"
#include "macro_types.h"  // Pour bool_e

// D�finition des variables globales (utilis�es aussi dans temperature.c)
BMP180_t BMP180_Data;
volatile bool_e flag = FALSE;

/**
 * @brief  Initialisation du capteur BMP180 pour la pression
 * @note   Cette fonction est d�j� appel�e dans Temperature_Init()
 */
void Pression_Init(void) {
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_I2C1_ENABLE();
    // Si le capteur n'est pas d�j� initialis�
    if (!flag) {
        flag = TRUE;
        BMP180_Init(&BMP180_Data);
    }
}

/**
 * @brief  R�cup�re la pression depuis le capteur BMP180
 * @return La pression en hPa (hectopascals)
 */
float Pression_get(void) {
    if (flag) {
        // Utilisez 0 comme valeur par d�faut pour le mode
        // 0 est g�n�ralement le mode standard/par d�faut dans la plupart des biblioth�ques
        BMP180_StartPressure(&BMP180_Data, 0);

        // Attendre que la mesure soit pr�te
        HAL_Delay(BMP180_Data.Delay/1000 + 1);

        // Lire la pression
        BMP180_ReadPressure(&BMP180_Data);

        // Retourner la pression en hPa (1 hPa = 100 Pa)
        return (float)(BMP180_Data.Pressure) / 100.0f;
    }
    return 0.0f;
}

/**
 * @brief  Convertit la pression en cha�ne format�e
 * @param  pressure La pression � convertir (hPa)
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Pression_to_string(float pressure, char* buffer) {
    sprintf(buffer, "Pression: %.1f hPa", pressure);
}
