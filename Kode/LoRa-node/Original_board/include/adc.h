#include <avr/io.h>
#include <avr/interrupt.h>


#define ADC_SCALING_CONSTANT 0.0025

// Channel selection for ADC conversion
#define ADC_CHANNEL_0					0x00
#define ADC_CHANNEL_1					0x01
#define ADC_CHANNEL_2					0x02
#define ADC_CHANNEL_3					0x03
#define ADC_CHANNEL_4					0x04
#define ADC_CHANNEL_5					0x05

// ADC voltage ref selection
#define ADC_VOLTAGE_REF_AREF			0x00
#define ADC_VOLTAGE_REF_AVCC			0x40
#define ADC_VOLTAGE_REF_INT11			0x80
#define ADC_VOLTAGE_REF_INT256			0xC0

// ADC prescalar selection
#define ADC_PRESCALAR_2					0x1
#define ADC_PRESCALAR_4					0x1
#define ADC_PRESCALAR_8					0x2
#define ADC_PRESCALAR_16				0x3
#define ADC_PRESCALAR_32				0x4
#define ADC_PRESCALAR_64				0x5
#define ADC_PRESCALAR_128				0x6

typedef uint16_t (*Read)(void);

typedef struct{
	Read rd;
	uint16_t regVal;
	
}adc_t;

void adc_init();
uint16_t adc_read();
void adc_change_channel(uint8_t channel);
uint16_t adc_getBatteryLevel();