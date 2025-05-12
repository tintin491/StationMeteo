/*
 * DHT11.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#ifndef HUMIDITE_H_
#define HUMIDITE_H_

/**
 * @brief  Initialisation du capteur DHT11 pour l'humidit�
 */
void Humidite_Init(void);

/**
 * @brief  R�cup�re l'humidit� depuis le capteur DHT11
 * @return L'humisit� en pourcentage (%)
 **/
float Humidite_get(void);

/**
 * @brief  Convertit l'humid� en cha�ne format�e
 * @param  Humidit� L'humidit�
 * @param  buffer Le buffer o� stocker la cha�ne
 */
void Humidite_to_string(float temperature, char* buffer);


#endif /* HUMIDITE_H_ */
