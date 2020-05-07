#include <util/delay.h>

#include "adc.h"








void adc_init(){
	ADMUX = 0;
	ADCSRA = 0;
	ADMUX |= ADC_VOLTAGE_REF_AVCC;											// Sets adc voltage reference
	ADCSRA |= ADC_PRESCALAR_128;											// ADC prescaler = 128 ---> 8MHz goes to 62.5 kHz
	//ADCSRA |= (0 << ADIE);												// Enables ADC interrupt
	ADCSRA |= (1 << ADEN);													// Enabling the ADC
}


uint16_t adc_read(){
	uint16_t val;					
	ADCSRA |= (1 << ADSC);													// Starts single conversion
	while(!(ADCSRA & (1<<ADIF)));
	val = ADCW;
	if ((int)val > 1000){val = 1000;}
	return val;
}

void adc_change_channel(uint8_t channel){
	ADMUX |= channel;														// Setting adc channel to be read
	adc_read();																// Discard first measurement after channel change
	_delay_us(10);
}


