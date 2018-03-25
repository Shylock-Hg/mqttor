#include "../../inc/mqtt_payload.h"
#include "../../inc/mqtt_packet.h"

#include <stdint.h>

uint8_t mqtt_payload_suback_flag_pack_s(struct mqtt_payload_suback_flag * p_flag){
	//!< chekc parameter
	MQTT_PACKET_FLAG_VAL_BOOL_CHECK(p_flag->ok);
	MQTT_PACKET_FLAG_VAL_QoS_CHECK(p_flag->QoS);
	//!< pack flag byte
	return MQTT_PAYLOAD_SUBACK_FLAG_PACK(p_flag);
}

