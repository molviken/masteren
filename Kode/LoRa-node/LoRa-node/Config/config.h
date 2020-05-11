/*
 * config.h
 *
 * Created: 29/04/2020 12:07:08
 *  Author: oyste
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif



/*******  TIMER FUNCTIONALITY			*******/
//#define TIMER1
#define TIMER2


/*******  LORA FUNCTIONALITY			*******/
#define LORA_NODE			// Uncomment for Lora functionality
#ifdef LORA_NODE
	#define OTAA
	#define LORA_DR 1
	#ifdef LORA_DR
		#define LORA_ADR 0
	#endif
	#define LORA_BAND 868
	#define LORA_PWRIDX 1
#endif


/*******  PRINT/USART FUNCTIONALITY		*******/
#define UART0
#define UART2
#define USART2_START_COND_INTERRUPT				// Enables the RX interrupt from sleep to wake MCU up
#define DEBUG_M									// Comment for release without any prints
#ifndef DEBUG_BAUD
	#define DEBUG_BAUD 9600UL
#endif


/*******  Bit operations				*******/
#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))




// Custom board settings
#define DEV_ID 0x02								// Id for board
#define SAMPLE_SIZE 600							// How many 1 sec samples before averaging and transmitting
#define LED1 PORTB0
#define LED2 PORTB1
#define LED3 PORTB2
#define GYRO_INT PORTD2
#define DMY_BTN PORTD3
#define PHOTO_INTENSITY ADC_CHANNEL0
