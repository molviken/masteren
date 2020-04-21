/*
 * RN2483A.c
 *
 * Created: 25.11.2019 12:54:54
 *  Author: oystmol
 */ 
#include "RN2483A.h"
#include "USART.h"
#include <string.h>
const char *appEui = "70B3D57ED0026360";
const char *appKey = "549DA4B607DDCB74EA02031171290CD7";




void lora_send_command(char * cmd){
	USART_putstring(cmd);
	USART_transmit(0x0D);
	USART_transmit(0x0A);
}
void lora_receive_response(){
	char buf[250];
	unsigned char rec;
	int i = 0;
	while (1){
		rec = USART_receive();
		buf[i] = rec;
		if (rec == 0x0A){break;}
		i++;
	}
	USART_putstring2(buf);
}

void lora_set_appeui(){
	char* temp = "mac set devaddr ";
	strcat(temp, appEui);
	USART_putstring(temp);
}
void lora_set_appkey(){
	char* temp = "mac set appkey ";
	strcat(temp, appKey);
	USART_putstring(temp);
}


void lora_init(){
	lora_set_appkey();
	lora_set_appeui();
}