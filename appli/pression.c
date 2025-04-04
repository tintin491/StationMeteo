/*
 * pression.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#include "bmp180/bmp180.h"

//Attribut pour le BMP180
BMP180_t BMP180_Data;
volatile bool_e flag;


/**
 * @brief  "initialization of pressure sensors
 */
void Pression_Init(void){
	flag = TRUE;
	BMP180_Init(&BMP180_Data);
}

/**
 * @brief  "Get pressure from sensors
 */
uint32_t  Pression_get(void){
	if(flag){
		BMP180_StartPressure(&BMP180_Data, BMP180_Oversampling_UltraHighResolution);
		HAL_Delay(BMP180_Data.Delay/1000+1);
		BMP180_ReadPressure(&BMP180_Data);
		return BMP180_Data.Pressure;
	}
	return (uint32_t )0;
}


