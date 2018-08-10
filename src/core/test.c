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
void test_packet_connack(void);
void test_packet_publish(void);
void test_packet_puback(void);
void test_packet_pubrec(void);
void test_packet_pubrel(void);
void test_packet_pubcomp(void);
void test_packet_subscribe(void);
void test_packet_suback(void);
void test_packet_unsubscribe(void);
void test_packet_unsuback(void);
void test_packet_pingreq(void);
void test_packet_pingresp(void);

int main(int argc, char * argv[]){
	/*
	test_toolkit();
	test_packet_segment();
	test_fixed_header();
	test_var_header();
	test_payload();
	*/

	//test_packet();
	//test_packet_connack();
	//test_packet_publish();
	//test_packet_puback();
	//test_packet_pubrec();
	//test_packet_pubrel();
	//test_packet_pubcomp();
	//test_packet_subscribe();
	//test_packet_suback();
	//test_packet_unsubscribe();
	//test_packet_unsuback();
	//test_packet_pingreq();
	test_packet_pingresp();

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
	mqtt_attr_str_t __str = mqtt_buf_str_4_buf(mq_str->buf);

	printf("[info]:decode _str `%s`.\n",_str);
	mqtt_log_print_buf(mq_str->buf, mq_str->len);
	printf("[info]:decode __str `%s`.\n", __str);

	free(__str);
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
	union mqtt_attr_payload_suback_ret_code suback_ret_code = {
		.bits = {
			2,  //!< QoS
			0,
			1,  //!< ok
		}
	};
	struct mqtt_buf_payload_suback_ret_code *  p_buf_ret_code = 
		mqtt_payload_suback_ret_code_pack(suback_ret_code);
	union mqtt_attr_payload_suback_ret_code suback_attr_ret_code = 
		mqtt_payload_suback_ret_code_unpack(
			p_buf_ret_code
			);
	printf("suback_ret_code_byte=0x%2x\n",p_buf_ret_code->buf[0]);
	printf("evaluate value of suback ret_codes `all=0x%2x` `ok=%2x` , `QoS=%2x`\n",
			suback_attr_ret_code.all,
			suback_attr_ret_code.bits.ok,
			suback_attr_ret_code.bits.QoS);

	mqtt_buf_release(p_buf_ret_code);

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

void test_packet_connack(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_connack = mqtt_attr_packet_new(0);
	attr_connack->attr_packet.connack.flag.all = 0x01;
	attr_connack->attr_packet.connack.ret_code = CONNECT_RET_CODE_REF_ID;

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_connack(attr_connack, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_connack(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("connack flag = `0x%2x`\n", 
			attr_packet->attr_packet.connack.flag.all);
	printf("connack return code = `0x%2x`\n", 
			attr_packet->attr_packet.connack.ret_code);
	
	mqtt_attr_packet_release(attr_connack);

	//< transmite
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

void test_packet_publish(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_publish = mqtt_attr_packet_new(1024);
	//< filling packet attributes
	//< hdr
	attr_publish->hdr.bits.DUP = 1;
	attr_publish->hdr.bits.QoS = 0;
	attr_publish->hdr.bits.RETAIN = 1;
	attr_publish->hdr.bits.type = MQTT_CTL_TYPE_PUBLISH;
	//< variable 
	attr_publish->attr_packet.publish.id_packet = 0x9527;
	attr_publish->attr_packet.publish.topic_name = "test";
	//< payload
	mqtt_attr_payload_write_string(attr_publish->payload, "content/helloworld");

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_publish(attr_publish, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_publish(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("topic is `%s`\n", attr_packet->attr_packet.publish.topic_name);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.publish.id_packet);
	
	mqtt_attr_packet_release(attr_publish);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_puback(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_puback = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_puback->hdr.bits.DUP = 1;
	attr_puback->hdr.bits.QoS = 0;
	attr_puback->hdr.bits.RETAIN = 1;
	attr_puback->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
	//< variable 
	attr_puback->attr_packet.puback.id_packet = 0x9527;
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_puback(attr_puback, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_puback(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.puback.id_packet);
	
	mqtt_attr_packet_release(attr_puback);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_pubrec(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_pubrec = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_pubrec->hdr.bits.DUP = 1;
	attr_pubrec->hdr.bits.QoS = 0;
	attr_pubrec->hdr.bits.RETAIN = 1;
	attr_pubrec->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
	//< variable 
	attr_pubrec->attr_packet.pubrec.id_packet = 0x9527;
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_pubrec(attr_pubrec, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_pubrec(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.pubrec.id_packet);
	
	mqtt_attr_packet_release(attr_pubrec);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_pubrel(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_pubrel = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_pubrel->hdr.bits.DUP = 1;
	attr_pubrel->hdr.bits.QoS = 0;
	attr_pubrel->hdr.bits.RETAIN = 1;
	attr_pubrel->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
	//< variable 
	attr_pubrel->attr_packet.pubrel.id_packet = 0x9527;
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_pubrel(attr_pubrel, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_pubrel(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.pubrel.id_packet);
	
	mqtt_attr_packet_release(attr_pubrel);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_pubcomp(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_pubcomp = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_pubcomp->hdr.bits.DUP = 1;
	attr_pubcomp->hdr.bits.QoS = 0;
	attr_pubcomp->hdr.bits.RETAIN = 1;
	attr_pubcomp->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
	//< variable 
	attr_pubcomp->attr_packet.pubcomp.id_packet = 0x9527;
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_pubcomp(attr_pubcomp, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_pubcomp(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.pubcomp.id_packet);
	
	mqtt_attr_packet_release(attr_pubcomp);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_subscribe(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_subscribe = mqtt_attr_packet_new(1024);
	//< filling packet attributes
	//< hdr
	attr_subscribe->hdr.bits.DUP = 1;
	attr_subscribe->hdr.bits.QoS = 0;
	attr_subscribe->hdr.bits.RETAIN = 1;
	attr_subscribe->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
	//< variable 
	attr_subscribe->attr_packet.subscribe.id_packet = 0x9527;
	//< payload
	mqtt_attr_payload_write_string(attr_subscribe->payload, "test");
	mqtt_attr_payload_write_byte(attr_subscribe->payload, 0x01);
	mqtt_attr_payload_write_string(attr_subscribe->payload, "topic");
	mqtt_attr_payload_write_byte(attr_subscribe->payload, 0x02);

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_subscribe(attr_subscribe, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_subscribe(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.subscribe.id_packet);
	mqtt_log_print_buf(attr_packet->payload->buf, attr_packet->payload->len);
	
	mqtt_attr_packet_release(attr_subscribe);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_suback(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_suback = mqtt_attr_packet_new(1024);
	//< filling packet attributes
	//< hdr
	attr_suback->hdr.bits.DUP = 1;
	attr_suback->hdr.bits.QoS = 0;
	attr_suback->hdr.bits.RETAIN = 1;
	attr_suback->hdr.bits.type = MQTT_CTL_TYPE_SUBACK;
	//< variable 
	attr_suback->attr_packet.suback.id_packet = 0x9527;
	//< payload
	mqtt_attr_payload_suback_ret_code_t ret_codes[2] = {{
		.bits = {
			.ok = 1,
			.reserved = 0,
			.QoS =1,
		}
	},{
		.bits = {
			.ok = 0,
			.reserved = 0,
			.QoS = 2,
		}
	}};
	mqtt_attr_payload_write_bytes(attr_suback->payload, 
			(uint8_t*)ret_codes, sizeof(ret_codes));

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_suback(attr_suback, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_suback(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.suback.id_packet);
	mqtt_log_print_buf(attr_packet->payload->buf, attr_packet->payload->len);
	
	mqtt_attr_packet_release(attr_suback);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_unsubscribe(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_unsubscribe = mqtt_attr_packet_new(1024);
	//< filling packet attributes
	//< hdr
	attr_unsubscribe->hdr.bits.DUP = 1;
	attr_unsubscribe->hdr.bits.QoS = 0;
	attr_unsubscribe->hdr.bits.RETAIN = 1;
	attr_unsubscribe->hdr.bits.type = MQTT_CTL_TYPE_UNSUBSCRIBE;
	//< variable 
	attr_unsubscribe->attr_packet.unsubscribe.id_packet = 0x9527;
	//< payload
	mqtt_attr_payload_write_string(attr_unsubscribe->payload, "test");
	mqtt_attr_payload_write_string(attr_unsubscribe->payload, "topic");

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_unsubscribe(attr_unsubscribe, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_unsubscribe(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.unsubscribe.id_packet);
	mqtt_log_print_buf(attr_packet->payload->buf, attr_packet->payload->len);
	
	mqtt_attr_packet_release(attr_unsubscribe);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_unsuback(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_unsuback = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_unsuback->hdr.bits.DUP = 1;
	attr_unsuback->hdr.bits.QoS = 0;
	attr_unsuback->hdr.bits.RETAIN = 1;
	attr_unsuback->hdr.bits.type = MQTT_CTL_TYPE_UNSUBSCRIBE;
	//< variable 
	attr_unsuback->attr_packet.unsuback.id_packet = 0x9527;
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_unsuback(attr_unsuback, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_unsuback(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	printf("packet identify is `0x%4x`\n", 
			attr_packet->attr_packet.unsuback.id_packet);
	
	mqtt_attr_packet_release(attr_unsuback);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_pingreq(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_pingreq = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_pingreq->hdr.bits.DUP = 1;
	attr_pingreq->hdr.bits.QoS = 0;
	attr_pingreq->hdr.bits.RETAIN = 1;
	attr_pingreq->hdr.bits.type = MQTT_CTL_TYPE_PINGREQ;
	//< variable 
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_pingreq(attr_pingreq, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_pingreq(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	
	mqtt_attr_packet_release(attr_pingreq);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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

void test_packet_pingresp(void){
	int sock;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TEST_PORT);
	addr.sin_addr.s_addr = inet_addr(TEST_IP);
	

	mqtt_attr_packet_t * attr_pingresp = mqtt_attr_packet_new(0);
	//< filling packet attributes
	//< hdr
	attr_pingresp->hdr.bits.DUP = 1;
	attr_pingresp->hdr.bits.QoS = 0;
	attr_pingresp->hdr.bits.RETAIN = 1;
	attr_pingresp->hdr.bits.type = MQTT_CTL_TYPE_PINGREQ;
	//< variable 
	//< payload

	//< pack packet
	mqtt_buf_packet_t * buf_packet;
	mqtt_pack_pingresp(attr_pingresp, &buf_packet);
	//< unpack packet
	mqtt_attr_packet_t * attr_packet;
	mqtt_unpack_pingresp(buf_packet, &attr_packet);
	printf("hdr = `0x%2x`\n", attr_packet->hdr.all);
	printf("remaining length = `0x%u`\n", attr_packet->remaining_length);
	
	mqtt_attr_packet_release(attr_pingresp);
	mqtt_attr_packet_release(attr_packet);

	//< transmite
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
