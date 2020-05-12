#ifndef RN2483A_H_
#define RN2483A_H_

#include <stdint.h>


////////////////////////////////////////////////////////
//			   RESPONSES FROM RN2483A				  //
////////////////////////////////////////////////////////

/***** Success responses *****/
// Default for most

#define RESP_OK										0x00

// Mac join success
#define RESP_ACCEPTED								0x00

// Mac tx without and with receive on RX window
#define RESP_MAC_TX_OK								0x10
#define RESP_MAC_RX_INC								0x11

/***** Error responses *****/
// Default for most
#define ERR_INVALID_PARAM							0x01

// Common for multiple
#define ERR_NO_FREE_CH								0x02
#define ERR_SILENT									0x03
#define ERR_BUSY									0x04
#define ERR_MAC_PAUSED								0x05

// First response mac join
#define ERR_KEYS_NOT_INIT							0x06

// Second response mac join
#define ERR_DENIED									0x0A

// First response mac tx
#define ERR_NOT_JOINED								0x07
#define ERR_FRAME_COUNTER_ERR_REJOIN_NEEDED			0x08
#define ERR_INVALID_DATA_LEN						0x09

// Second response mac tx
#define ERR_MAC_TX_OK								0x0B
#define ERR_MAC_RX_PORTNO_DATA						0x0C
#define ERR_MAC_ERR									0x0D


#define DEV_NOT_JOINED								0x23
#define LORA_RX_PAYLOAD_OFFSET						0x09



uint8_t lora_test_command();
uint8_t lora_reset();


uint8_t lora_join_OTAA();
void lora_join_ABP();
uint8_t lora_init(uint8_t *joined_err);

void lora_transmit(const char *payload);
void lora_strip_downlink(const char*downlink);
void lora_assert_downlink();

#endif /* RN2483A_H_ */