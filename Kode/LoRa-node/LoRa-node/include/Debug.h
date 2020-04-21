// /*
//  * Debug.h
//  *
//  * Created: 11.10.2018 13:27:46
//  *  Author: tobiasu
//  */ 
// 
// 
// #ifndef DEBUG_H_
// #define DEBUG_H_
// 
// 
// 
// #include <avr/io.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdlib.h>
// 
// 
// /*Uart for debug:*/
// 
// void USART_init0();
// void USART_init2();
// void USART_transmit0( uint8_t data );
// void USART_transmit2( uint8_t data );
// unsigned char USART_receive0(void);
// unsigned char USART_receive2(void);
// void USART_receive_string(char *buffer);
// void USART_putstring(int chan, char *string);
// int USART_TRANSMIT_printf(char var, FILE *stream);
// 
// 
// 
// 
// 
// 
// #endif /* DEBUG_H_ */