/*
 * DHT11.c
 *
 *  Created on: Mar 31, 2025
 *      Author: quent
 */

#include "humidite.h"
#include "dht11/dht11.h"

void Humidite_Init(void){
	DHT11_init(GPIOA, GPIO_PIN_0);
}


float Humidite_get(void){

	static uint8_t humidity_int;
	static uint8_t humidity_dec;
	static uint8_t temperature_int;
	static uint8_t temperature_dec;

	while(1)
	{
		switch(DHT11_state_machine_get_datas(&humidity_int, &humidity_dec, &temperature_int, &temperature_dec))
		{
			case END_OK:
				return humidity_int + (humidity_dec / 10.0f);
			case END_ERROR:
				debug_printf("DHT11 read error (h=%d,%d | t=%d,%d)\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
				HAL_Delay(1500);
				return 0;
			case END_TIMEOUT:
				debug_printf("DHT11 timeout (h=%d,%d | t=%d,%d)\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
				HAL_Delay(1500);
				return 0;
			default:
				return 0;
		}
	}
}

void Humidite_to_string(float humidite, char* buffer) {
    sprintf(buffer, "Humidite: %.1f", humidite);
}

