/*
 * board.h
 *
 * Created: 02/05/2020 17:44:23
 *  Author: oyste
 */ 
#include <stdint.h>
#include <INA219.h>
#include <config.h>
#ifndef BOARD_H_
#define BOARD_H_

typedef struct{
	uint8_t id;
	uint8_t frame_counter;
	uint32_t time_stamp;
	uint8_t batteryLevel;
	ina219_t ina219;
	uint8_t lora_joined_err;
	uint16_t sample_size;
	char * msg;
}board_t;

int lora_joined_flag;
void board_charge(int off);
void board_setup(uint8_t *joined_err);
uint8_t board_get_battery_level(void);
#endif /* BOARD_H_ */