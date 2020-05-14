/*
 * FSM.h
 *
 * Created: 16/04/2020 14:16:42
 *  Author: oyste
 */ 
#include <config.h>
#include <stddef.h>
#ifndef FSM_H_
#define FSM_H_



typedef enum {
	ST_INIT,
	ST_SLEEP,
	ST_ACTIVE,
	ST_NOT_JOINED,
	ST_DATA_RECEIVED
}States;

volatile uint32_t current_time;
uint16_t sample_size;
uint8_t downlink_pairs[100];
char downlink[100];
volatile uint8_t data_receive_flag;


void FSM_system_reset();
void FSM_run();
void FSM_assert_downlink();

//void FSM_system_init(uint8_t *joined_flag);

#endif /* FSM_H_ */