//#include "../../inc/core/mqtt_packet_segment.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <core/mqtt_payload.h>
#include <toolkit/mqtt_log.h>

mqtt_attr_payload_t * mqtt_attr_payload_new(size_t len){
	//assert(len);
	if(0 == len)
		return NULL;

	mqtt_attr_payload_t * payload = malloc(sizeof(mqtt_attr_payload_t));
	assert(payload);
	payload->len = len;
	payload->len_valid = 0;
	payload->pos = 0;
	payload->buf = malloc(len);
	assert(payload->buf);
	
	return payload;
	
}

void mqtt_attr_payload_release(mqtt_attr_payload_t * payload){
	//assert(payload);
	if(NULL != payload){
		free(payload->buf);
		free(payload);
	}
}

mqtt_buf_t * mqtt_attr_payload_2_buf(mqtt_attr_payload_t * payload){
	assert(payload);

	payload->len = payload->len_valid;

	return (mqtt_buf_t*)payload;
}

mqtt_buf_t * mqtt_attr_payload_deep2_buf(mqtt_attr_payload_t * payload){
	assert(payload);

	mqtt_buf_t * mq_buf = mqtt_buf_new(payload->len_valid);
	assert(mq_buf);
	memcpy(mq_buf->buf, payload->buf, payload->len_valid);

	return mq_buf;
}

//!< matt packet payload writer
int mqtt_attr_payload_write_string(mqtt_attr_payload_t * payload, 
		const char * string){
	assert(payload);
	assert(string);

	mqtt_attr_uint16_t len = strlen(string);
	if(len + MQTT_BUF_STR_MAX_BYTE + payload->len_valid > 
			payload->len){
		return -E_MEM_OUT;
	}
	mqtt_buf_uint16_t * buf_uint16 = mqtt_buf_uint16_encode(len);
	//mqtt_log_print_buf(buf_uint16->buf, buf_uint16->len);
	memcpy(payload->buf+payload->len_valid, buf_uint16->buf, 
			buf_uint16->len);
	payload->len_valid += buf_uint16->len;
	//mqtt_log_print_buf(payload->buf, payload->len_valid);
	mqtt_buf_release(buf_uint16);

	strcpy((char*)payload->buf+payload->len_valid, string);
	payload->len_valid += len;

	return len+MQTT_BUF_STR_MAX_BYTE;
}

int mqtt_attr_payload_read_string(mqtt_attr_payload_t * payload, char * buf, 
		uint16_t len){
	assert(payload);
	assert(buf);
	assert(len);

	//mqtt_log_printf(LOG_LEVEL_LOG, "Payload->len_valid == `%ld`!\n", 
			//payload->len_valid);

	mqtt_attr_uint16_t len_str = BYTES_2_UINT16(payload->buf+payload->pos);
	if(len_str > len){
		return -E_MEM_OUT;
	}
	if((payload->pos + len_str) >= payload->len_valid){
		return -E_MEM_OUT;
	}

	memcpy(buf, payload->buf+payload->pos+MQTT_BUF_STR_MAX_BYTE, len_str);
	buf[len_str] = '\0';

	payload->pos += (len_str+MQTT_BUF_STR_MAX_BYTE);

	return len_str + MQTT_BUF_STR_MAX_BYTE;
}

int mqtt_attr_payload_write_byte(mqtt_attr_payload_t * payload, uint8_t byte){
	assert(payload);

	if(payload->len == payload->len_valid){
		return -E_MEM_OUT;
	}

	payload->buf[payload->len_valid++] = byte;
	
	return sizeof(byte);
}

int mqtt_attr_payload_read_byte(mqtt_attr_payload_t * payload, uint8_t * byte){
	assert(payload);
	assert(byte);

	if(payload->pos >= payload->len_valid)
		return -E_MEM_OUT;

	*byte = payload->buf[payload->pos++];

	return E_NONE;
}

int mqtt_attr_payload_write_bytes(mqtt_attr_payload_t * payload, uint8_t * bytes,
		size_t len){
	assert(payload);
	assert(bytes);

	if(len + payload->len_valid > payload->len)
		return -E_MEM_OUT;

	memcpy(payload->buf+payload->len_valid, bytes, len);
	payload->len_valid += len;

	return len;
}

int mqtt_attr_payload_read_bytes(mqtt_attr_payload_t * payload, uint8_t * bytes, 
		size_t len){
	assert(payload);
	assert(bytes);

	if(len + payload->pos >= payload->len_valid)
		return -E_MEM_OUT;
	
	memcpy(bytes, payload->buf+payload->pos, len);
	payload->pos += len;

	return E_NONE;
}

struct mqtt_buf_payload_suback_ret_code * mqtt_payload_suback_ret_code_pack(
		union mqtt_attr_payload_suback_ret_code ret_code){
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(ret_code.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(ret_code.bits.reserved);

	//!< pack flag byte
	struct mqtt_buf_payload_suback_ret_code * p_buf_retcode = 
		mqtt_buf_new(sizeof(uint8_t));
	p_buf_retcode->buf[0] = MQTT_PAYLOAD_SUBACK_RET_CODE_PACK(ret_code);

	return p_buf_retcode;
}

union mqtt_attr_payload_suback_ret_code mqtt_payload_suback_ret_code_unpack(
		const struct mqtt_buf_payload_suback_ret_code * p_buf_ret_code){

	//!< unpack flag
	union mqtt_attr_payload_suback_ret_code ret_code = 
		MQTT_PAYLOAD_SUBACK_RET_CODE_UNPACK(p_buf_ret_code);
	
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(ret_code.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(ret_code.bits.reserved);

	return ret_code;
}


struct mqtt_buf_payload_subscribe_content_QoS * mqtt_payload_subscribe_content_QoS_pack(
		union mqtt_attr_payload_subscribe_content_QoS flag){
	//< check parameters
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);

	//< new mqtt buf
	struct mqtt_buf_payload_subscribe_content_QoS * p_buf_flag = 
		mqtt_buf_new(sizeof(uint8_t));
	p_buf_flag->buf[0] = MQTT_PAYLOAD_SUBSCRIBE_CONTENT_QoS_PACK(flag);

	return p_buf_flag;
}

union mqtt_attr_payload_subscribe_content_QoS mqtt_payload_subscribe_content_QoS_unpack(
		const struct mqtt_buf_payload_subscribe_content_QoS * p_buf_flag){

	union mqtt_attr_payload_subscribe_content_QoS flag = 
		MQTT_PAYLOAD_SUBSCRIBE_CONTENT_QoS_UNPACK(p_buf_flag);

	//< check parameters
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);
		
	return flag;
}

