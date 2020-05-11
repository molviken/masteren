#include <util/delay.h>
#include "FSM.h"
#include <USART.h>







int main(void){
	_delay_ms(400); // Start-up time after pressing reset button
	
	FSM_run();
	puts("Program ended");
	return 0;
}

