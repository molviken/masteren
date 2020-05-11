/*
 * RN2483A.c
 *
 * Created: 25.11.2019 12:54:54
 *  Author: oystmol
 */ 

#include <USART.h>
#include <util_functions.h>
#include <string.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <util/delay.h>
#include <config.h>
#include <FSM.h>
#include "RN2483A.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
volatile int duty_cycle;


static void lora_auto_baud_detect(void);
static void lora_send_command(char * cmd);
static uint8_t lora_receive_response();
uint8_t lora_assert_response(const char * response);

static uint8_t lora_reset_to_band(void);
static uint8_t lora_set_pwridx(void);
static uint8_t lora_set_adr(void);
static uint8_t lora_set_dr(void);

static uint8_t lora_set_deveui(void);
static uint8_t lora_set_appeui(void);
static uint8_t lora_set_appkey(void);
static uint8_t lora_set_OTAA_settings(uint8_t *joined_err);

static uint8_t lora_set_appSKey(void);
static uint8_t lora_set_nwkSKey(void);
static uint8_t lora_set_devaddr(void);
static uint8_t lora_set_ABP_settings(void);

static uint8_t lora_set_dcycle(void);
static uint8_t lora_save_settings(void);


char answer[250];
//const char *downlink;
void lora_auto_baud_detect(void){
	DDRD |= (1<<PORTD1);
	PORTD &= ~(1<<PORTD1);
	_delay_ms(10);
	DDRD = 0x00;
	_delay_us(10);
	USART_init();
	USART_transmit0(0x55);
	_delay_ms(100);
}

uint8_t lora_set_dcycle(void){
	uint8_t err;
	#ifdef DEBUG_M
		printf("Setting dcycle for ch 0: ");
	#endif
	lora_send_command("mac set ch dcycle 0 9");
	err = lora_receive_response();
	if (err) return 0xB1;
	
	#ifdef DEBUG_M
	printf("Setting dcycle for ch 1: ");
	#endif
	lora_send_command("mac set ch dcycle 1 9");
	err = lora_receive_response();
	if (err) return 0xB2;
	
	#ifdef DEBUG_M
	printf("Setting dcycle for ch 2: ");
	#endif
	lora_send_command("mac set ch dcycle 2 9");
	err = lora_receive_response();
	if (err) return 0xB3;
	
	//#ifdef DEBUG_M
	//printf("Setting dcycle for ch 3: ");
	//#endif
	//lora_send_command("mac set ch dcycle 3 9");
	//err = lora_receive_response();
	//if (err) return 0xB5;
	//
	//#ifdef DEBUG_M
		//printf("Enabling channel 3: ");
	//#endif
	//lora_send_command("mac set ch status 3 on");
	//err = lora_receive_response();
	//if (err) return 0xB4;
	

	
	return 0;
	
}
uint8_t lora_reset(){
	lora_send_command("sys reset");
	lora_receive_response();
	return lora_assert_response(answer);
}
uint8_t lora_test_command(){
	lora_send_command("sys get hweui");
	lora_receive_response();
	printf("test cmd: ");
	puts(answer);
	puts("");
	return lora_assert_response(answer);
}

void lora_send_command(char * cmd){
	USART_putstring0(cmd);
	USART_transmit0(0x0D);
	USART_transmit0(0x0A);
}
uint8_t lora_receive_response(){
	char resp[250];
	unsigned char rec = 0;
	int i = 0;
	while (1){
		rec = USART_receive0();
		if (rec == 0x0D){
			USART_receive0(); // Wait for LF to be sent as well
			resp[i] = '\0';
			break;
		}
		resp[i] = rec;
		i++;
	}
	#ifdef DEBUG_M
	printf("%s\n", resp);
	#endif
	return lora_assert_response(resp);
}
uint8_t lora_assert_response(const char * response){
	if (!strncmp(response, "ok", strlen("ok")))																return RESP_OK;
	if (!strncmp(response, "accepted", strlen("accepted")))													return RESP_ACCEPTED;
	if (!strncmp(response, "mac_tx_ok", strlen("mac_tx_ok")))												return RESP_MAC_TX_OK;
	if (!strncmp(response, "mac_rx ", strlen("mac_rx "))){strcpy(downlink, response);						return RESP_MAC_RX_INC;}
	if (!strncmp(response, "denied", strlen("denied")))														return ERR_DENIED;
	if (!strncmp(response, "invalid_param", strlen("invalid_param")))										return ERR_INVALID_PARAM;
	if (!strncmp(response, "no_free_chan", strlen("no_free_chan")))											return ERR_NO_FREE_CH;
	if (!strncmp(response, "silent", strlen("silent")))														return ERR_SILENT;
	if (!strncmp(response, "busy", strlen("busy")))															return ERR_BUSY;
	if (!strncmp(response, "mac_paused", strlen("mac_paused")))												return ERR_MAC_PAUSED;
	if (!strncmp(response, "keys_not_init", strlen("keys_not_init")))										return ERR_KEYS_NOT_INIT;
	if (!strncmp(response, "not_joined", strlen("not_joined")))												return ERR_NOT_JOINED;
	if (!strncmp(response, "frame_counter_err_rejoin_needed", strlen("frame_counter_err_rejoin_needed")))	return ERR_FRAME_COUNTER_ERR_REJOIN_NEEDED;
	if (!strncmp(response, "invalid_data_len", strlen("invalid_data_len")))									return ERR_INVALID_PARAM;
	if (!strncmp(response, "mac_err", strlen("mac_err")))													return ERR_MAC_ERR;
// 	#ifdef DEBUG_M
// 		puts(response);
// 	#endif
	return 0xAA;
	
	
}

uint8_t lora_reset_to_band(){
	#ifdef DEBUG_M
		printf("Reset to band: ");
	#endif
	
	char cmd[20];
	#ifdef LORA_BAND
		sprintf(cmd, "mac reset %d", LORA_BAND);
	#endif
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_adr(){
	#ifdef DEBUG_M
		printf("Set ADR: ");
	#endif
	
	#ifdef LORA_ADR
		lora_send_command("mac set adr on");
	#else
		lora_send_command("mac set adr off");
	#endif
	return lora_receive_response();
}
uint8_t lora_set_dr(){
	#ifdef DEBUG_M
		printf("Set DR: ");
	#endif
	
	char cmd[20];
	#ifdef LORA_DR
		sprintf(cmd, "mac set dr %d",LORA_DR);
	#endif
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_pwridx(){
	#ifdef DEBUG_M
		printf("Set pwridx: ");
	#endif
	
	char cmd[20];
	#ifdef LORA_PWRIDX
	sprintf(cmd, "mac set pwridx %d",LORA_PWRIDX);
	#endif	
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_save_settings(){
	#ifdef DEBUG_M
		printf("Save settings: ");
	#endif
	
	lora_send_command("mac save");
	return lora_receive_response();
}


/********************************************
             OTAA functions					
********************************************/
const char *devEui = "0004A30B00EB9F66";
const char *appEui = "70B3D57ED002E533";
const char *appKey = "654E5718FCCBB86E95272A1BB24C996D";

uint8_t lora_set_deveui(){
	#ifdef DEBUG_M
		printf("Set deveui: ");
	#endif
	
	char cmd[50] = "mac set deveui ";
	strcat(cmd, devEui);
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_appeui(){
	#ifdef DEBUG_M
		printf("Set appeui: ");
	#endif	
	
	char cmd[50] = "mac set appeui ";
 	strcat(cmd, appEui);
 	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_appkey(){
	#ifdef DEBUG_M
		printf("Set appkey: ");
	#endif
	
 	char cmd[50] = "mac set appkey ";
 	strcat(cmd, appKey);
 	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_OTAA_settings(uint8_t *joined_err){
	#ifdef DEBUG_M
		puts("Loading OTAA settings...");
	#endif
	uint8_t err = 0;
	
	err = lora_set_deveui();
	if (err) return 7;
		
	err = lora_set_appeui();
	if (err) return 8;
			
	err = lora_set_appkey();
	if (err) return 9;

	err = lora_save_settings();
	if (err) return 10;
	
	*joined_err = lora_join_OTAA();
	
	return 0;
}
uint8_t lora_join_OTAA(){
	uint8_t err;
	#ifdef DEBUG_M
	puts("joining OTAA...");
	#endif
	
	#ifdef DEBUG_M
	printf("Mac join command: ");
	#endif
	lora_send_command("mac join otaa");
	err = lora_receive_response();
	if (err) return 0x22;
	
	#ifdef DEBUG_M
	printf("Join result: ");
	#endif
	err = lora_receive_response();
	if (err) return DEV_NOT_JOINED;
	
	
	return 0;
}

/********************************************
             ABP functions					
********************************************/
const char *devAddr = "260116C0";
const char *nwkSKey = "B35B3DBCEB99FFFAD9F83E35603AA69D";
const char *appSKey = "39A7833E6A411AC95CD4E37DB472805B";

uint8_t lora_set_appSKey(){
	char cmd[50] = "mac set appskey ";
	strcat(cmd, appSKey);
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_nwkSKey(){
	char cmd[50] = "mac set nwkskey ";
	strcat(cmd, nwkSKey);
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_devaddr(){
	char cmd[50] = "mac set devaddr ";
	strcat(cmd, devAddr);
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_ABP_settings(){
	puts("Loading ABP settings...");
	
	uint8_t err = 0;
	
	printf("Set appSKey: ");
	err = lora_set_appSKey();
	if (err) { return 1;}
	
	printf("Set nwkSKey: ");
	err = lora_set_nwkSKey();
	if (err) return 2;
	
	printf("Set devaddr: ");
	err = lora_set_devaddr();
	if (err) return 3;
	
	printf("Save ABP setting: ");
	err = lora_save_settings();
	if (err) return 4;
	
	
	lora_join_ABP();
	//lora_transmit("deadbeef");
	return 0;
}
void lora_join_ABP(){
	uint8_t err;
	
	printf("Mac join ABP cmd: ");
	lora_send_command("mac join abp");
	err = lora_receive_response();
	printf("Mac join ABP result: ");
	err = lora_receive_response();
	//if (!err){
		//printf("Join result: ");
		//err = lora_receive_response();
	//}
	//else printf("Failed sendint join cmd: %02x\n", err);
	////lora_transmit("deadbeef");
}



uint8_t lora_init(uint8_t *joined_err){
	lora_auto_baud_detect();
	uint8_t err = 0;
	
	
	#ifdef DEBUG_M
	puts("Lora init...");
	#endif	
	


	err = lora_reset_to_band();
	if (err) return 1;
	
		
	err = lora_set_adr() ;
	if (err) return 2;
	
		
	err = lora_set_dr();
	if (err) return 3;
	
	
	err = lora_set_pwridx();
	if (err) return 4;
	
	lora_set_dcycle();
// 	lora_send_command("mac set ch dcycle 0 9");
// 	lora_receive_response();
	#ifdef OTAA
		err = lora_set_OTAA_settings(joined_err);
		if (err) return 5;	
	#endif
	#ifdef ABP
		err = lora_set_ABP_settings();
		if (err) return 6;
	#endif
	
	
	#ifdef DEBUG_M
	puts("Init success");
	#endif
	
	return 0;

}
uint8_t lora_transmit(const char *payload){
	uint8_t err;
	char buf[100] = "mac tx uncnf 3 ";
	strcat(buf, payload);
	
	#ifdef DEBUG_M
		printf("Mac tx cmd: ");
	#endif
	lora_send_command(buf);
	err = lora_receive_response();
	
	if (!err){
		#ifdef DEBUG_M
			printf("Mac tx result: ");
		#endif
		err = lora_receive_response();
		//puts(downlink);
		if(err == RESP_MAC_RX_INC){
			lora_assert_downlink(downlink);
		}
	}
	return 0;
}


void lora_assert_downlink(){
	uint32_t ts;
	puts("Asserting downlink");
	size_t str_len = strlen(downlink);
	//printf("Downlink: %s, with len: %d\n", downlink, str_len);
	ascii_hex_decode(downlink, str_len, downlink_pairs, 9);
	//puts("decode complete");
	if (downlink_pairs[0] == DEV_ID){
		switch (downlink_pairs[1]){
			case 0x0A:
				ts = (uint32_t)downlink_pairs[0] | ((uint32_t)downlink_pairs[1] << 8) | ((uint32_t)downlink_pairs[2] << 16) | ((uint32_t)downlink_pairs[3] << 24);
				current_time = ts;
				
				#ifdef DEBUG_M
				printf("Changed time to: %lu \n", current_time);
				#endif
				break;	
			case 0xB0:
				printf("changing data rate\n");
				break;	
			
			case 0xC0:
				cli();
				WDTCSR = 0x00;
				WDTCSR = (1<<WDE);
				break;	
		}
	}
}