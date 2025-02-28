#include "config.h"
#include "macro_types.h"
#include "stm32f1xx.h"
#include "stm32f1_timer.h"
#include "stm32f1_stepper_motor.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"

/*
Comment utiliser ce module logiciel ? (qui est encore en phase de d√©veloppement... il faut donc bien le comprendre pour s'en servir !)
1- d√©finir le nombre de moteurs dans le fichier header, en adaptant l'enumeration motor_id_e.
2- adapter ci-dessous les tableaux enable_pins, dir_pins, pulse_pins qui indiquent les broches de pilotage des moteurs pas √† pas.

*/

#if USE_STEPPER_MOTOR

#ifndef GPIO_STEPPER_MOTOR
	#define GPIO_STEPPER_MOTOR			GPIOB
#endif

#ifndef STEPPER_MOTOR_TIMER
	#define STEPPER_MOTOR_TIMER					TIMER2_ID
	#define STEPPER_MOTOR_timer_irq_handler		TIMER2_user_handler_it
#endif

#ifndef STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_OPENDRAIN_OUTPUTS
	#define STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_OPENDRAIN_OUTPUTS 0
#endif
#ifndef STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET
	#define STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET 1
#endif

#if (STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_OPENDRAIN_OUTPUTS + STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET != 1)
	#warning "vous devez choisir l'un de ces deux modes, RDV dans config.h !"
#endif

#if (STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET == 1)
	#define SLEEPING_STATE	0			//avec des mosfets pour piloter les entrÈes "PUL-", "EN-", "DIR-" du TB6600, la logique est inversÈe.
#else
	#define SLEEPING_STATE	1			//avec une liaison directe pour piloter les entrÈes "PUL-", "EN-", "DIR-" du TB6600, un "1" bascule la sortie en opendrain, la led de l'optocoupleur n'est plus pilotÈe.
#endif


#define DEFAULT_IT_PERIOD 100			//p√©riode de l'interruption qui g√©n√®re les pulses.

											//	 X   Y
static const uint8_t enable_pins[STEPPER_MOTOR_NB] = 	{11, 10};	//d√©finir ici les num√©ros de broches des enables des moteurs  (sur le GPIO unique GPIO_STEPPER_MOTOR)
static const uint8_t dir_pins[STEPPER_MOTOR_NB] = 		{14, 12};	//ici les broches des directions
static const uint8_t pulse_pins[STEPPER_MOTOR_NB] = 	{15, 13};	//ici les broches des pulse

static volatile int32_t positions[STEPPER_MOTOR_NB] = {0};
static volatile int32_t goals[STEPPER_MOTOR_NB] = {0};
static volatile uint32_t pulse_period[STEPPER_MOTOR_NB] = {10, 10};	//"vitesse" par d√©faut (p√©riode par d√©faut entre deux pulses)
static callback_fun_t callback_at_each_pulse = NULL;

static void STEPPER_MOTOR_pin_set(uint32_t pin, bool_e b);


/////////////////////Fonctions publiques///////////////////////


void STEPPER_MOTOR_init(void)
{
	for(motor_id_e m=0; m<STEPPER_MOTOR_NB; m++)
	{
		uint32_t gpio_mode;
		#if STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET
			gpio_mode = GPIO_MODE_OUTPUT_PP;
		#else
			gpio_mode = GPIO_MODE_OUTPUT_OD;
		#endif
		//Configurer les sorties
		BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, 1<<enable_pins[m],	gpio_mode,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
		BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, 1<<dir_pins[m], 	gpio_mode,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
		BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, 1<<pulse_pins[m],	gpio_mode,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

		//Appliquer l'Ètat de repos sur toutes les sorties
		STEPPER_MOTOR_pin_set(enable_pins[m], SLEEPING_STATE);
		STEPPER_MOTOR_pin_set(dir_pins[m], SLEEPING_STATE);
		STEPPER_MOTOR_pin_set(pulse_pins[m], SLEEPING_STATE);
	
		STEPPER_MOTOR_enable(m, TRUE);
	}
	
	TIMER_run_us(STEPPER_MOTOR_TIMER, DEFAULT_IT_PERIOD, TRUE);
}

void STEPPER_MOTOR_demo(void)
{
	STEPPER_MOTOR_init();

	bool_e toggle = FALSE;
	while(1)
	{
		if(STEPPER_MOTOR_is_arrived(STEPPER_MOTOR_0))
		{
			STEPPER_MOTOR_set_goal(STEPPER_MOTOR_0,(toggle)?1000:-1000);
			toggle = !toggle;
		}
	}
}


void STEPPER_MOTOR_set_callback_at_each_pulse(callback_fun_t cb)
{
	callback_at_each_pulse = cb;
}

void STEPPER_MOTOR_enable(motor_id_e id, bool_e enable)
{
	STEPPER_MOTOR_pin_set(enable_pins[id], (!enable)^SLEEPING_STATE);
}



void STEPPER_MOTOR_set_goal(motor_id_e id, int32_t newgoal)
{
	if(id<STEPPER_MOTOR_NB)
		goals[id] = newgoal;

}

int32_t STEPPER_MOTOR_get_goal(motor_id_e id)
{
	if(id<STEPPER_MOTOR_NB)
		return goals[id];
	return 0;
}

int32_t STEPPER_MOTOR_get_position(motor_id_e id){
	return positions[id];
}

void STEPPER_MOTOR_set_position(motor_id_e id, int32_t newposition)
{
	if(id<STEPPER_MOTOR_NB)
		positions[id] = newposition;
}

bool_e STEPPER_MOTOR_is_arrived (motor_id_e id){
	return (goals[id]==positions[id])?TRUE:FALSE;
}



/////////////////////Fonctions privÈes///////////////////////


static void STEPPER_MOTORS_do_pulse(motor_id_e id)
{
	STEPPER_MOTOR_pin_set(pulse_pins[id], !SLEEPING_STATE);
	Delay_us(50);
	STEPPER_MOTOR_pin_set(pulse_pins[id], SLEEPING_STATE);
	if(callback_at_each_pulse != NULL)
		callback_at_each_pulse();
}

static void STEPPER_MOTORS_set_dir(motor_id_e id, bool_e direction)
{
	STEPPER_MOTOR_pin_set(dir_pins[id], direction^SLEEPING_STATE);
}


static void STEPPER_MOTOR_pin_set(uint32_t pin, bool_e b)
{
	#if STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_OPENDRAIN_OUTPUTS
		if(pin<8)
		{
			if(b)
				GPIO_STEPPER_MOTOR->CRL &= (uint32_t)(~(3<<(4*(pin))));
			else
				GPIO_STEPPER_MOTOR->CRL |= (3<<(4*(pin)));
		}
		else
		{
			if(b)
				GPIO_STEPPER_MOTOR->CRH &= (uint32_t)(~(3<<(4*(pin - 8))));
			else
				GPIO_STEPPER_MOTOR->CRH |= (3<<(4*(pin - 8)));
		}
	#endif
	#if STEPPER_DRIVER_TB6600_IS_DRIVEN_BY_MOSFET
		if(b)
			GPIO_STEPPER_MOTOR->BSRR = (1<<pin);
		else
			GPIO_STEPPER_MOTOR->BSRR = (uint32_t)(1<<pin<<16);
	#endif

}




void STEPPER_MOTOR_timer_irq_handler (){
	static uint32_t periods[STEPPER_MOTOR_NB] = {0};

	for(motor_id_e m=0; m<STEPPER_MOTOR_NB; m++)
	{
		periods[m] = (uint32_t)((uint32_t)(periods[m]+1)%pulse_period[m]);
		if (periods[m] == 0)
		{
			if (positions[m] < goals[m])
			{
					positions[m]++;
					STEPPER_MOTORS_set_dir(m, FALSE);
					STEPPER_MOTORS_do_pulse(m);
			}
			else if (positions[m] > goals[m])
			{
					positions[m]--;
					STEPPER_MOTORS_set_dir(m, TRUE);
					STEPPER_MOTORS_do_pulse(m);
			}
		}
	}
}


#endif
