#include "../../inc/core/mqtt_fixed_header.h"
#include "../../inc/core/mqtt_packet_segment.h"

#include <stdlib.h>

uint8_t mqtt_ctl_flag_pack_s(struct mqtt_ctl_flag * p_flag){
        //!< check parameters
	MQTT_CTL_TYPE_CHECK(p_flag->type);
	MQTT_CTL_FLAG_DUP_CHECK(p_flag->DUP);
	MQTT_CTL_FLAG_QoS_CHECK(p_flag->QoS);
	MQTT_CTL_FLAG_RETAIN_CHECK(p_flag->RETAIN);

	//!< pack parameter
	/*
	head = (p_flag->type << MQTT_CTL_TYPE_OFFSET) |  //!< type
		(p_flag->DUP << MQTT_CTL_FLAG_DUP_OFFSET) |  //!< DUP
		(p_flag->QoS << MQTT_CTL_FLAG_QoS_OFFSET) |  //!< QoS
		(p_flag->RETAIN << MQTT_CTL_FLAG_RETAIN_OFFSET);  //!< RETAIN
	*/
	return MQTT_CTL_FLAG_PACK(p_flag);
}

struct mqtt_ctl_flag * mqtt_ctl_flag_unpack(uint8_t flag){
	struct mqtt_ctl_flag * p_flag = malloc(sizeof(struct mqtt_ctl_flag));

	p_flag->type   = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_TYPE_Msk,MQTT_CTL_TYPE_OFFSET);
	p_flag->DUP    = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_DUP_Msk,MQTT_CTL_FLAG_DUP_OFFSET);
	p_flag->QoS    = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_QoS_Msk,MQTT_CTL_FLAG_QoS_OFFSET);
	p_flag->RETAIN = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_RETAIN_Msk,MQTT_CTL_FLAG_RETAIN_OFFSET);

	return p_flag;
}

uint32_t mqtt_ctl_decode_remaining_len(const uint8_t * code){
	int multiplier = 1;
	uint32_t value = 0;
	do{
		if(multiplier > 128*128*128){
			//!< err `Malformaed Remaining Length`
			return -1;
		}
		value += ((*code) & 127) * multiplier;
		multiplier *= 128;
	}while(((*code++) & 128) != 0);

	//!< check length range
	MQTT_CTL_REMAINING_LEN_CHECK(value);
	
	return value;
}

int mqtt_ctl_encode_remaining_len(uint8_t * code, uint32_t length){
	//!< check length range
	MQTT_CTL_REMAINING_LEN_CHECK(length);

	int count = 0;
	do{
		*code = length % 128;
		length /= 128;
		if(0 < length)
			*code |= 128;
		code++;
		count++;
	}while(0 < length);
	
	return count;
}

