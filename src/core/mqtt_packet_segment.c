/*! \brief mqtt packet segment function
 *  \author Shylock Hg
 *  \date 2018-04-09
 *  \email tcath2s@gmail.com
 * */

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "../../inc/toolkit/array.h"

#define MQTT_PACKET_STR_MAX_BYTE 2
#define MQTT_PACKET_STR_MAX_LEN  0xFFFF

#define MQTT_PACKET_STR_LEN_CHECK(len) assert(MQTT_PACKET_STR_MAX_LEN >= len)

const char * mqtt_packet_str_decode(const uint8_t * code){
	size_t len = BYTES_2_UINT16(code);

	char * str = malloc(len+1);

	memcpy(str,code+MQTT_PACKET_STR_MAX_BYTE,len);
	str[len] = '\0';
	
	return str;
}

const uint8_t * mqtt_packet_str_encode(const char * str){
	size_t len = strlen(str);
	MQTT_PACKET_STR_LEN_CHECK(len);
	uint8_t code_len[MQTT_PACKET_STR_MAX_BYTE] = {0};
	UINT16_2_BYTES(len,code_len);

	uint8_t * code = malloc(len+MQTT_PACKET_STR_MAX_BYTE);

	/// |MSB|LSB|DATA...|
	memcpy(code,code_len,sizeof(code_len));
	memcpy(code+sizeof(code_len),str,len);

	return code;
}


