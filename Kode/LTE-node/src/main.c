/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <zephyr.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>	
#include <net/mqtt.h>
#include <net/socket.h>
#include <lte_lc.h>


#include "mqtt.h"

#include "buttons.h"

#include "uart.h"


struct mqtt_client client;
struct pollfd fds;
struct gpio_callback gpio_cb;
u8_t uart_buf[1024];
bool connected;
bool btn_press;


void main(void)
{
	int err;
	printk("The MQTT simple sample started\n");
	//struct device *gpiob;
        //gpiob = device_get_binding(PORT);
	for(int i=0; i < SERIAL_Q_SIZE; i++){
            k_work_init(&w_data[i].work, pub_uart_mqtt);
        }
	//struct device *uart;
	//uart = device_get_binding("UART_0");
	//uart_irq_callback_set(uart, uart_cb);
	//uart_irq_rx_enable(uart);
        //uart_sendCOM(uart2, "test\n");
        /*gpio_pin_configure(gpiob, PIN,
			   GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));

	gpio_add_callback(gpiob, &gpio_cb);
	gpio_pin_enable_callback(gpiob, PIN);
	*/
	

	modem_configure();

	client_init(&client);

	err = mqtt_connect(&client);
	if (err != 0) {
		printk("ERROR: mqtt_connect %d\n", err);
		return;
	}

	err = fds_init(&client);
	if (err != 0) {
		printk("ERROR: fds_init %d\n", err);
		return;
	}
        printk("The MQTT simple sample started\n");
	while (1) {
		err = poll(&fds, 1, mqtt_keepalive_time_left(&client));
		if (err < 0) {
			printk("ERROR: poll %d\n", errno);
			break;
		}

		err = mqtt_live(&client);
		if ((err != 0) && (err != -EAGAIN)) {
			printk("ERROR: mqtt_live %d\n", err);
			break;
		}

		if ((fds.revents & POLLIN) == POLLIN) {
			err = mqtt_input(&client);
			if (err != 0) {
				printk("ERROR: mqtt_input %d\n", err);
				break;
			}
		}
		
		if ((fds.revents & POLLERR) == POLLERR) {
			printk("POLLERR\n");
			break;
		}
		
		if ((fds.revents & POLLNVAL) == POLLNVAL) {
			printk("POLLNVAL\n");
			break;
		}
	}

	printk("Disconnecting MQTT client...\n");

	err = mqtt_disconnect(&client);
	if (err) {
		printk("Could not disconnect MQTT client. Error: %d\n", err);
	}
}
