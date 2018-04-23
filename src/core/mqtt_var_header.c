#include <stdint.h>

#include "../../inc/core/mqtt_var_header.h"

struct mqtt_buf_connect_flag * mqtt_connect_flag_pack(union mqtt_attr_connect_flag flag){
	//!< check parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.flag_w_QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.flag_reserved);

	struct mqtt_buf_connect_flag * p_buf_flag = mqtt_buf_new(sizeof(uint8_t));
	p_buf_flag->buf[0] = MQTT_CONNECT_FLAG_PACK(flag);

	return p_buf_flag;
}

union mqtt_attr_connect_flag mqtt_connect_flag_unpack(const struct mqtt_buf_connect_flag * p_buf_flag){
	union mqtt_attr_connect_flag flag = MQTT_CONNECT_FLAG_UNPACK(p_buf_flag);
	 
	//!< check parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.flag_w_QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.flag_reserved);
	
	return flag;
}


struct mqtt_buf_connack_flag * mqtt_connack_flag_pack(union mqtt_attr_connack_flag flag){
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);

	struct mqtt_buf_connack_flag * p_buf_flag = mqtt_buf_new(sizeof(uint8_t));
	p_buf_flag->buf[0] = MQTT_CONNACK_FLAG_PACK(flag);

	//!< pack matt connack flag byte
	return p_buf_flag;
}

union mqtt_attr_connack_flag mqtt_connack_flag_unpack(
		const struct mqtt_buf_connack_flag * p_buf_flag){
	
	union mqtt_attr_connack_flag flag = MQTT_CONNACK_FLAG_UNPACK(p_buf_flag);

	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.reserved);

	return flag;
}



