/*! \brief mqtt packet segment function
 *  \author Shylock Hg
 *  \date 2018-04-09
 *  \email tcath2s@gmail.com
 * */
#include <irmalloc.h>
#undef BIT

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <core/mqtt_packet_segment.h>

#include <toolkit/array.h>
#include <toolkit/mqtt_log.h>


//
static const char * errstr[] = {
	"E_NONE",
	"E_FORMAT_CHECK",
	"E_MEM_FAIL",
	"E_MEM_OUT"
};

const char * mqtt_str_error(mqtt_err_t err){
	return errstr[err];
}

struct mqtt_buf * mqtt_buf_new(size_t len){
	//uint8_t * buf = irmalloc(len);
	uint8_t * buf = ircalloc(len,sizeof(uint8_t));
	struct mqtt_buf * mq_buf = irmalloc(sizeof(struct mqtt_buf));
	mq_buf->buf = buf;
	mq_buf->len = len;
	//memset(mq_buf->buf,0x00,mq_buf->len);

	return mq_buf;
}

mqtt_buf_t * mqtt_buf_new_4_buf(uint8_t * buf, size_t len){
	assert(buf);
	
	mqtt_buf_t * mq_buf = mqtt_buf_new(len);
	assert(mq_buf);
	memcpy(mq_buf->buf, buf, len);

	return mq_buf;
}

void mqtt_buf_release(struct mqtt_buf * mq_buf){
	irfree(mq_buf->buf);
	irfree(mq_buf);
}



#define MQTT_BUF_STR_LEN_CHECK(len) assert(MQTT_BUF_STR_MAX_LEN >= len)

struct mqtt_buf_str * mqtt_buf_str_encode(const char * str){

	if(NULL == str)
		return NULL;

	size_t len = strlen(str);  //!< c-string length
	MQTT_BUF_STR_LEN_CHECK(len);  //< check c-string length

	//< caculate buffer length
	//struct mqtt_buf_str * mq_str = irmalloc(sizeof(struct mqtt_buf_str));
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
	if(NULL == mq_str)
		return NULL;

	//< c-string length with '\0'
	size_t len = (mq_str->len)+1-MQTT_BUF_STR_MAX_BYTE;
	mqtt_attr_str_t str = irmalloc(len);

	//< copy c-string content
	memcpy(str,mq_str->buf+MQTT_BUF_STR_MAX_BYTE,len-1);
	str[len] = '\0';
	
	return str;
}

mqtt_attr_str_t mqtt_buf_str_4_buf(const uint8_t * buf){
	assert(buf);
	
	mqtt_attr_uint16_t len_str = BYTES_2_UINT16(buf);
	//printf("str len = `%d`\n", len_str);
	mqtt_buf_str_t * buf_string = mqtt_buf_new(len_str+MQTT_BUF_STR_MAX_BYTE);
	assert(buf_string);
	memcpy(buf_string->buf, buf, len_str+MQTT_BUF_STR_MAX_BYTE);
	mqtt_attr_str_t string = mqtt_buf_str_decode(buf_string);
	mqtt_buf_release(buf_string);
	return string;
}


struct mqtt_buf_uint16 * mqtt_buf_uint16_encode(mqtt_attr_uint16_t num){
	struct mqtt_buf_uint16 * mq_uint16 = mqtt_buf_new(
			sizeof(mqtt_attr_uint16_t));
	UINT16_2_BYTES(num,mq_uint16->buf);

	return mq_uint16;
}

mqtt_attr_uint16_t mqtt_buf_uint16_decode(const mqtt_buf_uint16_t * mq_uint16){
	return BYTES_2_UINT16(mq_uint16->buf);
}



