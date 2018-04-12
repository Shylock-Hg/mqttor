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

struct mqtt_buf * mqtt_buf_new(size_t len){
	uint8_t * buf = malloc(len);
	struct mqtt_buf * mq_buf = malloc(sizeof(struct mqtt_buf));
	mq_buf->buf = buf;
	mq_buf->len = len;

	return mq_buf;
}

void mqtt_buf_release(struct mqtt_buf * mq_buf){
	free(mq_buf->buf);
	free(mq_buf);
}


#define MQTT_BUF_STR_MAX_BYTE 2
#define MQTT_BUF_STR_MAX_LEN  0xFFFF

#define MQTT_BUF_STR_LEN_CHECK(len) assert(MQTT_BUF_STR_MAX_LEN >= len)

struct mqtt_buf_str * mqtt_buf_str_encode(const mqtt_attr_str_t str){
	size_t len = strlen(str);  //!< c-string length
	MQTT_BUF_STR_LEN_CHECK(len);  //< check c-string length

	//< caculate buffer length
	//struct mqtt_buf_str * mq_str = malloc(sizeof(struct mqtt_buf_str));
	//mq_str->len = len + MQTT_BUF_STR_MAX_BYTE;
	struct mqtt_buf_str * mq_str = mqtt_buf_new(len+MQTT_BUF_STR_MAX_BYTE);

	//< encode c-string length
	uint8_t code_len[MQTT_BUF_STR_MAX_BYTE] = {0};
	UINT16_2_BYTES(len,code_len);

	/// |MSB|LSB|DATA...|
	memcpy(mq_str->buf,code_len,sizeof(code_len));
	memcpy((mq_str->buf)+sizeof(code_len),str,len);

	return mq_str;
}

mqtt_attr_str_t mqtt_buf_str_decode(const struct mqtt_buf_str * mq_str){
	//< c-string length with '\0'
	size_t len = (mq_str->len)+1-MQTT_BUF_STR_MAX_BYTE;
	mqtt_attr_str_t str = malloc(len);

	//< copy c-string content
	memcpy(str,mq_str->buf+MQTT_BUF_STR_MAX_BYTE,len-1);
	str[len] = '\0';
	
	return str;
}


struct mqtt_buf_uint16 * mqtt_buf_uint16_encode(mqtt_attr_uint16_t num){
	struct mqtt_buf_uint16 * mq_uint16 = mqtt_buf_new(sizeof(mqtt_attr_uint16_t));
	UINT16_2_BYTES(num,mq_uint16->buf);

	return mq_uint16;
}

mqtt_attr_uint16_t mqtt_buf_uint16_decode(const struct mqtt_buf_uint16 * mq_uint16){
	return BYTES_2_UINT16(mq_uint16->buf);
}
