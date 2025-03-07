/*
 * stm32f1_xpt2046.h
 *
 *  Created on: Nov 17, 2016
 *      Author: louisz
 */

#ifndef BSP_LCD_ILI9341_STM32F1_XPT2046_H_
#define BSP_LCD_ILI9341_STM32F1_XPT2046_H_

#include "macro_types.h"

typedef enum{
	XPT2046_COORDINATE_RAW,
	XPT2046_COORDINATE_SCREEN_RELATIVE
}XPT2046_coordinateMode_e;

void XPT2046_init(void);
void XPT2046_demo(void);
bool_e XPT2046_press_event_state_machine(int16_t * px, int16_t * py);	
bool_e XPT2046_getCoordinates(Sint16 * pX, Sint16 * pY, XPT2046_coordinateMode_e coordinateMode);
bool_e XPT2046_getMedianCoordinates(Sint16 * pX, Sint16 * pY, XPT2046_coordinateMode_e coordinateMode);
bool_e XPT2046_getAverageCoordinates(Sint16 * pX, Sint16 * pY, Uint8 nSamples, XPT2046_coordinateMode_e coordinateMode);

#ifndef XPT2046_SPI
	#define XPT2046_SPI           	SPI1
#endif

#define PIN_CS_TOUCH GPIOB, GPIO_PIN_0
#define PIN_IRQ_TOUCH GPIOA, GPIO_PIN_4	

#define XPT2046_USE_PIN_IRQ_TO_CHECK_TOUCH

#endif /* BSP_LCD_ILI9341_STM32F1_XPT2046_H_ */
