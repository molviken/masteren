/*
 * board.h
 *
 * Created: 02/05/2020 17:44:23
 *  Author: oyste
 */ 
#include <stdint.h>
#include <config.h>
#ifndef BOARD_H_
#define BOARD_H_

typedef struct{
	uint8_t frame_counter;
	uint8_t batteryLevel;
	uint16_t curr;
	uint16_t vpanel;
	uint8_t lora_joined_err;
	uint16_t sample_size;
	char * msg;
}board_t;

int lora_joined_flag;
void board_charge(int off);
void board_setup(uint8_t *joined_err);
uint8_t board_get_battery_level(void);
uint16_t board_get_panel_voltage(void);
uint16_t board_get_panel_current(void);
#endif /* BOARD_H_ */