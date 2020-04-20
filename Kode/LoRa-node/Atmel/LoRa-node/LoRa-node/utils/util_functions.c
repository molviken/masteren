/*
 * util_functions.c
 *
 * Created: 24.11.2019 17:54:54
 *  Author: oystmol
 */ 
#include "util_functions.h"
#include "USART.h"
uint8_t* hex_decode(const char *in, size_t len,uint8_t *out){
	unsigned int i, t, hn, ln;
	for (t = 0,i = 0; i < len; i+=2,++t) {
		hn = in[i] > '9' ? in[i] - 'A' + 10 : in[i] - '0';
		ln = in[i+1] > '9' ? in[i+1] - 'A' + 10 : in[i+1] - '0';
		out[t] = (hn << 4 ) | ln;
	}
	return out;
}

