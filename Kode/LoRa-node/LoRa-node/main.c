#include <util/delay.h>
#include "FSM.h"
#include <avr/interrupt.h>
#include <USART.h>
#include <avr/wdt.h>
#include <config.h>
#include <RN2483A.h>
#include <board.h>
#include <INA219.h>
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

	DDRB |= (1<<LED1) | (1<<LED2) | (1<<LED3);
	FSM_run();
	puts("Program ended");
	
	//_delay_ms(400);
	//
	//uint8_t join;
	//board_setup(&join);
	//puts("hei");
	//uint16_t vbus,curr;
	//while(1){
		//PORTD |= (1<<PORTD4);
		//_delay_us(1);
		//vbus = INA219_readBusVoltageReg();
		//curr = INA219_readCurrentReg();
		//
		//printf("vbus: %.2f      curr: %.2f \n", vbus*0.004, curr*0.015);
		//
		//_delay_ms(1500);
	//}
	
	return 0;
}

