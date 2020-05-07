#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>



void USART_init();
void USART_transmit0( uint8_t data );
unsigned char USART_receive0(void);
void USART_putstring0(char *string);

int USART_TRANSMIT_printf(char var, FILE *stream);
char * USART_receiveString2(void);
void  USART_putstring2(char *string);
unsigned char USART_receive2(void);