/*
 * Original_board.c
 *
 * Created: 03.06.2020 10:14:30
 * Author : oystmol
 */ 



#include <avr/io.h>
#include <util/delay.h>
#include <board.h>
#include <USART.h>
int main(void)
{
	//MCUSR &= ~(1 << WDRF);
	//wdt_disable();
	//_delay_ms(400); // Start-up time after pressing reset button
//
	//FSM_run();
	//puts("Program ended");
	
	
	_delay_ms(400);
	uint8_t joined_err, vbat;
	uint16_t vpanel;
	uint16_t curr; 
	board_setup(&joined_err);
	puts("hei");
	PORTD |= (1<<PORTD4);
	while (1){
		//puts("1");
		vbat = board_get_battery_level();
		//vpanel = board_get_panel_voltage();
		//curr = board_get_panel_current();
		printf("vbat: %u \n", vbat);
		//printf("vpan_f: %u\n", vpanel);
		//printf("curr_u: %u\n", curr);
		_delay_ms(1000);
	}
	return 0;
}

