/*
 * luminosite.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */


#include "luminosite.h"
#include "BH1750FVI/bh1750fvi.h"
#define BH1750FVI_ADDR BH1750FVI_ADDR_L

void Humidite_Init(void){
	BH1750FVI_init();
	BH1750FVI_powerOn();
}

uint16_t Luminosite_get(void){
	BH1750FVI_measureMode(BH1750FVI_CON_L);
	while(1)
	{
		HAL_Delay(200);
		return BH1750FVI_readLuminosity();
	}
}

 Pression_to_string(uint16_t luminosite, char* buffer) {
    sprintf(buffer,"Luminosite = %d lx", luminosite);
}

