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
#include <string.h>

#include "FSM.h"

#define BOARD_CHARGE_OFF 1
#define BOARD_CHARGE_ON 0


States nextState = ST_INIT;

static volatile uint8_t wake_up_flag = 0;
static uint8_t frame_counter = 0;
static volatile int rec_flag = 0;
static void FSM_system_init(uint8_t *joined_flag);

void FSM_system_init(uint8_t *joined_flag){
	board_setup(joined_flag);
	#ifdef DEBUG_M
	puts("Init complete, FSM running");
	#endif
}


//const char* test = "3565623838383532";
//const char* test2 = "mac_rx 2 63633131353562626666"; 
char test3[50] = "mac_rx 2 63633131353562626666";
uint8_t out[100];
void FSM_run(void){
	board_t board1 = {DEV_ID, 0, 0, 100, {0, 0, 0, 0}, 0, SAMPLE_SIZE, ""};
	int rejoin_attempts = 0;
	uint8_t lora_wait_rejoin_minutes = 3;
	while (1){
		switch (nextState){
			case ST_INIT:
				FSM_system_init(&board1.lora_joined_err);
				
				//ascii_hex_decode(test3, strlen(test3), out, LORA_RX_PAYLOAD_OFFSET);
				//puts(downlink);
				if (board1.lora_joined_err) nextState = ST_NOT_JOINED;
				else nextState = ST_ACTIVE;
				break;
			
			case ST_NOT_JOINED:
				#ifdef DEBUG_M
					puts("Not joined");
				#endif
				
				rejoin_attempts++;
				if (rejoin_attempts < 3) board1.lora_joined_err = lora_join_OTAA();
				else if (rejoin_attempts >= lora_wait_rejoin_minutes*60) rejoin_attempts = 0;

				nextState = ST_SLEEP;
				break;
			
			case ST_ACTIVE:
				#ifdef DEBUG_M
					//puts("Active");
				#endif
				
				board1.ina219.bus_voltage_avg	+= INA219_readBusVoltageReg();
				board1.ina219.current_avg		+= INA219_readCurrentReg();
				//printf("bus avg: %lu     curr_avg: %lu \n", board1.ina219.bus_voltage_avg, board1.ina219.current_avg);
				board1.batteryLevel = board_get_battery_level();
				if (board1.batteryLevel < 60) board_charge(BOARD_CHARGE_ON);
				else if (board1.batteryLevel > 95) board_charge(BOARD_CHARGE_OFF);
				if(!(current_time%board1.sample_size) && (current_time != 0)){
					if (frame_counter == 0xFF) frame_counter = 0;
					board1.frame_counter = frame_counter++;
					board1.ina219.bus_voltage	= board1.ina219.bus_voltage_avg / SAMPLE_SIZE;
					board1.ina219.current		= board1.ina219.current_avg / SAMPLE_SIZE;
					board1.time_stamp = current_time;
					board1.msg = hex_encode(board1);
						
					#ifndef LORA_NODE
						puts(board1.msg);
					#else 
						//puts(board1.msg);
						lora_transmit(board1.msg);
					#endif
					board1.ina219.bus_voltage_avg = 0;
					board1.ina219.current_avg = 0;
				}
				nextState = ST_SLEEP;
				break;
			
			case ST_SLEEP:
				#ifdef DEBUG_M
				//puts("Going to sleep");
				#endif
				_delay_ms(2); // Delay to allow a print to be written over serial before sleep
				enter_powerSave();
				#ifndef TIMER2
				_delay_ms(1000);
				#endif
				if(board1.lora_joined_err) nextState = ST_NOT_JOINED;
				else nextState = ST_ACTIVE;
				break;
		}
	}
}

ISR (INT1_vect)          //External interrupt_zero ISR
{
	lora_transmit("020255b17f355403210876");
}

ISR (USART2_RX_vect){
	USART_receiveString2();
}

ISR(TIMER2_OVF_vect){
	current_time ++;
	
	//wake_up_flag = 1;
}