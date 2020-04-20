
#include <USART.h>

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
#ifndef DEBUG_BAUD
	#define DEBUG_BAUD 9600UL
#endif

int USART_TRANSMIT_printf(char var, FILE *stream) {
	// translate \n to \r for br@y++ terminal
	if (var == '\n') USART_transmit2('\r');
	USART_transmit2(var);
	return 0;
}
/*Enabling printf:*/
FILE mystdout = FDEV_SETUP_STREAM(USART_TRANSMIT_printf, NULL, _FDEV_SETUP_RW);

unsigned char USART_receive(void){
	/* Wait for data to be received:*/
	while ( !(UCSR0A & (1<<RXC)) );
	/*Return data from buffer:*/
	return UDR0;
}

void  USART_transmit( uint8_t data ){
	/* Wait for empty transmit buffer:*/
	while ( !( UCSR0A & (1<<UDRE)));
	/* Put data into buffer:*/
	UDR0 = data;
}

void  USART_putstring(char *string){
	for(; *string; ++string){
		USART_transmit(*string);
	}
}
void  USART_putstring2(char *string){
	for(; *string; ++string){
		USART_transmit2(*string);
	}
}
void USART_init(){
	// Calculate ubbr:
	unsigned int ubrr =(F_CPU / (16*DEBUG_BAUD))-1U;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN)|(1<<TXEN);
	UCSR0C |= (3<<UCSZ0);	/* Set frame format:  1stop bit, 8data*/
	stdout = &mystdout;
}
void USART_init2(){
	// Calculate ubbr:
	unsigned int ubrr =(F_CPU / (16*DEBUG_BAUD))-1U;
	/*Set baud rate */
	UBRR2H = (unsigned char)(ubrr>>8);
	UBRR2L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR2B = (1<<RXEN)|(1<<TXEN);
	UCSR2C |= (3<<UCSZ0);	/* Set frame format:  1stop bit, 8data*/
	stdout = &mystdout;
}
void  USART_transmit2( uint8_t data ){
	/* Wait for empty transmit buffer:*/
	while ( !( UCSR2A & (1<<UDRE)));
	/* Put data into buffer:*/
	UDR2 = data;
}
