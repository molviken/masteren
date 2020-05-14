#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

extern char downlink[100];

void USART_flush0();
void USART_init();
void USART_transmit0( uint8_t data );
unsigned char USART_receive0(void);
void USART_putstring0(char *string);
void USART_transmit2( uint8_t data );
int USART_TRANSMIT_printf(char var, FILE *stream);
void USART_receiveString2(void);
void  USART_putstring2(char *string);
unsigned char USART_receive2(void);
void USART_receiveString0(void);