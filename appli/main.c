/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#include "tft_ili9341/stm32f1_ili9341.h"
#include "temperature.h"
#include "pression.h"

volatile uint32_t timer = 0;

// Variable pour la température BMP180
static float temperature_bmp;
static float pressure_bmp;
static char buffer[30]; // Ajout du buffer pour l'affichage

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

int main(void)
{
    // Initialisation de la couche logicielle HAL
    HAL_Init();

    // Initialisation du port de la LED verte (carte Nucleo)
    BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    // Initialisation du port du bouton bleu (carte Nucleo)
    BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);

    // On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms
    Systick_add_callback_function(&process_ms);

    // Initialisation de l'écran TFT
    ILI9341_Init();

    // Effacer l'écran avec une couleur de fond
    ILI9341_Fill(ILI9341_COLOR_WHITE);

    // Initialisation du capteur de température BMP180
    Temperature_Init();

    // Afficher un titre
    ILI9341_Puts(10, 10, "Weather Station", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
    ILI9341_DrawRectangle(5, 5, 230, 310, ILI9341_COLOR_BLUE);

    // Boucle principale
    while(1)
    {
        // Lire la température du BMP180
        temperature_bmp = Temperature_get();

        // Lire la pression du BMP180
        pressure_bmp = Pression_get();

        // Effacer les zones d'affichage précédentes
        ILI9341_DrawFilledRectangle(10, 40, 220, 120, ILI9341_COLOR_WHITE);

        // Afficher la température du BMP180
        Temperature_to_string(temperature_bmp, buffer);
        ILI9341_Puts(10, 110, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

        // Afficher la pression
        Pression_to_string(pressure_bmp, buffer);
        ILI9341_Puts(10, 80, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

        // Afficher une indication graphique de la pression
        // Niveau de pression normal ~1013 hPa
        char * pression_info;
        if(pressure_bmp > 1020) {
            pression_info = "Haute pression";
        } else if(pressure_bmp < 1000) {
            pression_info = "Basse pression";
        } else {
            pression_info = "Pression normale";
        }

        ILI9341_Puts(10, 200, pression_info, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

        // Attendre avant la prochaine mesure
        HAL_Delay(2000);  // Attendre 2 secondes
    }
}
