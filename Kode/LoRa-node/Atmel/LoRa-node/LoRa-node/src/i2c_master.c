/*************************************************************************
* Title:    I2C master library using hardware TWI interface
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: twimaster.c,v 1.3 2005/07/02 11:14:21 Peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device with hardware TWI 
* Usage:    API compatible with I2C Software Library i2cmaster.h
**************************************************************************/
#include <inttypes.h>
#include <util/twi.h>
#include <util/delay.h>
#include "i2c_master.h"
#include <stdio.h>

#define SCL_CLOCK 100000L


/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void i2c_init(void){
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  
  TWSR1 = 0;                         /* no prescaler */
  TWBR1 = (uint8_t)(((F_CPU/SCL_CLOCK)-16)/2);  /* must be > 10 for stable operation */

}

uint8_t i2c_sync(void){
	uint16_t timeout = 100; 
	while(!(TWCR1 & (1<<TWINT)) && timeout) {
		_delay_us(1); 
		timeout--; 
	}
	if (!timeout){
		TWCR1 = 0x00;
	}
	return timeout != 0; 
}

uint8_t i2c_waitStop(void){
	uint16_t timeout = 100; 
	while((TWCR1 & (1<<TWSTO)) && timeout) {
		_delay_us(1); 
		timeout--; 
	}
	if (!timeout){
		TWCR1 = 0x00;
	}
	return timeout != 0; 
}
/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char i2c_start(unsigned char address){
    uint8_t   twst;

	// send START condition
	TWCR1 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//printf("TWCR1: %u\n",TWCR1);
	// wait until transmission completed (this is stupid!!)
	if(!i2c_sync()) return 0; 

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR1 = address;
	TWCR1 = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	if(!i2c_sync()) return 0; 
	
	// check value of TWI Status Register. Mask               prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;
}


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
uint8_t i2c_start_wait(unsigned char address)
{
    uint8_t   twst;

    int retry = 2000; 
	printf("retry: %d\n", retry);
    while ( 1 )
    {
	    // send START condition
	    TWCR1 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    	// wait until transmission completed
    	if(!i2c_sync()) break; 
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
    	// send device address
    	TWDR1 = address;
    	TWCR1 = (1<<TWINT) | (1<<TWEN);
    
    	// wail until transmission completed
    	if(!i2c_sync()) break; 
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
    	{    	    
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR1 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        
	        // wait until stop condition is executed and bus released
	        if(!i2c_waitStop()) continue; 

	        if(!(retry --)) break;  
    	    continue;
    	}
    	return 1; 
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }
	 printf("retry: %d\n", retry);
	return 0; 
}


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 
 Input:   address and transfer direction of I2C device
 
 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address){
    return i2c_start( address );

}


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void){
    /* send stop condition */
	TWCR1 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	i2c_waitStop(); 

}


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   0 write successful 
            1 write failed
*************************************************************************/
unsigned char i2c_write( unsigned char data ){	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR1 = data;
	TWCR1 = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	i2c_sync();

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void){
	TWCR1 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	i2c_sync();    
	return TWDR1;
}


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void){
	TWCR1 = (1<<TWINT) | (1<<TWEN);
	i2c_sync(); 
	return TWDR1;
}


/*************************************************************************
  Read a 2-byte register from I2C device
  
  Input:    device address, register address, buffer for storage
  Return:   0 write successful 
            1 write failed
*************************************************************************/
uint8_t i2c_read2ByteRegister(uint8_t adr, uint8_t regAddr, uint8_t *data){
	uint8_t error = 0;
	error += i2c_start(adr | I2C_WRITE);
	error += i2c_write(regAddr);
	_delay_us(10);
	//read data
	error += i2c_start(adr | I2C_READ);
	data[1] = i2c_readAck();
	data[0] = i2c_readNak();
	i2c_stop();
	
	if (error) return 1;
	else return 0;
}

/*************************************************************************
  Write 2 bytes to a 2-byte register in I2C device
  
  Input:    device address, register address, bytes to be transfered 
  Return:   0 write successful
			1 write failed
*************************************************************************/
uint8_t i2c_write2ByteRegister(uint8_t adr, uint8_t regAddr, uint16_t data){
	uint8_t error = 0;
	error += i2c_start(adr | I2C_WRITE);
	error += i2c_write(regAddr);
	_delay_us(10);
	error += i2c_write((uint8_t)(data >> 8));
	error += i2c_write((uint8_t)(data & 0xFF));
	i2c_stop();
	if (error) return 1;
	else return 0;
}