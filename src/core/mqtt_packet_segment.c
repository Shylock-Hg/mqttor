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

#include "../../inc/core/mqtt_packet_segment.h"

#include "../../inc/toolkit/array.h"

#define MQTT_PACKET_STR_MAX_BYTE 2
#define MQTT_PACKET_STR_MAX_LEN  0xFFFF

#define MQTT_PACKET_STR_LEN_CHECK(len) assert(MQTT_PACKET_STR_MAX_LEN >= len)

struct mqtt_str * mqtt_packet_str_encode(const char * str){
	size_t len = strlen(str);  //!< c-string length
	MQTT_PACKET_STR_LEN_CHECK(len);  //< check c-string length

	//< caculate buffer length
	struct mqtt_str * mq_str = malloc(sizeof(struct mqtt_str));
	mq_str->len = len + MQTT_PACKET_STR_MAX_BYTE;

	//< encode c-string length
	uint8_t code_len[MQTT_PACKET_STR_MAX_BYTE] = {0};
	UINT16_2_BYTES(len,code_len);

	/// |MSB|LSB|DATA...|
	mq_str->code = malloc(mq_str->len);
	memcpy(mq_str->code,code_len,sizeof(code_len));
	memcpy((mq_str->code)+sizeof(code_len),str,len);

	return mq_str;
}

char * mqtt_packet_str_decode(const struct mqtt_str * mq_str){
	//< c-string length with '\0'
	size_t len = (mq_str->len)+1-MQTT_PACKET_STR_MAX_BYTE;
	char * str = malloc(len);

	//< copy c-string content
	memcpy(str,mq_str->code+MQTT_PACKET_STR_MAX_BYTE,len-1);
	str[len] = '\0';
	
	return str;
}


