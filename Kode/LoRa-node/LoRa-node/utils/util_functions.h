/*
 * util_functions.h
 *
 * Created: 23.11.2019 14:20:20
 *  Author: oystmol
 */ 
#include <board.h>

#ifndef UTIL_FUNCTIONS_H_
#define UTIL_FUNCTIONS_H_
#include <stdio.h>
uint8_t* hex_decode(const char *in, size_t len, uint8_t *out, int start);
uint16_t f2uflt16(float f);
char *hex_encode(board_t board);

#endif /* UTIL_FUNCTIONS_H_ */