/*! \brief unit test for core/mqtt_fixed_header
 *  \author Shylock Hg
 *  \data 2018-03-21
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../../inc/core/mqtt_fixed_header.h"
#include "../../inc/core/mqtt_var_header.h"
#include "../../inc/core/mqtt_payload.h"

#include "../../inc/core/mqtt_packet.h"

#include "../../inc/toolkit/mqtt_log.h"

void test_toolkit(void);
void test_packet_segment(void);

void test_fixed_header(void);
void test_var_header(void);
void test_payload(void);

void test_packet(void);


int main(int argc, char * argv[]){
	/*
	test_toolkit();
	test_packet_segment();
	test_fixed_header();
	test_var_header();
	test_payload();
	*/

	test_packet();

	return 0;
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
	mqtt_attr_str_t str = "hello world!";
	printf("[info]:origin string `%s`.\n",str);

	struct mqtt_buf_str * mq_str = mqtt_buf_str_encode(str);
	printf("[info]:code length `%ld`\n",mq_str->len);
	mqtt_attr_str_t _str = mqtt_buf_str_decode(mq_str);

	printf("[info]:decode string `%s`.\n",_str);

	free(_str);
	mqtt_buf_release(mq_str);
}

void test_fixed_header(void){
	//!< control packet header byte 
	printf("\n\n");
	printf("**********mqtt fixed header test**********\n");
	union mqtt_attr_ctl_flag flag = {
		.bits = { 
			.type   = MQTT_CTL_TYPE_SUBSCRIBE,  //!< type
			.DUP    = 1,  //!< DUP
			.QoS    = 2,  //!< QoS
			.RETAIN = 0,  //!< RETAIN
		}
	};
	struct mqtt_buf_ctl_flag * p_buf_ctl_flag = mqtt_ctl_flag_pack(flag);
	union mqtt_attr_ctl_flag  flag2 = mqtt_ctl_flag_unpack_low(p_buf_ctl_flag);
	printf("[info]:sizeof(union mqtt_attr_ctl_flag)=%lu\n",sizeof(union mqtt_attr_ctl_flag));
	printf("eval:all=0x%2x,type=%d,DUP=%d,QoS=%d,RETAIN=%d\n",\
			flag2.all,\
			flag2.bits.type,\
			flag2.bits.DUP,\
			flag2.bits.QoS,\
			flag2.bits.RETAIN);
	mqtt_buf_release(p_buf_ctl_flag);

	//!< control packet header remaining length
	mqtt_attr_re_len_t length[] = {0,127,128,16383,16384,2097151,2097152,268435455};
	for(int i=0; i<COUNT_OF_ARRAY(length); i++){
		//int size = mqtt_ctl_encode_remaining_len(code,length[i]);
		//uint32_t len = mqtt_ctl_decode_remaining_len(code);
		struct mqtt_buf_re_len * mq_buf_re_len = mqtt_ctl_encode_remaining_len(length[i]);
		uint8_t * buf = mq_buf_re_len->buf;
		size_t len_bytes = 0;
		mqtt_attr_re_len_t len = mqtt_ctl_decode_remaining_len(buf,&len_bytes);

		printf("[info]:(int)buf-(int)->buf=`%ld`\n",
				(uintptr_t)buf-(uintptr_t)(mq_buf_re_len->buf));
		printf("[info]:encode=");
		for(int j = 0; j < mq_buf_re_len->len; j++){
			printf("%2x,",*(mq_buf_re_len->buf+j));
		}
		printf("\n");

		printf("[info]:origin=%u,decode=%u,len_bytes=%lu.\n",
				length[i],len,len_bytes);

		mqtt_buf_release(mq_buf_re_len);
	}
}

void test_var_header(void){
	printf("\n\n");
	printf("**********mqtt variable header test**********\n");
	union mqtt_attr_connect_flag conn_flag = {
		.bits = {
			0,  //!< flag reserved
			0,  //!< flag clean session
			1,  //!< flag will flag
			2,  //!< flag will QoS
			1,  //!< flag will retain
			0,  //!< flag password
			1,  //!< flag user name
		}
	};
	struct mqtt_buf_connect_flag * p_conn_buf_flag = mqtt_connect_flag_pack(conn_flag);
	union mqtt_attr_connect_flag conn_attr_flag = mqtt_connect_flag_unpack(p_conn_buf_flag);
	printf("conn_flag_byte = 0x%2x\n",p_conn_buf_flag->buf[0]);
	printf("sizeof(conn_attr_flag)=`%lu`!\n",sizeof(conn_attr_flag));
	printf("evaluate vlaue of flags by order = %2x,%2x,%2x,%2x,%2x,%2x,%2x\n",
			conn_attr_flag.bits.flag_user_name,
			conn_attr_flag.bits.flag_pwd,
			conn_attr_flag.bits.flag_w_retain,
			conn_attr_flag.bits.flag_w_QoS,
			conn_attr_flag.bits.flag_w_flag,
			conn_attr_flag.bits.flag_clean_session,
			conn_attr_flag.bits.flag_reserved);
	mqtt_buf_release(p_conn_buf_flag);

	union mqtt_attr_connack_flag connack_flag = {
		.bits = {
			1,
			0
		}
	};
	struct mqtt_buf_connack_flag * p_buf_connack_flag = mqtt_connack_flag_pack(connack_flag);
	union mqtt_attr_connack_flag connack_attr_flag = mqtt_connack_flag_unpack(p_buf_connack_flag);
	printf("connack_flag_byte = 0x%2x\n",p_buf_connack_flag->buf[0]);
	printf("sizeof(connack_attr_flag)=`%lu`!\n",sizeof(connack_attr_flag));
	printf("evaluete value of flags by order = 0x%2x,0x%2x\n",
			connack_attr_flag.bits.reserved,
			connack_attr_flag.bits.SP);
	mqtt_buf_release(p_buf_connack_flag);
}

void test_payload(void){
	printf("\n\n");
	printf("**********mqtt payload test**********\n");
	union mqtt_attr_payload_suback_flag suback_flag = {
		.bits = {
			2,  //!< QoS
			0,
			1,  //!< ok
		}
	};
	struct mqtt_buf_payload_suback_flag *  p_buf_flag = 
		mqtt_payload_suback_flag_pack(suback_flag);
	union mqtt_attr_payload_suback_flag suback_attr_flag = 
		mqtt_payload_suback_flag_unpack(
			p_buf_flag
			);
	printf("suback_flag_byte=0x%2x\n",p_buf_flag->buf[0]);
	printf("evaluate value of suback flags `all=0x%2x` `ok=%2x` , `QoS=%2x`\n",
			suback_attr_flag.all,
			suback_attr_flag.bits.ok,
			suback_attr_flag.bits.QoS);

	mqtt_buf_release(p_buf_flag);

	union mqtt_attr_payload_subscribe_content_QoS subscribe_qos = {
		.bits = {
			2,   //!< QoS
			0    //!< reserved
		}
	};
	struct mqtt_buf_payload_subscribe_content_QoS * p_buf_subscribe_flag = 
		mqtt_payload_subscribe_content_QoS_pack(subscribe_qos);
	union mqtt_attr_payload_subscribe_content_QoS subscribe_attr_flag =
		mqtt_payload_subscribe_content_QoS_unpack(p_buf_subscribe_flag);
	printf("subscribe QoS flag byte=0x%2x\n",p_buf_subscribe_flag->buf[0]);
	printf("evaluate value of subscribe content flags `reserved=0x%2x`,`QoS=0x%2x`",
			subscribe_attr_flag.bits.reserved,
			subscribe_attr_flag.bits.QoS);
	mqtt_buf_release(p_buf_subscribe_flag);
}

#define TEST_IP   "127.0.0.1"
#define TEST_PORT 1883

void test_packet(void){
	int sock;
	/*
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = in_aton(TEST_IP);//inet_addr(TEST_IP);
	*/
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	/*
	struct mqtt_attr_packet attr_connect = {
		.attr_packet = {
			.connect = {
				.flag = {
					.bits = {
						0,  //!< flag reserved
						0,  //!< flag clean session
						1,  //!< flag will flag
						2,  //!< flag will QoS
						1,  //!< flag will retain
						1,  //!< flag password
						1,  //!< flag user name
					}
				},
				.keep_alive = 256,
				//.keep_alive = 256,
				//.id_client = "shylock-archlinux",
				//.w_topic = "/topic/test",
				//.w_msg = "hello world",
				//.user = "shylock",
				//.pwd = "huangshihai",
			}
		}
	};
	*/
	mqtt_attr_packet_t * attr_connect = mqtt_attr_packet_new(1024);
	attr_connect->attr_packet.connect.flag.all = 0xF4;
	attr_connect->attr_packet.connect.keep_alive = 256;
	//attr_connect->payload = mqtt_attr_payload_new(1024);
	//!< id client
	mqtt_attr_payload_write_string(attr_connect->payload,"shylock-archlinux");
	//mqtt_log_print_buf(attr_connect->payload->buf, attr_connect->payload->len_valid);
	//!< w_topic
	mqtt_attr_payload_write_string(attr_connect->payload, "topic/test");
	//!< w_msg
	mqtt_attr_payload_write_string(attr_connect->payload, "hello world");
	//!< user
	mqtt_attr_payload_write_string(attr_connect->payload, "shylock");
	//!< pwd
	mqtt_attr_payload_write_string(attr_connect->payload, "huangshihai");

	//< pack packet
	//mqtt_buf_packet_t * buf_packet = mqtt_pack_connect(attr_connect);
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_connect(attr_connect, &buf_packet);

	//< unpack packet
	//mqtt_attr_packet_t * attr_packet = mqtt_unpack_connect(buf_packet);
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_connect(buf_packet, &attr_packet);
	printf("header = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `%d`\n", attr_packet->remaining_length);
	printf("connect flag = `0x%2x`\n",attr_packet->attr_packet.connect.flag.all);
	printf("keepalive = `%d`\n", attr_packet->attr_packet.connect.keep_alive);
	mqtt_log_print_buf(attr_packet->payload->buf,attr_packet->payload->len);

	//!< don't free in mqtt_pack_connect
	//mqtt_attr_payload_release(attr_connect->payload); 
	mqtt_attr_packet_release(attr_packet);
	mqtt_attr_packet_release(attr_connect);

	if(-1 == (sock = socket(addr.sin_family,SOCK_STREAM,IPPROTO_TCP))){
		fprintf(stderr, "Creat socket failed!\n");
		fflush(stderr);
		exit(-1);
	}

	if(0 != connect(sock, (struct sockaddr*)&addr, sizeof(addr))){
		fprintf(stderr, "Connect failed!\n");
		fflush(stderr);
		exit(-1);
	}

	int count = send(sock, buf_packet->buf, buf_packet->len, 0);
	printf("send connect len `%d`!\n",count);
	//mqtt_log_print_buf(buf_packet->buf, buf_packet->len);
	close(sock);

	mqtt_buf_release(buf_packet);
}

