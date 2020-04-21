// /*
//  * Debug.c
//  *
//  * Created: 11.10.2018 13:27:34
//  *  Author: tobiasu
//  */ 
// 
// 
// #include "Debug.h"
// 
// #ifndef F_CPU
// 	#define F_CPU 8000000UL
// #endif
// 
// #ifndef DEBUG_BAUD
// 	#define DEBUG_BAUD 9600UL
// #endif
// 
// 
// int USART_TRANSMIT_printf(char var, FILE *stream) {
// 	// translate \n to \r for br@y++ terminal
// 	if (var == '\n') USART_transmit2('\r');
// 	USART_transmit2(var);
// 	return 0;
// }
// 
// 
// 
// /*Enabling prinf:*/
// FILE mystdout = FDEV_SETUP_STREAM(USART_TRANSMIT_printf, NULL, _FDEV_SETUP_RW);
// 
// 
// 
// unsigned char USART_receive0(void){
// 	/* Wait for data to be received:*/
// 	while ( !(UCSR0A & (1<<RXC)) );
// 	/*Return data from buffer:*/
// 	return UDR0;
// }
// unsigned char USART_receive2(void){
// 	/* Wait for data to be received:*/
// 	while ( !(UCSR2A & (1<<RXC)) );
// 	/*Return data from buffer:*/
// 	return UDR2;
// }
// void USART_receive_string(char *buffer){
// 	buffer[0] = USART_receive0;
// 	//buffer[1] = USART_receive0;
// 	//buffer[2] = USART_receive0;
// }
// 
// void  USART_transmit0( uint8_t data ){
// 	/* Wait for empty transmit buffer:*/
// 	while ( !( UCSR0A & (1<<UDRE)));
// 	/* Put data into buffer:*/
// 	UDR0 = data;
// }
// void  USART_transmit2( uint8_t data ){
// 	/* Wait for empty transmit buffer:*/
// 	while ( !( UCSR2A & (1<<UDRE)));
// 	/* Put data into buffer:*/
// 	UDR2 = data;
// }
// 
// void  USART_putstring(int chan, char *string){
// 	switch (chan){
// 		case 0:
// 			for(; *string; ++string){
// 				USART_transmit0(*string);
// 			}
// 			break;
// 		case 2:
// 			for(; *string; ++string){
// 				USART_transmit2(*string);
// 			}
// 			break;	
// 	}
// }
// void USART_init0(){
// 	// Calculate ubbr:
// 	unsigned int ubrr =(F_CPU / (16*DEBUG_BAUD))-1U;
// 	/*Set baud rate */
// 	UBRR0H = (unsigned char)(ubrr>>8);
// 	UBRR0L = (unsigned char)ubrr;
// 	/* Enable receiver and transmitter */
// 	UCSR0B = (1<<RXEN)|(1<<TXEN);
// 	/* Set frame format:  2stop bit, 8data*/
// 	UCSR0C |= /*(1<<USBS)|*/(3<<UCSZ0);
// 	stdout = &mystdout;
// 	//stdin = &mystdout;
// }
// void USART_init2(){
// 	// Calculate ubbr:
// 	unsigned int ubrr =(F_CPU / (16*DEBUG_BAUD))-1U;
// 	/*Set baud rate */
// 	UBRR2H = (unsigned char)(ubrr>>8);
// 	UBRR2L = (unsigned char)ubrr;
// 	/* Enable receiver and transmitter */
// 	UCSR2B = (1<<RXEN)|(1<<TXEN);
// 	/* Set frame format:  2stop bit, 8data*/
// 	UCSR2C = /*(1<<USBS)*/(3<<UCSZ0);
// 	//stdout = &mystdout;
// 	//stdin = &mystdout;
// }
// 
