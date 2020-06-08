/*
 * board.c
 *
 * Created: 02/05/2020 17:44:10
 *  Author: oyste
 */ 
#include <avr/io.h>
#include <config.h>
#include <adc.h>
#include <USART.h>
#include <RN2483A.h>
#include <timers.h>
#include "board.h"



static void board_gpio_init();


void board_gpio_init(void){
	DDRD |= (1 << DDRD4);	  // Set PD4 as output
	DDRB |= (1 << PORTB0);
	DDRB |= (1 << PORTB1);
	DDRB |= (1 << PORTB2);
}

void board_charge(int off){
	if (off){
		set_bit(PORTD, PORTD4);
	}
	else {
		clear_bit(PORTD, PORTD4);
	}
}

void board_setup(uint8_t *joined_err){
	#ifndef LORA_NODE
	USART_init();
	#endif
	adc_init();
	board_gpio_init();
	#ifdef LORA_NODE
		lora_init(joined_err);
	#else
		*joined_err = 0;
	#endif
	timers_init();
	sei();
}

uint16_t board_get_panel_voltage(void){
	uint16_t val = 0;
	
	adc_change_channel(VPANEL_CHANNEL);
	val = adc_read();
	if (val > 989) val = 989;
	
	return val;
}

uint16_t board_get_panel_current(void){
	uint16_t val = 0;
	
	adc_change_channel(VCURR_CHANNEL);
	val = adc_read();
	//if (val > 989) val = 989;
	
	return val;
}


uint8_t board_get_battery_level(void){
	uint16_t val = 0;
	float r1, r2;
	float battery_max, v_div_tick;
	float vbat;
	int bat_percentage;
	
	r1 = 41.2;
	r2 = 150;
	battery_max = 4.2;
	
	adc_change_channel(VBAT_CHANNEL);
	val = adc_read();
	
	v_div_tick = (((r2)/(r1+r2))*battery_max) / 1024;
	vbat = v_div_tick*val*(r1+r2)/r2;
	bat_percentage = vbat*100/battery_max;
	
	return bat_percentage;
}