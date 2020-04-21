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

/* The mqtt client struct */
extern struct mqtt_client client;

/* File descriptor */
extern struct pollfd fds;

/* Connected flag */
extern bool connected;





void pub_uart_mqtt(struct k_work *item);
void bsd_recoverable_error_handler(uint32_t err);
void data_print(u8_t *prefix, u8_t *data, size_t len);
int data_publish(struct mqtt_client *c, enum mqtt_qos qos, u8_t *data, size_t len);
int subscribe(void);
int publish_get_payload(struct mqtt_client *c, size_t length);
void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt);
void broker_init(void);
void client_init(struct mqtt_client *client);
int fds_init(struct mqtt_client *c);
void modem_configure(void);