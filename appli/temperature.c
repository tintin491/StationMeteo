/*
 * temperature.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */


#include "bmp180/bmp180.h"

//Attribut pour le BMP180
BMP180_t BMP180_Data;
volatile bool_e flag;


/**
 * @brief  "initialization of temperature sensors
 */
void Temperature_Init(void){
	flag = TRUE;
	BMP180_Init(&BMP180_Data);
}

/**
 * @brief  "Get temperature from sensors
 */
float Temperature_get(void){
	if(flag){
		BMP180_StartTemperature(&BMP180_Data);
		HAL_Delay(BMP180_Data.Delay/1000+1);
		BMP180_ReadTemperature(&BMP180_Data);
		return BMP180_Data.Temperature;
	}
	return 0.0f;
}



