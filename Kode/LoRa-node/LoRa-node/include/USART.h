#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>


/*Uart for debug:*/

void USART_init();
void USART_transmit( uint8_t data );
unsigned char USART_receive(void);
void USART_putstring(char *string);

int USART_TRANSMIT_printf(char var, FILE *stream);

void USART_init2();
void USART_transmit2( uint8_t data );
void  USART_putstring2(char *string);