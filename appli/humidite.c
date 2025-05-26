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

float Humidite_get(void) {
	static uint8_t humidity_int;
	static uint8_t humidity_dec;
	static uint8_t temperature_int;
	static uint8_t temperature_dec;
    /*running_e status = DHT11_state_machine_get_datas(&humidity_int, &humidity_dec,
                          &temperature_int, &temperature_dec);

    switch(status) {
        case END_OK:
            debug_printf("Read successful: %d.%d\n", humidity_int, humidity_dec);
            return humidity_int + (humidity_dec / 10.0f);
        case END_ERROR:
            debug_printf("Checksum error!\n");
            break;
        case END_TIMEOUT:
            debug_printf("Timeout occurred!\n");
            break;
        case IN_PROGRESS:
            debug_printf("Reading in progress...\n");
            break;
    }*/
    HAL_Delay(1000);
    while (DHT11_state_machine_get_datas(&humidity_int, &humidity_dec,&temperature_int, &temperature_dec) == IN_PROGRESS){}
    float valeur = humidity_int + (humidity_dec / 10.0f);
    return valeur;
}

void Humidite_to_string(float humidite, char* buffer) {
    sprintf(buffer, "Humidite: %.1f%%", humidite);
}


