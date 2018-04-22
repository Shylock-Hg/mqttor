#include <stdint.h>

#include "../../inc/core/mqtt_var_header.h"

struct mqtt_buf_conn_flag * mqtt_conn_flag_pack(union mqtt_attr_conn_flag flag){
	//!< check parameter
	MQTT_ATTR_FLAG_VAL_QoS_CHECK(flag.bits.flag_w_QoS);
	MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(flag.bits.flag_reserved);
	struct mqtt_buf_conn_flag * p_buf_flag = mqtt_buf_new(sizeof(uint8_t));
	p_buf_flag->buf[0] = MQTT_CONN_FLAG_PACK(flag);

	return p_buf_flag;
}

union mqtt_attr_conn_flag mqtt_conn_flag_unpack(struct mqtt_buf_conn_flag * p_buf_flag){
	//!< 
	
	return MQTT_CONN_FLAG_UNPACK(p_buf_flag);
}


uint8_t mqtt_connack_flag_pack_s(struct mqtt_connack_flag * p_flag){
	//!< chekc parameter
	MQTT_ATTR_FLAG_VAL_BOOL_CHECK(p_flag->SP);

	//!< pack matt connack flag byte
	return MQTT_CONNACK_FLAG_PACK(p_flag);
}
