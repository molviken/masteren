/*
 * board.h
 *
 * Created: 02/05/2020 17:44:23
 *  Author: oyste
 */ 
#include <stdint.h>
#include <INA219.h>
#ifndef BOARD_H_
#define BOARD_H_

typedef struct{
	uint8_t id;
	uint8_t frame_counter;
	uint32_t time_stamp;
	uint8_t batteryLevel;
	ina219_t ina219;
	
	char * msg;
}board_t;

int lora_joined_flag;
void board_charge(int on);
void board_setup(void);
uint8_t board_get_battery_level(void);
#endif /* BOARD_H_ */