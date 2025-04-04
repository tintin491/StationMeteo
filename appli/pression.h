/*
 * pression.h
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#ifndef PRESSION_H_
#define PRESSION_H_


/**
 * @brief  "initialization of pressure sensors
 */
void Pression_Init(void);

/**
 * @brief  "Get pressure from sensors
 */
uint32_t Pression_get(void);

#endif /* PRESSION_H_ */
