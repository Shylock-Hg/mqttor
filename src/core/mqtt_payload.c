//#include "../../inc/core/mqtt_packet_segment.h"

#include <stdint.h>

#include "../../inc/core/mqtt_payload.h"

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

