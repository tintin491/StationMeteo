/*
 * DHT11.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#ifndef HUMIDITE_H_
#define HUMIDITE_H_

/**
 * @brief  Initialisation du capteur DHT11 pour l'humidité
 */
void Humidite_Init(void);

/**
 * @brief  Récupère l'humidité depuis le capteur DHT11
 * @return L'humisité en pourcentage (%)
 **/
float Humidite_get(void);

/**
 * @brief  Convertit l'humidé en chaîne formatée
 * @param  Humidité L'humidité
 * @param  buffer Le buffer où stocker la chaîne
 */
void Humidite_to_string(float temperature, char* buffer);


#endif /* HUMIDITE_H_ */
