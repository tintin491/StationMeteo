/*
 * ld19.c
 *
 *  Created on: 1 juin 2023
 *      Author: Nirgal
 */

//Ce fichier est un driver concernant le LIDAR LD19
//https://www.elecrow.com/download/product/SLD06360F/LD19_Development%20Manual_V2.3.pdf
#include "config.h"
#if USE_LD19
#include "ld19.h"
#include "macro_types.h"
#include "stm32f1_uart.h"


#ifndef LD19_UART
	#define LD19_UART	UART1_ID
#endif

#define POINT_PER_PACK 12
#define HEADER 0x54
#define PI4096	12868
#define PI	3.141592654f
typedef struct {
	pos_t robot_position;
	pos_t lidar_position_in_robot;
	bool_e lidar_way;	//si le lidar est posé la tête en bas : FALSE.
}lidar_parameters_t;

typedef struct __attribute__((packed)) {
	uint16_t distance;
	uint8_t intensity;
} LidarPointStructDef;

typedef struct __attribute__((packed)) {
	uint8_t header;
	uint8_t ver_len;
	uint16_t speed;
	uint16_t start_angle;
	LidarPointStructDef point[POINT_PER_PACK];
	uint16_t end_angle;
	uint16_t timestamp;
	uint8_t crc8;
	float start_angle_rad;
	float end_angle_rad;
	float computed_angle_rad[POINT_PER_PACK];
}LiDARFrameTypeDef;

#define VER_MASK	0xE0
#define LEN_MASK	0x1F

__attribute__((unused)) static uint8_t CalCRC8(uint8_t *p, uint8_t len);
static running_e LD19_parse(char c, LiDARFrameTypeDef * packet);
__attribute__((unused)) static void display_packet_infos(LiDARFrameTypeDef * packet);
static void LD19_rx_callback(void);


void LD19_init(void)
{
	UART_init(LD19_UART, 230400);
	UART_set_callback(LD19_UART, &LD19_rx_callback);
}

static LiDARFrameTypeDef last_packet;
static volatile bool_e flag_new_packet_available = FALSE;
static volatile bool_e flag_error_receiving_packet = FALSE;


void LD19_rx_callback(void)
{
	static LiDARFrameTypeDef it_packet;
	char c;
	while(UART_data_ready(LD19_UART))
	{
		c = UART_get_next_byte(LD19_UART);
		switch(LD19_parse(c, &it_packet))
		{
			case END_OK:
				last_packet = it_packet;
				flag_new_packet_available = TRUE;
				break;
			case END_ERROR:
				flag_error_receiving_packet = TRUE;
				break;
			case IN_PROGRESS:	//no break
			default:
				break;
		}
	}
}

void LD19_DEMO_process_main(void)
{
	if(flag_new_packet_available)
	{
		flag_new_packet_available = FALSE;
		display_packet_infos(&last_packet);
	}

	if(flag_error_receiving_packet)
	{
		flag_error_receiving_packet = FALSE;
		debug_printf("LD19 frame failure\n");
	}
}



/*
int16_t GEOMETRY_modulo_angle(int16_t angle)
{
    while(angle > 2*PI4096)
    {
        angle -= 2 * PI4096;
    }
    while(angle < 0)
    {
        angle += 2 * PI4096;
    }

    return angle;
}
*/

float GEOMETRY_modulo_angle_f(float angle)
{
    while(angle > 2*3.1415)
    {
        angle -= (float)(2 * 3.1415);
    }
    while(angle < 0)
    {
        angle += (float)(2 * 3.1415);
    }

    return angle;
}



static const uint8_t CrcTable[256] ={
0x00, 0x4d, 0x9a, 0xd7, 0x79, 0x34, 0xe3,
0xae, 0xf2, 0xbf, 0x68, 0x25, 0x8b, 0xc6, 0x11, 0x5c, 0xa9, 0xe4, 0x33,
0x7e, 0xd0, 0x9d, 0x4a, 0x07, 0x5b, 0x16, 0xc1, 0x8c, 0x22, 0x6f, 0xb8,
0xf5, 0x1f, 0x52, 0x85, 0xc8, 0x66, 0x2b, 0xfc, 0xb1, 0xed, 0xa0, 0x77,
0x3a, 0x94, 0xd9, 0x0e, 0x43, 0xb6, 0xfb, 0x2c, 0x61, 0xcf, 0x82, 0x55,
0x18, 0x44, 0x09, 0xde, 0x93, 0x3d, 0x70, 0xa7, 0xea, 0x3e, 0x73, 0xa4,
0xe9, 0x47, 0x0a, 0xdd, 0x90, 0xcc, 0x81, 0x56, 0x1b, 0xb5, 0xf8, 0x2f,
0x62, 0x97, 0xda, 0x0d, 0x40, 0xee, 0xa3, 0x74, 0x39, 0x65, 0x28, 0xff,
0xb2, 0x1c, 0x51, 0x86, 0xcb, 0x21, 0x6c, 0xbb, 0xf6, 0x58, 0x15, 0xc2,
0x8f, 0xd3, 0x9e, 0x49, 0x04, 0xaa, 0xe7, 0x30, 0x7d, 0x88, 0xc5, 0x12,
0x5f, 0xf1, 0xbc, 0x6b, 0x26, 0x7a, 0x37, 0xe0, 0xad, 0x03, 0x4e, 0x99,
0xd4, 0x7c, 0x31, 0xe6, 0xab, 0x05, 0x48, 0x9f, 0xd2, 0x8e, 0xc3, 0x14,
0x59, 0xf7, 0xba, 0x6d, 0x20, 0xd5, 0x98, 0x4f, 0x02, 0xac, 0xe1, 0x36,
0x7b, 0x27, 0x6a, 0xbd, 0xf0, 0x5e, 0x13, 0xc4, 0x89, 0x63, 0x2e, 0xf9,
0xb4, 0x1a, 0x57, 0x80, 0xcd, 0x91, 0xdc, 0x0b, 0x46, 0xe8, 0xa5, 0x72,
0x3f, 0xca, 0x87, 0x50, 0x1d, 0xb3, 0xfe, 0x29, 0x64, 0x38, 0x75, 0xa2,
0xef, 0x41, 0x0c, 0xdb, 0x96, 0x42, 0x0f, 0xd8, 0x95, 0x3b, 0x76, 0xa1,
0xec, 0xb0, 0xfd, 0x2a, 0x67, 0xc9, 0x84, 0x53, 0x1e, 0xeb, 0xa6, 0x71,
0x3c, 0x92, 0xdf, 0x08, 0x45, 0x19, 0x54, 0x83, 0xce, 0x60, 0x2d, 0xfa,
0xb7, 0x5d, 0x10, 0xc7, 0x8a, 0x24, 0x69, 0xbe, 0xf3, 0xaf, 0xe2, 0x35,
0x78, 0xd6, 0x9b, 0x4c, 0x01, 0xf4, 0xb9, 0x6e, 0x23, 0x8d, 0xc0, 0x17,
0x5a, 0x06, 0x4b, 0x9c, 0xd1, 0x7f, 0x32, 0xe5, 0xa8
};

/* Constitution de la trame :
 *
 * 		Header 	| Verlen	| Speed		| StartAngle	|  Data.......				| EndAngle	|Timestamp 	| CRC Check
 * 		0x54  	| 1 byte	| LSB MSB	| LSB MSB		| 3 bytes per sample		| LSB MSB	|LSB MSB	| 1 byte
 * 				| tttmmmmm  |
 * 				| 00101100 	| [°/sec]	| [0.01°]		| distance(16) intensity(8)	| [0.01°]	|[ms]%30000	|
 * 				type=1
 * 				nb bytes = 12
*/
static running_e LD19_parse(char c, LiDARFrameTypeDef * packet)
{
	typedef enum{
				WAIT_HEADER,
				VERLEN,
				SPEED_LSB,
				SPEED_MSB,
				START_ANGLE_LSB,
				START_ANGLE_MSB,
				DATA_DISTANCE_LSB,
				DATA_DISTANCE_MSB,
				DATA_INTENSITY,
				END_ANGLE_LSB,
				END_ANGLE_MSB,
				TIME_STAMP_LSB,
				TIME_STAMP_MSB,
				CRC_CHECK
	}state_e;
	static state_e state = WAIT_HEADER;
	static uint8_t index_data = 0;
	static uint8_t crc = 0;
	running_e ret = IN_PROGRESS;

	if(state != CRC_CHECK)
	{
		crc = CrcTable[(crc ^ c) & 0xff];
	}

	switch(state)
	{
		case WAIT_HEADER:
			if(c == HEADER)
			{
				state = VERLEN;
				crc = CrcTable[HEADER];
			}
			break;
		case VERLEN:
			packet->ver_len = c;
			if((packet->ver_len & VER_MASK) >> 5 != 1)
			{
				ret = END_ERROR;
				state = WAIT_HEADER;
			}
			else
				state = SPEED_LSB;
			break;
		case SPEED_LSB:
			packet->speed = c;
			state = SPEED_MSB;
			break;
		case SPEED_MSB:
			packet->speed |= (uint16_t)(((uint16_t)c)<<8);
			state = START_ANGLE_LSB;
			break;
		case START_ANGLE_LSB:
			packet->start_angle = c;
			state = START_ANGLE_MSB;
			break;
		case START_ANGLE_MSB:
			packet->start_angle |= (uint16_t)(((uint16_t)c)<<8);
			packet->start_angle_rad = (float)((float)(packet->start_angle)*0.01*PI/180); //GEOMETRY_modulo_angle(-(int16_t)(((uint32_t)packet->start_angle)*183/256));	// °->rad4096 :    *0.01*PI4096/180
			state = DATA_DISTANCE_LSB;
			index_data = 0;
			break;
		case DATA_DISTANCE_LSB:
			packet->point[index_data].distance = c;
			state = DATA_DISTANCE_MSB;
			break;
		case DATA_DISTANCE_MSB:
			packet->point[index_data].distance |= (uint16_t)(((uint16_t)c)<<8);
			state = DATA_INTENSITY;
			break;
		case DATA_INTENSITY:
			packet->point[index_data].intensity = c;
			index_data++;
			if(index_data >= POINT_PER_PACK)
				state = END_ANGLE_LSB;
			else
				state = DATA_DISTANCE_LSB;
			break;
		case END_ANGLE_LSB:
			packet->end_angle = c;
			state = END_ANGLE_MSB;
			break;
		case END_ANGLE_MSB:
			packet->end_angle |= (uint16_t)(((uint16_t)c)<<8);
			//packet->end_angle_rad4096 = GEOMETRY_modulo_angle(-(uint16_t)(((uint32_t)packet->end_angle)*183/256));	// °->rad4096 :    *0.01*PI4096/180
			packet->end_angle_rad = (float)((float)(packet->end_angle)*0.01*PI/180);
			state = TIME_STAMP_LSB;
			break;
		case TIME_STAMP_LSB:
			packet->timestamp = c;
			state = TIME_STAMP_MSB;
			break;
		case TIME_STAMP_MSB:
			packet->timestamp |= (uint16_t)(((uint16_t)c)<<8);
			state = CRC_CHECK;
			break;
		case CRC_CHECK:
			if(c == crc)
			{
				float angle_step;
				angle_step = (packet->end_angle_rad - packet->start_angle_rad);	//en principe, cet angle est positif, car le capteur augmente ses angles en sens horaire !

				if(angle_step < 0) //on conditionne l'usage de GEOMETRY_modulo_angle_f au cas où le end_angle < start_angle
				{
					angle_step = GEOMETRY_modulo_angle_f(angle_step)/(12-1);
					for(uint8_t i = 0; i<POINT_PER_PACK; i++)
						packet->computed_angle_rad[i] =  GEOMETRY_modulo_angle_f(packet->start_angle_rad + angle_step*i);
				}
				else	//un peu crade, mais rentable d'un point de vue algorithmique
				{
					angle_step /=(12-1);
					for(uint8_t i = 0; i<POINT_PER_PACK; i++)
						packet->computed_angle_rad[i] =  (packet->start_angle_rad + angle_step*i);
				}

				ret = END_OK;
			}
			else
				ret = END_ERROR;
			state = WAIT_HEADER;
			break;
		default:
			state = WAIT_HEADER;
			break;
	}
	return ret;
}

#define DISPLAY_ONE_FRAME_OUT_OF_X		1	//100
#define DISPLAY_WITH_DETAILS	0
__attribute__((unused)) static void display_packet_infos(LiDARFrameTypeDef * packet)
{
	static uint16_t previous_time = 0;
	uint16_t delta_t;
	delta_t = packet->timestamp-previous_time;
	previous_time = packet->timestamp;
	static uint16_t filter_this_frame = 0;
	filter_this_frame = (filter_this_frame + 1) % DISPLAY_ONE_FRAME_OUT_OF_X;
	if(!filter_this_frame)	//on peut remplacer ceci par if(1) si on veut voir toutes les trames
	{

		debug_printf("dt=%d [%.1f->%.1f] - ", delta_t, packet->start_angle_rad, packet->end_angle_rad);

		for(uint8_t i = 0; i<POINT_PER_PACK; i++)
		{
			#if DISPLAY_WITH_DETAILS
				debug_printf("%.1f:%d(%d) ", packet->computed_angle_rad[i], packet->point[i].distance, packet->point[i].intensity);
			#else
				//if(packet->computed_angle_rad[i] < 0.5 )
				debug_printf("%d ", packet->point[i].distance);
				//debug_printf("%d\n", packet->start_angle);
			#endif
		}
		debug_printf("\n");
	}
}

//fonction indicative donnée par le fournisseur. Ce calcul est repris au fil de la lecture des octets.
__attribute__((unused)) static uint8_t CalCRC8(uint8_t *p, uint8_t len){
	uint8_t crc = 0;
	uint16_t i;
	for (i = 0; i < len; i++){
		crc = CrcTable[(crc ^ *p++) & 0xff];
	}
	return crc;
}

#endif
