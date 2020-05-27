
#include "USART.h"
#include <config.h>
#include <string.h>




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
	
	#ifdef UART1
		UBRR1H = (unsigned char)(ubrr>>8);
		UBRR1L = (unsigned char)ubrr;
		UCSR1B = (1<<RXEN)|(1<<TXEN);						// Enable receiver and transmitter
		UCSR1C |= (3<<UCSZ0);								// Set frame format:  1stop bit, 8data
		//stdout = &mystdout;
	#endif
	
	#ifdef UART2
		/* Set baud rate UART2 */
		UBRR2H = (unsigned char)(ubrr>>8);
		UBRR2L = (unsigned char)ubrr;
		UCSR2B = (1<<RXEN)|(1<<TXEN) | (1 << RXCIE);		// Enable receiver and transmitter 
		UCSR2C |= (3<<UCSZ0);								// Set frame format:  1stop bit, 8data
		stdout = &mystdout;
	#endif
	
	#ifdef UART1
	
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

void  USART_transmit1( uint8_t data ){
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

void USART_receiveString2(void){
	char rx_buf[250];
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
	#ifdef DEBUG_M
	printf("rx int: %s\n", rx_buf);
	#endif
	strcpy(downlink, rx_buf);
}

void USART_receiveString0(void){
	char rx_buf[250];
	unsigned char rec = 0;
	int i = 0;
	while (1){
		rec = USART_receive0();
		printf("%02x", rec);
		if (rec == 0x0A){
			rx_buf[i] = '\0';
			break;
		}
		rx_buf[i] = rec;
		i++;
	}
	#ifdef DEBUG_M
	printf("rx int: %s\n", rx_buf);
	#endif
	strcpy(downlink, rx_buf);
}