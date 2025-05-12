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
 * @brief  Récupère la pression depuis le capteur BMP180
 * @return La pression en hPa (hectopascals)
 */
float Pression_get(void);

/**
 * @brief  Convertit la pression en chaîne formatée
 * @param  pressure La pression à convertir (hPa)
 * @param  buffer Le buffer où stocker la chaîne
 */
void Pression_to_string(float pressure, char* buffer);

#endif /* PRESSION_H_ */
