/*! \brief unit test for core/mqtt_fixed_header
 *  \author Shylock Hg
 *  \data 2018-03-21
 * */

#include "../../inc/core/mqtt_packet_segment.h"

#include "../../inc/core/mqtt_fixed_header.h"
#include "../../inc/core/mqtt_var_header.h"
#include "../../inc/core/mqtt_payload.h"
#include "../../inc/toolkit/array.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_fixed_header(void);
void test_var_header(void);
void test_toolkit(void);
void test_payload(void);

void test_packet_segment(void);

int main(int argc, char * argv[]){
	test_fixed_header();
	test_var_header();
	test_toolkit();
	test_payload();
	test_packet_segment();

	return 0;
}

void test_fixed_header(void){
	//!< control packet header byte 
	printf("\n\n");
	printf("**********mqtt fixed header test**********\n");
	struct mqtt_ctl_flag header = {
		MQTT_CTL_TYPE_CONNACK,  //!< type
		1,  //!< DUP
		1,  //!< QoS
		0,  //!< RETAIN
	};
	uint8_t s_head = mqtt_ctl_flag_pack_s(&header);
	printf("eval:type=%d,DUP=%d,QoS=%d,RETAIN=%d\n",
			MQTT_PACKET_SEGMENT_EVAL(s_head,MQTT_CTL_TYPE_Msk,MQTT_CTL_TYPE_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(s_head,MQTT_CTL_FLAG_DUP_Msk,MQTT_CTL_FLAG_DUP_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(s_head,MQTT_CTL_FLAG_QoS_Msk,MQTT_CTL_FLAG_QoS_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(s_head,MQTT_CTL_FLAG_RETAIN_Msk,MQTT_CTL_FLAG_RETAIN_OFFSET));

	//!< control packet header remaining length
	uint32_t length[] = {0,127,128,16383,16384,2097151,2097152,268435455};
	uint8_t code[4] = {0};
	for(int i=0; i<COUNT_OF_ARRAY(length); i++){
		int size = mqtt_ctl_encode_remaining_len(code,length[i]);
		uint32_t len = mqtt_ctl_decode_remaining_len(code);
		printf("[info]:origin=%u,encode=[%d]-%2x-%2x-%2x-%2x,decode=%u.\r\n",
				length[i],size,*(code+0),*(code+1),*(code+2),*(code+3),len);
		memset(code,0x00,sizeof(code));
	}
}

void test_var_header(void){
	printf("\n\n");
	printf("**********mqtt variable header test**********\n");
	struct mqtt_connect_flag conn_flag = {
		1,  //!< flag user name
		0,  //!< flag password
		1,  //!< flag will retain
		2,  //!< flag will QoS
		1,  //!< flag will flag
		0,  //!< flag clean session
		//1,  //!< flag reserved
	};
	uint8_t conn_flag_byte = mqtt_connect_flag_pack_s(&conn_flag);
	printf("conn_flag_byte = %2x\n",conn_flag_byte);
	printf("evaluate vlaue of flags by order = %2x,%2x,%2x,%2x,%2x,%2x,%2x\n",
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_USER_NAME_Msk,MQTT_CONNECT_FLAG_USER_NAME_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_PWD_Msk,MQTT_CONNECT_FLAG_PWD_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_W_RETAIN_Msk,MQTT_CONNECT_FLAG_W_RETAIN_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_W_QoS_Msk,MQTT_CONNECT_FLAG_W_QoS_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_W_FLAG_Msk,MQTT_CONNECT_FLAG_W_FLAG_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_CLEAN_SESSION_Msk,MQTT_CONNECT_FLAG_CLEAN_SESSION_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(conn_flag_byte,MQTT_CONNECT_FLAG_RESERVED_Msk,MQTT_CONNECT_FLAG_RESERVED_OFFSET));
}

void test_payload(void){
	printf("\n\n");
	printf("**********mqtt payload test**********\n");
	struct mqtt_payload_suback_flag suback_flag = {
		1,  //!< ok
		2,  //!< QoS
	};
	uint8_t suback_flag_byte = mqtt_payload_suback_flag_pack_s(&suback_flag);
	printf("suback_flag_byte=%2x\n",suback_flag_byte);
	printf("evaluate value of suback flags `ok=%2x` , `QoS=%2x`\n",
			MQTT_PACKET_SEGMENT_EVAL(suback_flag_byte,MQTT_PAYLOAD_SUBACK_FLAG_OK_Msk,MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET),
			MQTT_PACKET_SEGMENT_EVAL(suback_flag_byte,MQTT_PAYLOAD_SUBACK_FLAG_QoS_Msk,MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET));
}

void test_toolkit(void){
	printf("\n\n");
	printf("**********mqtt toolkit test**********\n");
	uint16_t value = 0x1234;
	printf("origin=%x\n",value);
	uint8_t bytes[2] = {0};
	UINT16_2_BYTES(value,bytes);
	value = BYTES_2_UINT16(bytes);
	printf("endecode=%x\n",value);
}

void test_packet_segment(void){
	printf("\n\n");
	printf("**********mqtt packet segment test**********\n");
	char * str = "hello world!";
	printf("[info]:origin string `%s`.\n",str);

	const uint8_t * code = mqtt_packet_str_encode(str);
	const char * _str = mqtt_packet_str_decode(code);

	printf("[info]:decode string `%s`.\n",_str);

	free((void*)code);
	free((void*)_str);
}

