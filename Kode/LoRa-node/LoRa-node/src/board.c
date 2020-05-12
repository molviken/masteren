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
#include <i2c_master.h>
#include <INA219.h>
#include "board.h"





static void board_dmy_btn_init();
static void board_gpio_init();

void board_dmy_btn_init(void){
	DDRD &= ~(1 << DDD3);     // Clear the PD2 pin
	EICRA |= (1 << ISC11);    // set INT1 to trigger on falling edge
	EIMSK |= (1 << INT1);     // Turns on INT1
}

void board_gpio_init(void){
	DDRD |= (1 << DDRD4);	  // Set PD4 as output
	
}

void board_charge(int off){
	if (off){
		clear_bit(PORTD, PORTD4);
		clear_bit(LEDS, LED1);
	}
	else {
		set_bit(PORTD, PORTD4);
		set_bit(LEDS, LED1);
	}
}

void board_setup(uint8_t *joined_err){
	puts("test1");
	#ifndef LORA_NODE
	USART_init();
	#endif
	adc_init();
	board_dmy_btn_init();
	board_gpio_init();
	board_charge(0);
	puts("test2");
	#ifdef LORA_NODE
		lora_init(joined_err);
		
	#else
		*joined_err = 0;
	#endif

	i2c_init();
	INA219_init();
	timers_init();
	sei();
}

uint8_t board_get_battery_level(void){
	uint16_t val = 0;
	float r1, r2;
	float battery_max, v_div_tick;
	float vbat;
	int bat_percentage;
	
	r1 = 174;
	r2 = 255;
	battery_max = 4.2;
	
	adc_change_channel(VBAT);
	val = adc_read();
	
	v_div_tick = (((r2)/(r1+r2))*battery_max) / 1024;
	vbat = v_div_tick*val*(r1+r2)/r2;
	bat_percentage = vbat*100/battery_max;
	
	return bat_percentage;
}