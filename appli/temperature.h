/*
 * BMP_180.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */
#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "macro_types.h"

/**
 * @brief  Initialisation du capteur de température BMP180
 */
void Temperature_Init(void);

/**
 * @brief  Récupère la température depuis le capteur BMP180
 * @return La température en degrés Celsius (°C)
 */
float Temperature_get(void);

/**
 * @brief  Convertit la température en chaîne formatée
 * @param  temperature La température à convertir (°C)
 * @param  buffer Le buffer où stocker la chaîne
 */
void Temperature_to_string(float temperature, char* buffer);

#endif /* TEMPERATURE_H_ */

