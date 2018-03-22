/*  \brief unit test for core/mqtt_fixed_header
 *  \author Shylock Hg
 *  \data 2018-03-21
 * */

#include "../../inc/mqtt_fixed_header.h"
#include <stdio.h>
#include "../toolkit/array.h"

int main(int argc, char * argv[]){
	//!< control packet header byte 
	struct mqtt_ctl_head header = {
		MQTT_CTL_TYPE_CONNACK,  //!< type
		1,  //!< DUP
		1,  //!< QoS
		0,  //!< RETAIN
	};
	uint8_t s_head = mqtt_ctl_head_pack_s(&header);
	uint8_t head   = MQTT_CTL_HEAD_PACK((&header));
	printf("eval:type=%d,DUP=%d,QoS=%d,RETAIN=%d\n",
			MQTT_CTL_TYPE_EVAL(s_head),
			MQTT_CTL_FLAG_DUP_EVAL(s_head),
			MQTT_CTL_FLAG_QoS_EVAL(s_head),
			MQTT_CTL_FLAG_RETAIN_EVAL(s_head));
	printf("eval:type=%d,DUP=%d,QoS=%d,RETAIN=%d\n",
			MQTT_CTL_TYPE_EVAL(head),
			MQTT_CTL_FLAG_DUP_EVAL(head),
			MQTT_CTL_FLAG_QoS_EVAL(head),
			MQTT_CTL_FLAG_RETAIN_EVAL(head));

	//!< control packet header remaining length
	uint32_t length[] = {0,127,128,16383,16384,2097151,2097152,268435455};
	uint8_t code[4] = {0};
	for(int i=0; i<COUNT_OF_ARRAY(length); i++){
		mqtt_ctl_encode_remaining_len(code,length[i]);
		uint32_t len = mqtt_ctl_decode_remaining_len(code);
		printf("[info]:origin=%d,encode_decode=%d.\r\n",length[i],len);
	}

	return 0;
}
