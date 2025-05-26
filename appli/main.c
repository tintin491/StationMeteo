/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "affichage.h"
#include "stm32f1xx_hal.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#include "tft_ili9341/stm32f1_ili9341.h"
#include "temperature.h"
#include "pression.h"
#include "affichage.h"
#include "humidite.h"


volatile uint32_t timer = 0;

// Variable pour la température BMP180
static float temperature_bmp;
static float pressure_bmp;
static float humidite_dht;
static void process_ms(void);

static volatile uint32_t t = 0;

void process_ms(void)
{
    if(t)
        t--;

    if(timer) timer--;
}


int main(void)
{
//     Initialisation de la couche logicielle HAL
    HAL_Init();

    Systick_add_callback_function(&process_ms);

//	// Initialisation du l'écran
	AFFICHEUR_init();
//
//    // Initialisation du capteur de température BMP180
    Temperature_Init();

////    // Initialisation du capteur de pression BMP180
    Pression_Init();

//    // Initialisation du capteur d'humidité DHT11
    Humidite_Init();

    while(1)
    {
        // Lire les données...
        temperature_bmp = Temperature_get();
        pressure_bmp = Pression_get();
        humidite_dht = Humidite_get();

        // Mettre à jour l'affichage (une seule fois pour éviter le scintillement)
        AFFICHEUR_setTemperature(temperature_bmp);
        AFFICHEUR_setPression(pressure_bmp);
        AFFICHEUR_setHumidite(humidite_dht);

        HAL_Delay(2000);
    }
}
