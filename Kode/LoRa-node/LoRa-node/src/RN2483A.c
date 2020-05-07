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

#include "RN2483A.h"


#define BAND 868
volatile int duty_cycle;


static void lora_auto_baud_detect(void);
static void lora_send_command(char * cmd);
static uint8_t lora_receive_response();
static uint8_t lora_assert_response(const char * response);

static uint8_t lora_reset_to_band(void);
static uint8_t lora_set_pwridx(void);
static uint8_t lora_set_adr(void);
static uint8_t lora_set_dr(void);

static uint8_t lora_set_deveui(void);
static uint8_t lora_set_appeui(void);
static uint8_t lora_set_appkey(void);
static uint8_t lora_set_OTAA_settings(void);

static uint8_t lora_set_appSKey(void);
static uint8_t lora_set_nwkSKey(void);
static uint8_t lora_set_devaddr(void);
static uint8_t lora_set_ABP_settings(void);

static uint8_t lora_save_settings(void);


char answer[250];
const char *hweui;

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
	return lora_assert_response(resp);
}
uint8_t lora_assert_response(const char * response){
	if (!strncmp(response, "ok", strlen("ok")))																return RESP_OK;
	if (!strncmp(response, "accepted", strlen("accepted")))													return RESP_ACCEPTED;
	if (!strncmp(response, "mac_tx_ok", strlen("mac_tx_ok")))												return RESP_MAC_TX_OK;
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
	puts(response);
	hweui = response;
	return -1;
	
	
}

uint8_t lora_reset_to_band(){
	char *cmd = "";
	#ifdef LORA_BAND
		sprintf(cmd, "mac reset %d", LORA_BAND);
	#endif
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_adr(){
	#ifdef LORA_ADR
		lora_send_command("mac set adr on");
	#else
		lora_send_command("mac set adr off");
	#endif
	return lora_receive_response();
}
uint8_t lora_set_dr(){
	char *cmd = "";
	#ifdef LORA_DR
		sprintf(cmd, "mac set dr %d",LORA_DR);
	#endif
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_pwridx(){
	char *cmd = "";
	#ifdef LORA_PWRIDX
	sprintf(cmd, "mac set pwridx %d",LORA_PWRIDX);
	#endif	
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_save_settings(){
	lora_send_command("mac save");
	return lora_receive_response();
}


/********************************************
             OTAA functions					
********************************************/
//const char *devEui_otaa = "0004A30B00EC2B38";
const char *appEui = "70B3D57ED002E533";
const char *appKey = "7C9097CF710A2B6098F7A8ED368778DE";


uint8_t lora_set_deveui(){
	char cmd[50] = "mac set deveui ";
	lora_send_command("sys get hweui");
	lora_receive_response();
	strcat(cmd, hweui);
	puts(cmd);
	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_appeui(){
	char cmd[50] = "mac set appeui ";
 	strcat(cmd, appEui);
 	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_appkey(){
 	char cmd[50] = "mac set appkey ";
 	strcat(cmd, appKey);
 	lora_send_command(cmd);
	return lora_receive_response();
}
uint8_t lora_set_OTAA_settings(){
	puts("Loading OTAA settings...");
	uint8_t err = 0;
	err = lora_set_deveui();
	if (err) {printf("set deveui failed with err: %02x\n", err); return 1;}
	else printf("Set deveui: %d\n", err);
	
	err = lora_set_appeui();
	if (err) {printf("set appeui failed with err: %02x\n", err); return 2;}
	else printf("Set appeui: %d\n", err);	
		
	err = lora_set_appkey();
	if (err) {printf("set appkey failed with err: %02x\n", err); return 3;}
	else printf("Set appkey: %d\n", err);	
		
	err = lora_save_settings();
	if (err) {printf("saving settings failed with err: %02x\n", err); return 4;}
	else printf("Save settings: %d\n", err);
	
	lora_joined_flag = lora_join_OTAA();
	
	return 0;
}
uint8_t lora_join_OTAA(){
	uint8_t err;
	puts("joining OTAA...");
	for (int i = 0; i<3; i++){
		printf("Try: %d...\n", i);
		lora_send_command("mac join otaa");
		err = lora_receive_response();
		//printf("1    %u\n", err);
		if (!err){
			err = lora_receive_response();
			//printf("2    %d\n", err);
			if (err) printf("Joining over OTAA failed with error: %02X\n", err);
			else {
				puts("Joined successfully");
				return 1;
			}
		}
		else printf("mac join cmd failed with error: %d\n", err);
	}
	return 0;
}

/********************************************
             ABP functions					
********************************************/
//const static char *devEui_abp = "0004A30B00EBF02C";
const char *devAddr = "260118B2";
const char *nwkSKey = "971B25D9071731773A5145547F71302E";
const char *appSKey = "6CE9997BA88AB546F8E97E9EFAFB4122";

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
	err = lora_set_appSKey();
	if (err) { return 1;}
	printf("set appSKey: %02x\n", err);
	err = lora_set_nwkSKey();
	
	if (err) return 2;
	printf("set nwkSKey: %02x\n", err);
	
	err = lora_set_devaddr();
	if (err) return 3;
	printf("set devaddr: %02x\n", err);
	
	err = lora_save_settings();
	if (err) return 4;
	printf("save ABP settings: %02x\n", err);
	lora_join_ABP();
	
	return 0;
}
void lora_join_ABP(){
	uint8_t err;
	
	lora_send_command("mac join abp");
	err = lora_receive_response();
	printf("First: %02x\n", err);
	if (!err){
		err = lora_receive_response();
		printf("Second: %02x\n", err);
	}
	else printf("Failed sendint join cmd: %02x\n", err);
	lora_transmit("deadbeef");
}





uint8_t lora_init(){
	lora_auto_baud_detect();
	uint8_t err = 0;
	
	
	
	#ifdef DEBUG_M
	puts("Lora init...");
	#endif	
	
	
	err = lora_reset_to_band();
	#ifdef DEBUG_M
		if (err) {puts("Reset to band failed"); return 1;}
	#endif	
		
	err = lora_set_adr() ;
	#ifdef DEBUG_M
		if (err) {puts("Disable adr failed"); return 2;}
	#endif
		
	err = lora_set_dr();
	#ifdef DEBUG_M
		if (err) {puts("Setting dr failed"); return 3;}
	#endif	
	
	err = lora_set_pwridx();
	#ifdef DEBUG_M
		if (err) {puts("Set pwridx failed"); return 4;}
	#endif	
	
// 	lora_send_command("mac set ch dcycle 0 9");
// 	lora_receive_response();
	#ifdef OTAA
		err = lora_set_OTAA_settings();
		#ifdef DEBUG_M
			if (err) {printf("Loading OTAA settings failed with err: %u", err); return 5;}
		#endif	
	#endif
	#ifdef ABP
		err = lora_set_ABP_settings();
		#ifdef DEBUG_M
		if (err) {printf("Loading ABP settings failed with err: %u", err); return 6;}	
		#endif	
	#endif
	
	return 0;
	puts("Init success");
}
uint8_t lora_transmit(const char *payload){
	uint8_t err;
	char buf[100] = "mac tx uncnf 3 ";
	strcat(buf, payload);
	puts(buf);
	lora_send_command(buf);
	err = lora_receive_response();
	if (!err){
		err = lora_receive_response();
		printf("Transmit with code: %02x\n", err);
	}
	return 0;
}

void lora_strip_downlink(const char*rx){
	size_t str_len = strlen(rx);
	hex_decode(rx, str_len, downlink_pairs, 10);
}
//void lora_assert_downlink(){
	//printf("%u %u %u\n", downlink_pairs[0], downlink_pairs[1], downlink_pairs[2]);
	//if (downlink_pairs[0] == 0x7E){
		//switch (downlink_pairs[1]){
			//case 0x0A:
				//printf("reseting..\n");
				//break;	
			//case 0xB0:
				//printf("changing data rate\n");
				////int err = lora_set_dr(downlink_pairs[2]);
				//if (err){printf("data rate changed to %d\n", downlink_pairs[2]);}			
		//}
	//}
//}