
#include "USART.h"
#include <config.h>
#include <string.h>

static char rx_buf[30];

static void USART_transmit2( uint8_t data );

void USART_flush0(){
	unsigned char dmy;
	while ( !(UCSR0A & (1<<RXC)) ) dmy = UDR0;
}

int USART_TRANSMIT_printf(char var, FILE *stream) {
	// translate \n to \r for br@y++ terminal
	if (var == '\n') USART_transmit2('\r');
	USART_transmit2(var);
	return 0;
}
/*Enabling printf:*/
FILE mystdout = FDEV_SETUP_STREAM(USART_TRANSMIT_printf, NULL, _FDEV_SETUP_RW);



void USART_init(){
	unsigned int ubrr =(F_CPU / (16*DEBUG_BAUD))-1U;	// Calculate ubbr:
	
	#ifdef UART0
		/* Set baud rate UART0 */
		UBRR0H = (unsigned char)(ubrr>>8);
		UBRR0L = (unsigned char)ubrr;		
		UCSR0B = (1<<RXEN)|(1<<TXEN);						// Enable receiver and transmitter 
		UCSR0C |= (3<<UCSZ0);								// Set frame format:  1stop bit, 8data
	#endif
	
	#ifdef UART2
		/* Set baud rate UART2 */
		UBRR2H = (unsigned char)(ubrr>>8);
		UBRR2L = (unsigned char)ubrr;
		UCSR2B = (1<<RXEN)|(1<<TXEN) | (1 << RXCIE);		// Enable receiver and transmitter 
		UCSR2C |= (3<<UCSZ0);								// Set frame format:  1stop bit, 8data
		stdout = &mystdout;
	#endif
}

unsigned char USART_receive0(void){
	/* Wait for data to be received:*/
	while ( !(UCSR0A & (1<<RXC)) );
	/*Return data from buffer:*/
	return UDR0;
}

void  USART_transmit0( uint8_t data ){
	/* Wait for empty transmit buffer:*/
	while ( !( UCSR0A & (1<<UDRE)));
	/* Put data into buffer:*/
	UDR0 = data;
}



void  USART_putstring0(char *string){
	for(; *string; ++string){
		USART_transmit0(*string);
	}
}

void USART_transmit2( uint8_t data ){
	/* Wait for empty transmit buffer:*/
	while ( !( UCSR2A & (1<<UDRE)));
	/* Put data into buffer:*/
	UDR2 = data;
}
unsigned char USART_receive2(void){
	/* Wait for data to be received:*/
	while ( !(UCSR2A & (1<<RXC)) );
	/*Return data from buffer:*/
	return UDR2;
}

void  USART_putstring2(char *string){
	for(; *string; ++string){
		USART_transmit2(*string);
	}
	//USART_transmit2(0x0D);
	USART_transmit2(0x0A);
}

char * USART_receiveString2(void){
	//char rx_buf[250];
	unsigned char rec = 0;
	int i = 0;
	while (1){
		rec = USART_receive2();
		if (rec == 0x0A){
			rx_buf[i] = '\0';
			break;
		}
		rx_buf[i] = rec;
		i++;
	}
	puts(rx_buf);
	return &rx_buf[0];
}
