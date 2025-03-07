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
#include <stdlib.h>

char buffer[100];
/* Working structure */
BMP180_t BMP180_Data;

int main(void)
{

	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();

	// Initialisation de l'écran TFT
	/*
	ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);
	ILI9341_Putc(110,11,'x',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Putc(15,110,'y',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,200, "chaine", &Font_11x18, ILI9341_COLOR_BROWN,ILI9341_COLOR_WHITE);
	*/


	/* Initialize BMP180 pressure sensor */
	BMP180_Init(&BMP180_Data);

	while(1)	//boucle de tâche de fond
	{
		/* Start temperature conversion */
		BMP180_StartTemperature(&BMP180_Data);

		/* Wait delay in microseconds */
		/* You can do other things here instead of delay */
		HAL_Delay(BMP180_Data.Delay/1000+1);

		/* Read temperature first */
		BMP180_ReadTemperature(&BMP180_Data);

		/* Start pressure conversion at ultra high resolution */
		BMP180_StartPressure(&BMP180_Data, BMP180_Oversampling_UltraHighResolution);

		/* Wait delay in microseconds */
		/* You can do other things here instead of delay */
		HAL_Delay(BMP180_Data.Delay/1000+1);

		/* Read pressure value */
		BMP180_ReadPressure(&BMP180_Data);

		/* Format data and print to USART */
		sprintf(buffer, "Temp: %2.3f degrees\nPressure: %6ld Pascals\nAltitude at current pressure: %3.2f meters\n\n",
			BMP180_Data.Temperature,
			BMP180_Data.Pressure,
			BMP180_Data.Altitude
		);
		/* Send to USART */
		printf(buffer);

		/* Some delay */
		HAL_Delay(1000);
	}
}
