#include <util/delay.h>
#include "FSM.h"
#include <USART.h>
#include <avr/wdt.h>
#include <config.h>
#include <RN2483A.h>

void lora_auto_baud_detect(void){
	DDRD |= (1<<PORTD1);
	PORTD &= ~(1<<PORTD1);
	_delay_ms(10);
	DDRD = 0x00;
	_delay_us(10);
	USART_init();
	USART_transmit0(0x55);
	_delay_ms(100);
}


int main(void){
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	_delay_ms(400); // Start-up time after pressing reset button

	FSM_run();
	puts("Program ended");
	//USART_init();
	//DDRB |= (1<<LED1) | (1<<LED2) | (1<<LED3);
	////lora_auto_baud_detect();
	////puts("lora baud detected");
	////uint8_t err = 0;
	////err = lora_test_command();
	////if (err) puts("failed");
	//while(1){
		//PORTB ^= (1<<LED1);
		//lora_test_command();
		//_delay_ms(100);
	//}
	return 0;
}

