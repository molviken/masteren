#ifndef _I2CMASTER_H
#define _I2CMASTER_H   1
/************************************************************************* 
* Title:    C include file for the I2C master interface 
*           (i2cmaster.S or twimaster.c)
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: i2cmaster.h,v 1.10 2005/03/06 22:39:57 Peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device
**************************************************************************/



/*
 @brief I2C (TWI) Master Software Library
 Basic routines for communicating with I2C slave devices. This single master 
 implementation is limited to one bus master on the I2C bus. 
 This I2c library is implemented as a compact assembler software implementation of the I2C protocol 
 which runs on any AVR (i2cmaster.S) and as a TWI hardware interface for all AVR with built-in TWI hardware (twimaster.c).
 Since the API for these two implementations is exactly the same, an application can be linked either against the
 software I2C implementation or the hardware I2C implementation.
 Use 4.7k pull-up resistor on the SDA and SCL pin.
 
 Adapt the SCL and SDA port and pin definitions and eventually the delay routine in the module 
 i2cmaster.S to your target when using the software I2C implementation ! 
*/



#include <avr/io.h>

#define I2C_READ    1
#define I2C_WRITE   0

extern void i2c_init(void);

extern void i2c_stop(void);

extern unsigned char i2c_start(unsigned char addr);

extern unsigned char i2c_rep_start(unsigned char addr);

extern uint8_t i2c_start_wait(unsigned char addr);

extern unsigned char i2c_write(unsigned char data);

extern unsigned char i2c_readAck(void);

extern unsigned char i2c_readNak(void);

extern uint8_t i2c_write2ByteRegister(uint8_t adr, uint8_t regAddr, uint16_t data);

extern uint8_t i2c_read2ByteRegister(uint8_t adr, uint8_t regAddr, uint8_t *data);

/** 
 @brief    read one byte from the I2C device
 
 Implemented as a macro, which calls either i2c_readAck or i2c_readNak
 
 @param    ack 1 send ack, request more data from device<br>
               0 send nak, read is followed by a stop condition 
 @return   byte read from I2C device
 */
extern unsigned char i2c_read(unsigned char ack);
#define i2c_read(ack)  (ack) ? i2c_readAck() : i2c_readNak(); 

#ifdef __cplusplus
}
#endif

/**@}*/
#endif