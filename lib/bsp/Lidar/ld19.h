/*
 * ld19.h
 *
 *  Created on: 1 juin 2023
 *      Author: Nirgal
 */

#ifndef LD19_H_
#define LD19_H_

#include <stdint.h>

void LD19_init(void);

void LD19_DEMO_process_main(void);

typedef struct
{
	int32_t x;
	int32_t y;
	int32_t theta;
}pos_t;
#endif /* LD19_H_ */
