/*
 * FSM.c
 *
 * Created: 16/04/2020 14:16:33
 *  Author: oyste
 */ 

#include <FSM.h>
#include <util/delay.h>
#include <stdio.h>

enum States currentState = ST_INIT;




int FSM_init(){
	return 0;
}


void FSM_run(){
	if (FSM_init()) printf("Init failed, ending program...\n");
	
	while (1){
		switch (currentState){
			case ST_INIT:
				printf("init...\n");
				currentState = ST_IDLE;
				break;
			
			case ST_IDLE:
				printf("idle...\n");
				currentState = ST_INIT;
				break;
			default:
				printf("default...\n");
		}
		_delay_ms(3000);
	}
}