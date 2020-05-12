#include <util/delay.h>
#include "FSM.h"
#include <USART.h>
#include <avr/wdt.h>






int main(void){
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	_delay_ms(400); // Start-up time after pressing reset button
	
	FSM_run();
	puts("Program ended");
	return 0;
}

