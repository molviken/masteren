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
#include <util_functions.h>
#include <string.h>
#include <avr/interrupt.h>
#include "FSM.h"

#define BOARD_CHARGE_OFF 1
#define BOARD_CHARGE_ON 0


States nextState = ST_INIT;


static volatile uint8_t wake_up_flag = 0;
static uint8_t frame_counter = 0;
static volatile int rec_flag = 0;
static void FSM_system_init(uint8_t *joined_flag);
static uint8_t tx_test_flag;

void FSM_system_init(uint8_t *joined_flag){
	board_setup(joined_flag);
	#ifdef DEBUG_M
	puts("Init complete, FSM running");
	#endif
}
void FSM_system_reset(){
	cli();	
	WDTCSR = 0x00;
	WDTCSR = (1<<WDE);
}
void FSM_assert_downlink(){
	uint32_t ts = 0;
	uint16_t sample_size_new;
	size_t str_len = strlen(downlink);
	#ifdef LORA_NODE
		if (str_len > 28) ascii_hex_decode(downlink, str_len, downlink_pairs, LORA_RX_PAYLOAD_OFFSET);
		else hex_decode(downlink, str_len, downlink_pairs, LORA_RX_PAYLOAD_OFFSET);
	
	#else
		hex_decode(downlink, str_len, downlink_pairs, 0);
	#endif
	//printf("%02X %02X %02X\n", downlink_pairs[0], downlink_pairs[1], downlink_pairs[2]);
	switch (downlink_pairs[0]){
		case 0x11:
			ts = (uint32_t)downlink_pairs[4] | ((uint32_t)downlink_pairs[3] << 8) | ((uint32_t)downlink_pairs[2] << 16) | ((uint32_t)downlink_pairs[1] << 24);
			current_time = ts;
			break;
		
		case 0x22:
			sample_size_new = (uint16_t)downlink_pairs[2] | ((uint16_t)downlink_pairs[1] << 8);
			sample_size = sample_size_new;
			break;
		
		case 0x99:
			FSM_system_reset();
			break;
	}
}

void FSM_run(void){
	data_receive_flag = 0;
	sample_size = BOARD_SAMPLE_SIZE;
	board_t board1 = {0, 100, {0, 0, 0, 0}, 0, sample_size, ""};
		
	int rejoin_attempts = 0;
	uint8_t lora_wait_rejoin_minutes = 3;
	
	while (1){
		if (data_receive_flag){
			data_receive_flag = 0;
			FSM_assert_downlink();
			
		}
		if (tx_test_flag){
			tx_test_flag = 0;
			printf("Transmit test\n");
			#ifdef LORA_NODE
				lora_transmit("025403210876");
			#else
				USART_putstring2("025403210876");
			#endif
		}
		switch (nextState){
			
			case ST_INIT:
				FSM_system_init(&board1.lora_joined_err);
				if (board1.lora_joined_err) nextState = ST_NOT_JOINED;
				else nextState = ST_ACTIVE;
				break;
			
			case ST_NOT_JOINED:
				set_bit(LEDS,LED1);
				#ifdef DEBUG_M
					puts("Not joined");
				#endif
				
				rejoin_attempts++;
				if (rejoin_attempts < 3) board1.lora_joined_err = lora_join_OTAA();
				else if (rejoin_attempts >= lora_wait_rejoin_minutes*60) rejoin_attempts = 0;
				
				
				if(board1.lora_joined_err) nextState = ST_ACTIVE;
				else nextState = ST_SLEEP;
				break;
		
			case ST_ACTIVE:
				clear_bit(LEDS,LED1);
				PORTB ^= (1<<LED2);
				#ifdef DEBUG_M
					puts("Active");
				#endif
				board1.ina219.bus_voltage_avg	+= INA219_readBusVoltageReg();
				board1.ina219.current_avg		+= INA219_readCurrentReg();
				board1.batteryLevel = board_get_battery_level();
				board1.sample_size = sample_size;
				if (board1.batteryLevel < 70) board_charge(BOARD_CHARGE_ON);
				else if (board1.batteryLevel > 95) board_charge(BOARD_CHARGE_OFF);
				if(!(current_time%board1.sample_size) && (current_time != 0)){
					#ifdef DEBUG_M
						puts("It's time");
					#endif
					if (frame_counter == 0xFF) frame_counter = 0;
					board1.frame_counter = frame_counter++;
					board1.ina219.bus_voltage	= board1.ina219.bus_voltage_avg / board1.sample_size;
					board1.ina219.current		= board1.ina219.current_avg / board1.sample_size;
					board1.msg = hex_encode(board1);
					#ifndef LORA_NODE
						puts(board1.msg);
					#else 
						#ifdef DEBUG_M
						puts(board1.msg);
						#endif
						lora_transmit(board1.msg);
					#endif
					free(board1.msg);
					board1.ina219.bus_voltage_avg = 0;
					board1.ina219.current_avg = 0;
				}
				nextState = ST_SLEEP;
				break;
			
			case ST_SLEEP:
				#ifdef DEBUG_M
					puts("Sleep");
				#endif
				_delay_ms(2); // Delay to allow a print to be written over serial before sleep
				
				#ifndef TIMER2
				_delay_ms(1000);
				#else
				//_delay_ms(1000);
				enter_powerSave();
				#endif
				if(board1.lora_joined_err) nextState = ST_NOT_JOINED;
				else nextState = ST_ACTIVE;
				break;
		}
	}
}

//ISR (INT1_vect)          //External interrupt_zero ISR
//{	
	////tx_test_flag = 1;
	//puts("button pressed");
//}

ISR (USART2_RX_vect){
	USART_receiveString2();
	data_receive_flag = 1;
}


ISR(TIMER2_OVF_vect){
	//puts("s");
	current_time ++;
}