/*
 * BH1750.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */
#ifndef LUMINOSITE_H_
#define LUMINOSITE_H_

#include "macro_types.h"

/**
 * @brief  Initialisation du capteur de luminosite BH1750
 */
void Luminosite_Init(void);

/**
 * @brief  R�cup�re la luminosite depuis le capteur BH1750
 * @return La luminosite en luc
 *  */
uint16_t Luminosite_get(void);

/**
 * @brief  Convertit la luminosite en cha�ne format�e
 * @param  luminosite La luminosite � convertir (Lux)
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Luminosite_to_string(uint16_t luminosite, char* buffer);

#endif /* LUMINOSITE_H_ */
