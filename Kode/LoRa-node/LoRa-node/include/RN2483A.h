/*
 * RN2483A.h
 *
 * Created: 25.11.2019 12:55:07
 *  Author: oystmol
 */ 


#ifndef RN2483A_H_
#define RN2483A_H_

void lora_send_command(char * cmd);
void lora_receive_response();
void lora_init();
void lora_set_appkey();
void lora_set_appeui();
#endif /* RN2483A_H_ */