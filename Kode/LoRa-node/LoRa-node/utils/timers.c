/*
 * timers.c
 *
 * Created: 29.11.2019 15:05:42
 *  Author: oystmol
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <config.h>
#include "timers.h"


void timers_init(){
	#ifdef TIMER1
		TCCR1B = 0;
		TCCR1B |= (1 << WGM12) | (1 << CS12); // Setting mode 4 (CTC mode, OCR1A compare), prescalar 256
		TCNT1 = 0; // Initialize counter to 0
		OCR1A = 31250;
		TIMSK1 |= (1 << OCIE1A);
	#endif

	#ifdef TIMER2
		TIMSK2 = 0;
		ASSR = 0b00100000;
		_delay_ms(1000);
		TCCR2A = 0b00000000;
		TCCR2B = 0b00000101;
		//Synchronize registers
		while (ASSR & ( (1 << TCN2UB) | (1 << TCR2AUB) | (1 << TCR2BUB) )) ;
		TIFR2 = 0; //Clear interrupt flags
		TIMSK2 = (1 << TOIE2); //Enable overflow interrupt
		
	#endif
	
}


void enter_powerSave(){
	#ifdef USART2_START_COND_INTERRUPT
	UCSR2D |= (1<<SFDE) | (0<<RXSIE);			// Enable start condition detection with interrupt on RXCIE
	#endif
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_mode();
	_delay_ms(70);
	UCSR2D &= ~(1<<SFDE);						// Disable start condition detection while awake.
}




