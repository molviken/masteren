/*
 * FSM.c
 *
 * Created: 16/04/2020 14:16:33
 *  Author: oyste
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <INA219.h>
#include <RN2483A.h>
#include <timers.h>
#include <adc.h>
#include <USART.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <board.h>
#include <config.h>
#include <util_functions.h>


#include "FSM.h"



enum States currentState = ST_INIT;

static volatile uint8_t wake_up_flag = 0;
static volatile uint32_t current_time;
static uint8_t frame_counter = 0;
static volatile int rec_flag = 0;
//static void FSM_system_init(void);

void FSM_system_init(void){
	
	board_setup();
	puts("hi");
	#ifdef DEBUG_M
	puts("Init complete, FSM running");
	#endif
}



void FSM_run(void){
	
	board_t board1 = {DEV_ID, 0, 0, 100, {0, 0, 0, 0}};
	while (1){
		switch (currentState){
			case ST_INIT:
				FSM_system_init();
				currentState = ST_ACTIVE;
				break;
			
			case ST_ACTIVE:
				#ifdef LORA_NODE
				//if (!lora_joined_flag){wake_up_flag = 0; _delay_ms(180000); lora_join_OTAA();}
				#endif
				//puts("Active");
				if (wake_up_flag){
					wake_up_flag = 0;
					board1.ina219.bus_voltage_avg	+= INA219_readBusVoltageReg();
					board1.ina219.current_avg		+= INA219_readCurrentReg();
					board1.batteryLevel = board_get_battery_level();
					if(!(current_time%SAMPLE_SIZE)){
						if (frame_counter == 0xFF) frame_counter = 0;
						board1.frame_counter = frame_counter++;
						board1.ina219.bus_voltage	= board1.ina219.bus_voltage_avg / SAMPLE_SIZE;
						board1.ina219.current		= board1.ina219.current_avg / SAMPLE_SIZE;
						board1.time_stamp = current_time;
						board1.msg = hex_encode(board1);
						
						#ifndef LORA_NODE
							//USART_putstring2("500000FF5411");
							//puts(board1.msg);
						#else 
							lora_transmit(board1.msg);
						#endif
						board1.ina219.bus_voltage_avg = 0;
						board1.ina219.current_avg = 0;
					}
				}
				currentState = ST_SLEEP;
				break;
			
			case ST_SLEEP:
				puts("Going to sleep");
				_delay_ms(2); // Delay to allow a print to be written over serial before sleep
				enter_powerSave();
				puts("Awake");
				#ifndef TIMER2
				//_delay_ms(1000);
				#endif
				currentState = ST_ACTIVE;
				break;
		}
	}
}


ISR (INT1_vect)          //External interrupt_zero ISR
{
	puts("btn press");
	lora_transmit("11FF");
}

ISR (USART2_RX_vect){
	USART_receiveString2();
}

ISR(TIMER2_OVF_vect){
	current_time ++;
	wake_up_flag = 1;
}