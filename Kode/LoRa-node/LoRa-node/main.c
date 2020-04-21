
#include <util/delay.h>
#include <avr/interrupt.h>
#include <INA219.h>
#include <USART.h>
#include <i2c_master.h>
#include <FSM.h>


int main(void){
	
	USART_init2();
	printf("Hello world\n");
	
	ina219 dev40;
	i2c_init();
	INA219_init();
	float current = 0;
	float busVoltage;
	sei();
	
	FSM_run();
	
	
}

