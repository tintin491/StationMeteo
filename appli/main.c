/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include <stdio.h>
#include <time.h>
#include <locale.h>
#define SIZE 50
char buffer_Date[SIZE];

volatile uint32_t timer = 0;

void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}

static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;

	if(timer) timer--;
}

time_t currentTime;

int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsulée sur l'USB vers le PC de développement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le périphérique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	Systick_add_callback_function(&process_ms);

	//Ajouter dans la fonction main(), juste avant la boucle while(1) de tâche de fond :
	ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);
	ILI9341_Putc(110,11,'x',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Putc(15,110,'y',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,200, "chaine", &Font_11x18, ILI9341_COLOR_BROWN,
	ILI9341_COLOR_WHITE);


    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    strftime(buffer_Date, sizeof(buffer_Date), "%d/%m/%y", localTime);
    // Initialize TFT screan

    ILI9341_DrawFilledCircle(107,140,100,ILI9341_COLOR_YELLOW);

    for(uint16_t i = 0; i <= SIZE; i++) {
        ILI9341_Puts(116,(18 + i),&buffer_Date[i], &Font_11x18, ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE);
    }
	 while(1)
	 {

	  }
}
