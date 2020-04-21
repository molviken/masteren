#include "uart.h"



// UART transmitt
void uart_sendCOM(struct device *x,  u8_t *Cont)
{
	       
         u16_t len = strlen(Cont); 
         uart_fifo_fill(x, Cont,len );
         uart_irq_tx_enable(x);
    
}

// UART callback

/*void uart_cb(struct device *x)
{
	uart_irq_update(x);
	int data_length = 0;

	if (uart_irq_rx_ready(x)) {
		data_length = uart_fifo_read(x, uart_buf, sizeof(uart_buf));
		uart_buf[data_length] = 0;
	}

        printk("%s", uart_buf);
}
*/
///*
void uart_cb(struct device *x)
{
	uart_irq_update(x);
	int data_length = 0;
        static int pos, serial_in_count, tot_len = 0;

	if (uart_irq_rx_ready(x)) {
		data_length = uart_fifo_read(x, uart_buf, sizeof(uart_buf));
                tot_len = tot_len + data_length;

                memcpy(&w_data[serial_in_count].data[pos], uart_buf, data_length);
                printk("Position is: %d\n", pos);
                //Check if last char is 'LF'
                if(uart_buf[data_length-1] == 10){
                      printk("last char is 'LF', call k_work_submit(), word number %d \n", serial_in_count);
                      w_data[serial_in_count].data_len = tot_len - 1;
                      k_work_submit(&w_data[serial_in_count].work);

                      pos = 0;
                      tot_len = 0;
                      
                      if(serial_in_count < SERIAL_Q_SIZE-1){
                        serial_in_count++;
                      }else{
                          serial_in_count=0;
                      }
                }else{
                      printk("last char is: %d\n", uart_buf[data_length-1]);
                      pos = pos + data_length;
                }
                  


	}
	printk("%s", uart_buf);
}
//*/