/*! \file mqtt_buf_packet.c
 *  \brief mqtt packet pack&unpack functions
 *  \author Shylock Hg
 *  \date 2018-04-10
 * */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../inc/core/mqtt_packet.h"
#include "../../inc/toolkit/mqtt_log.h"



mqtt_attr_packet_t * mqtt_attr_packet_new(size_t len_payload){
	//assert(len_payload);

	mqtt_attr_packet_t * packet = malloc(sizeof(mqtt_attr_packet_t));
	assert(packet);
	packet->payload = mqtt_attr_payload_new(len_payload);
	if(0 != len_payload)
		assert(packet->payload);

	return packet;
}

void mqtt_attr_packet_release(mqtt_attr_packet_t * packet){
	assert(packet);

	mqtt_attr_payload_release(packet->payload);
	free(packet);
}


//#define MQTT_PACK_CONNECT_VAR_LEN 10

int mqtt_pack_connect(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		){
	mqtt_attr_re_len_t remaining_length = 0;
	struct mqtt_buf * mqtt_buffer_array[20] = {NULL};
	int i = 0;
	/*
	/// payload
	/// password
	struct mqtt_buf_str * p_buf_password = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.pwd);
	if(NULL != p_buf_password){
		mqtt_buffer_array[i++] = p_buf_password;
		remaining_length += p_buf_password->len;
	}
	/// user
	struct mqtt_buf_str * p_buf_user = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.user);
	if(NULL != p_buf_user){
		mqtt_buffer_array[i++] = p_buf_user;
		remaining_length += p_buf_user->len;
	}
	/// w_msg
	struct mqtt_buf_str * p_buf_w_msg = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.w_msg);
	if(NULL != p_buf_w_msg){
		mqtt_buffer_array[i++] = p_buf_w_msg;
		remaining_length += p_buf_w_msg->len;
	}
	/// w_topic
	struct mqtt_buf_str * p_buf_w_topic = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.w_topic);
	if(NULL != p_buf_w_topic){
		mqtt_buffer_array[i++] = p_buf_w_topic;
		remaining_length += p_buf_w_topic->len;
	}
	/// id_client
	struct mqtt_buf_str * p_buf_id_client = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.id_client);
	if(NULL != p_buf_id_client){
		mqtt_buffer_array[i++] = p_buf_id_client;
		remaining_length += p_buf_id_client->len;
	}
	*/
	/*
	mqtt_buf_t * p_buf_payload = mqtt_attr_payload_2_buf(p_attr_packet->payload);
	assert(p_buf_payload);
	if(NULL != p_buf_payload){
		mqtt_buffer_array[i++] = p_buf_payload;
		remaining_length += p_buf_payload->len;
	}
	*/
	mqtt_buf_t * p_buf_payload = mqtt_attr_payload_deep2_buf(
			p_attr_packet->payload);
	assert(p_buf_payload);
	if(NULL != p_buf_payload){
		mqtt_buffer_array[i++] = p_buf_payload;
		remaining_length += p_buf_payload->len;
	}

	/// variable header
	/// keep alive
	struct mqtt_buf_uint16 * p_buf_keep_alive = mqtt_buf_uint16_encode(p_attr_packet->attr_packet.connect.keep_alive);
	mqtt_buffer_array[i++] = p_buf_keep_alive;
	remaining_length += p_buf_keep_alive->len;
	/// connect flags
	struct mqtt_buf_connect_flag * p_buf_connect_flag = mqtt_connect_flag_pack(p_attr_packet->attr_packet.connect.flag);
	mqtt_buffer_array[i++] = p_buf_connect_flag;
	remaining_length += p_buf_connect_flag->len;
	/// level 4
	struct mqtt_buf * p_buf_level = mqtt_buf_new(sizeof(uint8_t));
	p_buf_level->buf[0] = 4;
	mqtt_buffer_array[i++] = p_buf_level;
	remaining_length += p_buf_level->len;
	/// protocol name "MQTT"
	struct mqtt_buf_str * p_buf_mqtt_name = mqtt_buf_str_encode(MQTT_PROTOCOL_NAME);
	mqtt_buffer_array[i++] = p_buf_mqtt_name;
	remaining_length += p_buf_mqtt_name->len;  //!< add varibale header length

	/// fixed header
	/// remaining length
	MQTT_CTL_REMAINING_LEN_CHECK(remaining_length);
	struct mqtt_buf_re_len * p_buf_re_len = mqtt_ctl_encode_remaining_len(remaining_length);
	mqtt_buffer_array[i++] = p_buf_re_len;
	/// ctl flags
	union mqtt_attr_ctl_flag ctl_flag = {
		.bits = {
			.type   = MQTT_CTL_TYPE_CONNECT,
			.DUP    = 0,
			.QoS    = 0,
			.RETAIN = 0
		}
	};
	struct mqtt_buf_ctl_flag * p_buf_ctl_flag = mqtt_ctl_flag_pack(ctl_flag);
	mqtt_buffer_array[i] = p_buf_ctl_flag;

	//< |payload|var header|remaining_length_code|fixed header byte|
	size_t total_len = remaining_length +   //!< length of [payload + var header]
			   p_buf_re_len->len +   //!< length of [remaining_length]
			   p_buf_ctl_flag->len;    //!< length of [fixed header byte]

	//struct mqtt_buf_packet * p_buf_packet = mqtt_buf_new(total_len);
	*pp_buf_packet = mqtt_buf_new(total_len);

	//// copy buffers to mqtt packet then release buffers
	if(NULL != *pp_buf_packet){
		size_t offset = 0;
		for(; i>=0; i--){
			memcpy((*pp_buf_packet)->buf+offset,
					mqtt_buffer_array[i]->buf,
					mqtt_buffer_array[i]->len);
			offset += mqtt_buffer_array[i]->len;
			mqtt_buf_release(mqtt_buffer_array[i]);
		}
		return E_NONE;
	}else{
		printf("[err]:malloc fail!\n");
		return E_MEM_FAIL;
	}
}

int mqtt_unpack_connect(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		){

	size_t offset = 0;
	//< fixed header
	//< control type
	mqtt_attr_ctl_flag_t attr_hdr;
	attr_hdr.all = p_buf_packet->buf[offset++];
	//< remaining length
	size_t len_bytes = 0;
	mqtt_attr_re_len_t attr_remaining_length = mqtt_ctl_decode_remaining_len(
			p_buf_packet->buf+offset, 
			&len_bytes);
	offset += len_bytes;
	
	//< variable header
	//< protocol name
	mqtt_buf_str_t * p_buf_protocol_name = mqtt_buf_new_4_buf(
			p_buf_packet->buf+offset,
			strlen(MQTT_PROTOCOL_NAME)+MQTT_BUF_STR_MAX_BYTE);
	mqtt_attr_str_t attr_protocol_name = mqtt_buf_str_decode(p_buf_protocol_name);
	free(attr_protocol_name);
	mqtt_buf_release(p_buf_protocol_name);
	offset += (strlen(MQTT_PROTOCOL_NAME)+MQTT_BUF_STR_MAX_BYTE);
	//< level
	uint8_t attr_level = p_buf_packet->buf[offset++];
	//< connect flags
	mqtt_attr_connect_flag_t attr_connect_flag;
	attr_connect_flag.all = p_buf_packet->buf[offset++];
	//< keep alive
	mqtt_buf_t * p_buf_keepalive = mqtt_buf_new_4_buf(p_buf_packet->buf+offset,
			sizeof(mqtt_attr_uint16_t));
	mqtt_attr_uint16_t attr_keepalive = mqtt_buf_uint16_decode(p_buf_keepalive);
	mqtt_buf_release(p_buf_keepalive);
	offset += sizeof(mqtt_attr_uint16_t);

	//< payload
	*pp_attr_packet = mqtt_attr_packet_new(
			p_buf_packet->len-offset);
	memcpy((*pp_attr_packet)->payload->buf, p_buf_packet->buf+offset, 
			p_buf_packet->len-offset);
	
	//< filling packet attributes
	(*pp_attr_packet)->hdr = attr_hdr;
	(*pp_attr_packet)->remaining_length = attr_remaining_length;
	(*pp_attr_packet)->attr_packet.connect.flag = attr_connect_flag;
	(*pp_attr_packet)->attr_packet.connect.keep_alive = attr_keepalive;

	return E_NONE;
}

/*
		//< connack packet
		struct {
			//!< fixed header
			
			//!< variable header
			union mqtt_attr_connack_flag flag;  //!< p_connack_flag flags of connack
			enum mqtt_connect_ret_code ret_code;  //!< ret_code return code of connect
			//!< payload
		
		} connack;
*/

int mqtt_pack_connack(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		){
	
	mqtt_attr_re_len_t remaining_length = 0;
	struct mqtt_buf * mqtt_buffer_array[20] = {NULL};
	int i = 0;

	//< payload 
	
	//< variable header
	//< connack return code
	mqtt_buf_t * buf_ret_code = mqtt_buf_new(
			sizeof(mqtt_attr_connack_ret_code_t));
	buf_ret_code->buf[0] = p_attr_packet->attr_packet.connack.ret_code;
	remaining_length++;
	mqtt_buffer_array[i++] = buf_ret_code;
	//< connack flag
	mqtt_buf_t * buf_connack_flag = mqtt_buf_new(
			sizeof(mqtt_attr_connack_flag_t));
	buf_connack_flag->buf[0] = p_attr_packet->attr_packet.connack.flag.all;
	remaining_length++;
	mqtt_buffer_array[i++] = buf_connack_flag;

	//< fixed header
	//< remaining length
	MQTT_CTL_REMAINING_LEN_CHECK(remaining_length);
	struct mqtt_buf_re_len * p_buf_re_len = mqtt_ctl_encode_remaining_len(
			remaining_length);
	mqtt_buffer_array[i++] = p_buf_re_len;
	//< header
	union mqtt_attr_ctl_flag ctl_flag = {
		.bits = {
			.type   = MQTT_CTL_TYPE_CONNACK,
			.DUP    = 0,
			.QoS    = 0,
			.RETAIN = 0
		}
	};
	struct mqtt_buf_ctl_flag * p_buf_ctl_flag = mqtt_ctl_flag_pack(ctl_flag);
	mqtt_buffer_array[i] = p_buf_ctl_flag;

	//< |payload|var header|remaining_length_code|fixed header byte|
	size_t total_len = remaining_length +   //!< length of [payload + var header]
			   p_buf_re_len->len +   //!< length of [remaining_length]
			   p_buf_ctl_flag->len;    //!< length of [fixed header byte]

	//struct mqtt_buf_packet * p_buf_packet = mqtt_buf_new(total_len);
	*pp_buf_packet = mqtt_buf_new(total_len);

	//// copy buffers to mqtt packet then release buffers
	if(NULL != *pp_buf_packet){
		size_t offset = 0;
		for(; i>=0; i--){
			memcpy((*pp_buf_packet)->buf+offset,
					mqtt_buffer_array[i]->buf,
					mqtt_buffer_array[i]->len);
			offset += mqtt_buffer_array[i]->len;
			mqtt_buf_release(mqtt_buffer_array[i]);
		}
		return E_NONE;
	}else{
		printf("[err]:malloc fail!\n");
		return E_MEM_FAIL;
	}
	
}

int mqtt_unpack_connack(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		){
	//< create packet attributes
	*pp_attr_packet = mqtt_attr_packet_new(0);
	int offset = 0;

	//< unpack packet
	//< fixed header
	//< header
	(*pp_attr_packet)->hdr.all = p_buf_packet->buf[offset++];
	//< remaining length
	size_t len_bytes = 0;
	(*pp_attr_packet)->remaining_length = mqtt_ctl_decode_remaining_len(
			p_buf_packet->buf+offset, &len_bytes);
	offset += len_bytes;

	//< variable header
	//< connack flag
	(*pp_attr_packet)->attr_packet.connack.flag.all = p_buf_packet->buf[offset++];
	//< connect return code
	(*pp_attr_packet)->attr_packet.connack.ret_code = p_buf_packet->buf[offset];

	//< payload
	//
	return E_NONE;
}


mqtt_err_t mqtt_pack_publish(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_publish * p_packet_publish
		);


mqtt_err_t mqtt_unpack_publish(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_publish * p_packet_publish
		);


mqtt_err_t mqtt_pack_puback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_puback * p_packet_puback
		);


mqtt_err_t mqtt_unpack_puback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_puback * p_packet_puback
		);


mqtt_err_t mqtt_pack_pubrec(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);


mqtt_err_t mqtt_unpack_pubrec(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);


mqtt_err_t mqtt_pack_pubrel(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);


mqtt_err_t mqtt_unpack_pubrel(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);


mqtt_err_t mqtt_pack_pubcomp(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);


mqtt_err_t mqtt_unpacke_pubcomp(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);


mqtt_err_t mqtt_pack_subscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);

mqtt_err_t mqtt_unpack_subscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);


mqtt_err_t mqtt_pack_suback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_suback * p_packet_suback
		);

mqtt_err_t mqtt_unpack_suback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_suback * p_packet_suback
		);


mqtt_err_t mqtt_pack_unsubscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);

mqtt_err_t mqtt_unpack_unsubscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);


mqtt_err_t mqtt_pack_unsuback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsuback * p_packet_unsuback
		);

mqtt_err_t mqtt_unpack_unsuback(
		const struct mqtt_buf_packet p_packet,
		struct mqtt_buf_packet_unsuback * p_packet_unsuback
		);


mqtt_err_t mqtt_pack_pingreq(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pingreq * p_packet_pingreq
		);


mqtt_err_t mqtt_pack_pingresp(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pingresp * p_packet_pingresp
		);


mqtt_err_t mqtt_pack_disconnect(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_disconnect * p_packet_disconnect
		);


