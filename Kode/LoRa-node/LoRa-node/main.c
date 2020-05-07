#include <util/delay.h>
#include "FSM.h"








int main(void){
	_delay_ms(100); // Start-up time after pressing reset button
	
	FSM_run();
	puts("ended");
	return 0;
}

