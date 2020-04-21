
#include <drivers/uart.h>
#include <inttypes.h>
#include <string.h>

#define SERIAL_Q_SIZE 5

static struct work_data{    
    struct k_work work;    
    u8_t data[1024]; 
    u8_t data_len;
}w_data[SERIAL_Q_SIZE]; 


extern u8_t uart_buf[1024];




void uart_sendCOM(struct device *x,  u8_t *Cont);
void uart_cb(struct device *x);