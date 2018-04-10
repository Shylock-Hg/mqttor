#include "../../inc/core/mqtt_fixed_header.h"

uint8_t mqtt_ctl_flag_pack_s(struct mqtt_ctl_flag * p_header){
        //!< check parameters
	MQTT_CTL_TYPE_CHECK(p_header->type);
	MQTT_CTL_FLAG_DUP_CHECK(p_header->DUP);
	MQTT_CTL_FLAG_QoS_CHECK(p_header->QoS);
	MQTT_CTL_FLAG_RETAIN_CHECK(p_header->RETAIN);


	//!< pack parameter
	/*
	head = (p_header->type << MQTT_CTL_TYPE_OFFSET) |  //!< type
		(p_header->DUP << MQTT_CTL_FLAG_DUP_OFFSET) |  //!< DUP
		(p_header->QoS << MQTT_CTL_FLAG_QoS_OFFSET) |  //!< QoS
		(p_header->RETAIN << MQTT_CTL_FLAG_RETAIN_OFFSET);  //!< RETAIN
	*/
	return MQTT_CTL_FLAG_PACK(p_header);
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

