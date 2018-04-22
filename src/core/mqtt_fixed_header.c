#include <stdlib.h>
#include <string.h>

#include "../../inc/core/mqtt_fixed_header.h"

struct mqtt_buf_ctl_flag * mqtt_ctl_flag_pack(union mqtt_attr_ctl_flag flag){
        //!< check parameters
	MQTT_CTL_TYPE_CHECK(flag.bits.type);
	//MQTT_CTL_FLAG_DUP_CHECK(p_flag.DUP);
	MQTT_CTL_FLAG_QoS_CHECK(flag.bits.QoS);
	//MQTT_CTL_FLAG_RETAIN_CHECK(p_flag.RETAIN);

	struct mqtt_buf_ctl_flag * p_buf_ctl_flag = mqtt_buf_new(sizeof(uint8_t));
	memcpy(p_buf_ctl_flag->buf,&flag,sizeof(flag));

	//!< pack parameter
	/*
	head = (p_flag->type << MQTT_CTL_TYPE_OFFSET) |  //!< type
		(p_flag->DUP << MQTT_CTL_FLAG_DUP_OFFSET) |  //!< DUP
		(p_flag->QoS << MQTT_CTL_FLAG_QoS_OFFSET) |  //!< QoS
		(p_flag->RETAIN << MQTT_CTL_FLAG_RETAIN_OFFSET);  //!< RETAIN
	*/
	//return MQTT_CTL_FLAG_PACK(p_flag);
	return p_buf_ctl_flag;
}

union mqtt_attr_ctl_flag mqtt_ctl_flag_unpack(const struct mqtt_buf_ctl_flag * p_buf_ctl_flag){
	/*
	struct mqtt_ctl_flag * p_flag = malloc(sizeof(struct mqtt_ctl_flag));

	p_flag->type   = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_TYPE_Msk,MQTT_CTL_TYPE_OFFSET);
	p_flag->DUP    = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_DUP_Msk,MQTT_CTL_FLAG_DUP_OFFSET);
	p_flag->QoS    = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_QoS_Msk,MQTT_CTL_FLAG_QoS_OFFSET);
	p_flag->RETAIN = MQTT_PACKET_SEGMENT_EVAL(flag,MQTT_CTL_FLAG_RETAIN_Msk,MQTT_CTL_FLAG_RETAIN_OFFSET);
*/
	return MQTT_CTL_FLAG_UNPACK(p_buf_ctl_flag);
}


mqtt_attr_re_len_t mqtt_ctl_decode_remaining_len(const struct mqtt_buf_re_len * mq_buf_re_len){
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

