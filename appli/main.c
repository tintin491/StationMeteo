/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "tft_ili9341/stm32f1_ili9341.h"
#include "stm32f1xx_hal.h"
#include "bmp180/bmp180.h"
#include "dht11/dht11.h"
#include "stdlib.h"
#include <stdio.h>
#include <time.h>
#include <locale.h>

//Attribut pour le BMP180
char buffer_BMP180[100];
BMP180_t BMP180_Data;

//Attribut pour le DHT11
static uint8_t humidity_int;
static uint8_t humidity_dec;
static uint8_t temperature_int;
static uint8_t temperature_dec;

#define SIZE 50
time_t currentTime;

char buffer_Date[SIZE];

int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();
	Temperature_Init();
	Pression_Init();
	time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    strftime(buffer_Date, sizeof(buffer_Date), "%d/%m/%y", localTime);
	// Initialize TFT screan

	ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawFilledCircle(107,140,100,ILI9341_COLOR_YELLOW);

    for(uint16_t i = 0; i <= SIZE; i++) {
    	ILI9341_Puts(116,18 + i,buffer_Date[i], &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
    }

	/* Initialiaze DTH11 */
	//DHT11_init(GPIOA, GPIO_PIN_0);

	while(1)	//boucle de tâche de fond
	{
		/*

		//Format data and print to USART
		sprintf(buffer, "Temp: %2.3f degrees\nPressure: %6ld Pascals\nAltitude at current pressure: %3.2f meters\n\n",
			BMP180_Data.Temperature,
			BMP180_Data.Pressure,
			BMP180_Data.Altitude
		);
		//Send to USART
		printf(buffer);

		//Some delay
		HAL_Delay(1000);

		// DTH11
		switch(DHT11_state_machine_get_datas(&humidity_int, &humidity_dec, &temperature_int, &temperature_dec))
		{
			case END_OK:
 				debug_printf("DHT11 h=%d,%d | t=%d,%d\n",humidity_int, humidity_dec, temperature_int, temperature_dec);
 				HAL_Delay(1500);
				break;
			case END_ERROR:
				debug_printf("DHT11 read error (h=%d,%d | t=%d,%d)\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
				HAL_Delay(1500);
				break;
			case END_TIMEOUT:
				debug_printf("DHT11 timeout (h=%d,%d | t=%d,%d)\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
				HAL_Delay(1500);
				break;
			default:
				break;
		}
		*/
	}
}
