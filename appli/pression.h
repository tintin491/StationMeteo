/*
 * pression.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#ifndef PRESSION_H_
#define PRESSION_H_

/**
 * @brief  Initialisation du capteur BMP180 pour la pression
 */
void Pression_Init(void);

/**
 * @brief  R�cup�re la pression depuis le capteur BMP180
 * @return La pression en hPa (hectopascals)
 */
float Pression_get(void);

/**
 * @brief  Convertit la pression en cha�ne format�e
 * @param  pressure La pression � convertir (hPa)
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Pression_to_string(float pressure, char* buffer);

#endif /* PRESSION_H_ */
