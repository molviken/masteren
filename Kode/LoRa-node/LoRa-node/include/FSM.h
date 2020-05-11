/*
 * FSM.h
 *
 * Created: 16/04/2020 14:16:42
 *  Author: oyste
 */ 


#ifndef FSM_H_
#define FSM_H_



typedef enum {
	ST_INIT,
	ST_SLEEP,
	ST_ACTIVE,
	ST_NOT_JOINED
}States;

volatile uint32_t current_time;


void FSM_run();
//void FSM_system_init(uint8_t *joined_flag);

#endif /* FSM_H_ */