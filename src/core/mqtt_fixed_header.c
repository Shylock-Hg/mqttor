#include <stdlib.h>
#include <string.h>

#include "../../inc/core/mqtt_fixed_header.h"

struct mqtt_buf_ctl_flag * mqtt_ctl_flag_pack(union mqtt_attr_ctl_flag flag){
        //< check parameters
	MQTT_CTL_TYPE_CHECK(flag.bits.type);
	MQTT_CTL_FLAG_QoS_CHECK(flag.bits.QoS);

	//< new mqtt buf
	struct mqtt_buf_ctl_flag * p_buf_ctl_flag = mqtt_buf_new(sizeof(uint8_t));
	p_buf_ctl_flag->buf[0] = MQTT_CTL_FLAG_PACK(flag);

	return p_buf_ctl_flag;
}

union mqtt_attr_ctl_flag mqtt_ctl_flag_unpack_low(const struct mqtt_buf_ctl_flag * p_buf_ctl_flag){
	union mqtt_attr_ctl_flag flag = MQTT_CTL_FLAG_UNPACK(p_buf_ctl_flag);

        //< check parameters
	MQTT_CTL_TYPE_CHECK(flag.bits.type);
	MQTT_CTL_FLAG_QoS_CHECK(flag.bits.QoS);

	return flag;
}

union mqtt_attr_ctl_flag mqtt_ctl_flag_unpack(uint8_t ** p_packet){

#define packet (*p_packet)
	struct mqtt_buf_ctl_flag * p_buf_packet = mqtt_buf_new(sizeof(uint8_t));
	p_buf_packet->buf[0] = packet[0];
	packet++;  //!< scroll to next fragment

	union mqtt_attr_ctl_flag flag =  mqtt_ctl_flag_unpack_low(p_buf_packet);
	mqtt_buf_release(p_buf_packet);

	return flag;
#undef packet
}


mqtt_attr_re_len_t mqtt_ctl_decode_remaining_len_low(const struct mqtt_buf_re_len * mq_buf_re_len){
	int multiplier = 1;
	mqtt_attr_re_len_t value = 0;
	uint8_t * data = calloc(MQTT_CTL_REMAINING_MAX_LEN_BYTE,sizeof(uint8_t));
	uint8_t * code = data;
	memcpy(code,mq_buf_re_len->buf,mq_buf_re_len->len);
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

	free(data);
	
	return value;
}
mqtt_attr_re_len_t mqtt_ctl_decode_remaining_len(uint8_t ** p_packet){
#define code (*p_packet)
	int multiplier = 1;
	mqtt_attr_re_len_t value = 0;
	do{
		if(multiplier > 128*128*128){
			//!< err `Malformaed Remaining Length`
			return -1;
		}
		value += ((*code) & 127) * multiplier;
		multiplier *= 128;
	}while(((*code++) & 128) != 0);

	return value;
#undef code
}

struct mqtt_buf_re_len * mqtt_ctl_encode_remaining_len(mqtt_attr_re_len_t mq_attr_re_len){
	//!< check length range
	MQTT_CTL_REMAINING_LEN_CHECK(mq_attr_re_len);

	uint8_t * data = calloc(MQTT_CTL_REMAINING_MAX_LEN_BYTE,sizeof(uint8_t));
	uint8_t * code = data;

	int count = 0;
	do{
		*code = mq_attr_re_len % 128;
		mq_attr_re_len /= 128;
		if(0 < mq_attr_re_len)
			*code |= 128;
		code++;
		count++;
	}while(0 < mq_attr_re_len);


	struct mqtt_buf_re_len * mq_buf_re_len = mqtt_buf_new(count);
	memcpy(mq_buf_re_len->buf,data,count);
	
	free(data);

	return mq_buf_re_len;
}

