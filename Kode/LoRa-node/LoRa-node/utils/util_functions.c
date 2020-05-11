/*
 * util_functions.c
 *
 * Created: 24.11.2019 17:54:54
 *  Author: oystmol
 */ 
#include "util_functions.h"
#include "USART.h"
#include <math.h>

//const char* test2 = "6363 3131 3535 6262 6666"; = "cc 11 55 bb ff" = "204 17 85 187 255"
//const char* test = "3565623838383532";
void ascii_hex_decode(const char *in, size_t len, uint8_t *out, int start){
	char buf[10];
	uint8_t i, t, j, hn, ln, p1, p2;
	for (t = 0, i = start; i<len; i+=2, t++){
		//puts("2");
		p1 = ((uint8_t)(in[i]) - '0')*16;
		p2 = (uint8_t)(in[i+1]) - '0';
		buf[t] = p1+p2;
		//printf("%u", buf[t]);
	}
	//puts("3");
	for (j = 0; j<10; j+=2){
		hn = buf[j] > '9' ? buf[j] - 'a' + 10 : buf[j] - '0';
		ln = buf[j+1] > '9' ? buf[j+1] - 'a' + 10 : buf[j+1] - '0';
		out[j] = (hn << 4) | ln;
		printf("%02x ", out[j]);
	}
	//puts("4");
}

uint8_t* hex_decode(const char *in, size_t len,uint8_t *out, int start){
	puts("");
	unsigned int i, t, hn, ln;
	for (t = 0,i = start; i < len; i+=2,++t) {
		hn = in[i] > '9' ? in[i] - 'a' + 10 : in[i] - '0';
		ln = in[i+1] > '9' ? in[i+1] - 'a' + 10 : in[i+1] - '0';
		out[t] = (hn << 4 ) | ln;
		printf(" %x ", out[t]);
	}
	return out;
}

//				1  2  3  4  5  6  7  8  9 10 11 byte pair
//  MSG STYLE: xx xx xx xx xx xx xx xx xx xx xx
//  1: frame, 2: ID, 3-6: timestamp, 7: battery, 8+9: bus voltage, 10+11: current

char* hex_encode(board_t board){
	char *msg = "";
	sprintf(msg, "%02X%02X%08lX%02X%04X%04X", board.frame_counter, board.id, board.time_stamp, board.batteryLevel, board.ina219.bus_voltage, board.ina219.current);
	return msg;
}


uint16_t f2uflt16(float f){
	if (f < 0.0)return 0;
	else if (f >= 1.0)return 0xFFFF;
	else{
		int iExp;
		float normalValue;

		normalValue = frexpf(f, &iExp);
		iExp += 15;
		if (iExp < 0){iExp = 0;}
		
		uint16_t outputFraction = ldexpf(normalValue, 12) + 0.5;
		if (outputFraction >= (1 << 12u)){
			outputFraction = 1 << 11;
			++iExp;
		}
		if (iExp > 15)return 0xFFFF;

		return (uint16_t)((iExp << 12u) | outputFraction);
	}
}