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
 * @brief  Initialisation du capteur de temp�rature BMP180
 */
void Temperature_Init(void);

/**
 * @brief  R�cup�re la temp�rature depuis le capteur BMP180
 * @return La temp�rature en degr�s Celsius (�C)
 */
float Temperature_get(void);

/**
 * @brief  Convertit la temp�rature en cha�ne format�e
 * @param  temperature La temp�rature � convertir (�C)
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Temperature_to_string(float temperature, char* buffer);

#endif /* TEMPERATURE_H_ */

