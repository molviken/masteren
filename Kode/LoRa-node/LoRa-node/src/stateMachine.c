/*
 * stateMachine.c
 *
 * Created: 06.12.2019 16:06:11
 *  Author: oystmol
 */ 
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "stateMachine.h"
#include "adc.h"
#include "USART.h"
#include "RN2483A.h"
#include "util_functions.h"
#include "timers.h"




void stateMachine(){
	uint16_t adc_min_val = 0;
	stateMachine_init();
	while(1){
		switch (state){
			case INIT:	// This state attempt to connect to the TTN, and tries until completion.
				lora_join_OTAA();
				if (strncmp(join_request, "accepted",8)==0){
					state = ACTIVE;
				}
				else{
					printf("Attempting to connect...\n");
					lora_join_OTAA();
				}
				break;
			
			case SLEEP:
				printf("STATE SLEEP\n");
				enter_powerSave();
				state = ACTIVE;
				break;
			
			case ACTIVE:
				printf("STATE ACTIVE\n");
				uint16_t adc_val = adc_read();
				adc_min_val += adc_val;
				
				// Converts the averaged adv_min_val to hex-bytes and transmits if 60 seconds has passed
				if ((current_time)%60==0){
					
					// Processing the digital value
					adc_min_val /= 59;
					char t[10];
					float real_voltage = (adc_min_val * ADC_SCALING_CONSTANT)/2.5;
					uint16_t hex_val = f2uflt16(real_voltage);
					sprintf(t, "%x",hex_val);
					adc_min_val = 0;
					
					// Transmission
					int tx_status = lora_transmit(t);
					if (tx_status == 2){state = SLEEP;}
					else if(tx_status == 3){lora_assert_downlink(downlink_pairs);}
				
				}
				state = SLEEP;
				break;
		}
	}	
}

void stateMachine_init(){
	timer2_init();
	USART_init2();
	lora_auto_baud_detect(); // the LoRa chip has default baud-rate of 57600 and is changed with this function
	adc_init();
	lora_init();
		
	// Initializes the GPIO for controlling the transistors
	DDRD |= (1<<PD3);
	DDRB |= (1<<PB0);
	

}
