//#include "../../inc/core/mqtt_packet_segment.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/core/mqtt_payload.h"

mqtt_attr_payload_t * mqtt_attr_payload_new(size_t len){
	assert(len);

	mqtt_attr_payload_t * payload = malloc(sizeof(mqtt_attr_payload_t));
	assert(payload);
	payload->len = len;
	payload->len_valid = 0;
	payload->buf = malloc(len);
	assert(payload->buf);
	
	return payload;
	
}

void mqtt_attr_payload_release(mqtt_attr_payload_t * payload){
	assert(payload);

	free(payload->buf);
	free(payload);
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

struct mqtt_buf_payload_suback_flag * mqtt_payload_suback_flag_pack(
		union mqtt_attr_payload_suback_flag flag){
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);

	//!< pack flag byte
	struct mqtt_buf_payload_suback_flag * p_buf_flag = mqtt_buf_new(sizeof(uint8_t));
	p_buf_flag->buf[0] = MQTT_PAYLOAD_SUBACK_FLAG_PACK(flag);

	return p_buf_flag;
}

union mqtt_attr_payload_suback_flag mqtt_payload_suback_flag_unpack(
		const struct mqtt_buf_payload_suback_flag * p_buf_flag){

	//!< unpack flag
	union mqtt_attr_payload_suback_flag flag = MQTT_PAYLOAD_SUBACK_FLAG_UNPACK(p_buf_flag);
	
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);

	return flag;
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

