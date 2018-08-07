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


static const char * errstr[] = {
	"E_NONE",
	"E_FORMAT_CHECK",
	"E_MEM_FAIL",
	"E_MEM_OUT"
};

const char * mqtt_str_error(mqtt_err_t err){
	return errstr[err];
}

mqtt_attr_packet_t * mqtt_attr_packet_new(size_t len_payload){
	assert(len_payload);

	mqtt_attr_packet_t * packet = malloc(sizeof(mqtt_attr_packet_t));
	assert(packet);
	packet->payload = mqtt_attr_payload_new(len_payload);
	assert(packet->payload);

	return packet;
}

void mqtt_attr_packet_release(mqtt_attr_packet_t * packet){
	assert(packet);

	mqtt_attr_payload_release(packet->payload);
	free(packet);
}

//!< matt packet payload writer
int mqtt_packet_payload_write_string(mqtt_attr_packet_t * packet, 
		mqtt_attr_str_t string){
	assert(packet);
	assert(string);

	mqtt_attr_uint16_t len = strlen(string);
	if(len + MQTT_BUF_STR_MAX_BYTE + packet->payload->len_valid > 
			packet->payload->len){
		return -E_MEM_OUT;
	}
	mqtt_buf_uint16_t * buf_uint16 = mqtt_buf_uint16_encode(len);
	//mqtt_log_print_buf(buf_uint16->buf, buf_uint16->len);
	memcpy(packet->payload->buf+packet->payload->len_valid, buf_uint16->buf, 
			buf_uint16->len);
	packet->payload->len_valid += buf_uint16->len;
	//mqtt_log_print_buf(packet->payload->buf, packet->payload->len_valid);
	mqtt_buf_release(buf_uint16);

	strcpy((char*)packet->payload->buf+packet->payload->len_valid, string);
	packet->payload->len_valid += len;

	return len+MQTT_BUF_STR_MAX_BYTE;
}

int mqtt_packet_payload_write_byte(mqtt_attr_packet_t * packet, uint8_t byte){
	assert(packet);

	if(packet->payload->len == packet->payload->len_valid){
		return -E_MEM_OUT;
	}

	packet->payload->buf[packet->payload->len_valid++] = byte;
	
	return sizeof(byte);
}

int mqtt_packet_payload_write_bytes(mqtt_attr_packet_t * packet, uint8_t * bytes,
		size_t len){
	assert(packet);
	assert(bytes);

	if(len + packet->payload->len_valid > packet->payload->len){
		return -E_MEM_OUT;
	}

	memcpy(packet->payload->buf+packet->payload->len_valid, bytes, len);
	packet->payload->len_valid += len;

	return len;
}

//#define MQTT_PACK_CONNECT_VAR_LEN 10

struct mqtt_buf_packet * mqtt_pack_connect(
		const struct mqtt_attr_packet * p_attr_packet
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
	mqtt_buf_t * p_buf_payload = mqtt_attr_payload_2_buf(p_attr_packet->payload);
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

	struct mqtt_buf_packet * p_buf_packet = mqtt_buf_new(total_len);

	//// copy buffers to mqtt packet then release buffers
	if(NULL != p_buf_packet){
		size_t offset = 0;
		for(; i>=0; i--){
			memcpy(p_buf_packet->buf+offset,mqtt_buffer_array[i]->buf,mqtt_buffer_array[i]->len);
			offset += mqtt_buffer_array[i]->len;
			mqtt_buf_release(mqtt_buffer_array[i]);
		}
		return p_buf_packet;
	}else{
		printf("[err]:malloc fail!\n");
		return NULL;
	}
}

struct mqtt_attr_packet * mqtt_unpack_connect(
		const struct mqtt_buf_packet * p_packet
		){
	/*
	mqtt_attr_re_len_t remaining_length = 0;
	struct mqtt_buf * mqtt_buffer_array[20] = {NULL};
	int i = 0;
	/// payload
	/// password
	struct mqtt_buf_str * p_buf_password = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.pwd);
	mqtt_buffer_array[i++] = p_buf_password;
	remaining_length += p_buf_password->len;
	/// user
	struct mqtt_buf_str * p_buf_user = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.user);
	mqtt_buffer_array[i++] = p_buf_user;
	remaining_length += p_buf_user->len;
	/// w_msg
	struct mqtt_buf_str * p_buf_w_msg = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.w_msg);
	mqtt_buffer_array[i++] = p_buf_w_msg;
	remaining_length += p_buf_w_msg->len;
	/// w_topic
	struct mqtt_buf_str * p_buf_w_topic = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.w_topic);
	mqtt_buffer_array[i++] = p_buf_w_topic;
	remaining_length += p_buf_w_topic->len;
	/// id_client
	struct mqtt_buf_str * p_buf_id_client = mqtt_buf_str_encode(p_attr_packet->attr_packet.connect.id_client);
	mqtt_buffer_array[i++] = p_buf_id_client;
	remaining_length += p_buf_id_client->len;

	/// variable header
	/// protocol name "MQTT"
	struct mqtt_buf_str * p_buf_mqtt_name = mqtt_buf_str_encode(MQTT_PROTOCOL_NAME);
	mqtt_buffer_array[i++] = p_buf_mqtt_name;
	remaining_length += p_buf_mqtt_name->len;  //!< add varibale header length
	/// level 4
	struct mqtt_buf * p_buf_level = mqtt_buf_new(sizeof(uint8_t));
	p_buf_level->buf[0] = 4;
	mqtt_buffer_array[i++] = p_buf_level;
	remaining_length += p_buf_level->len;
	/// connect flags
	struct mqtt_buf_connect_flag * p_buf_connect_flag = mqtt_connect_flag_pack(p_attr_packet->attr_packet.connect.flag);
	mqtt_buffer_array[i++] = p_buf_connect_flag;
	remaining_length += p_buf_connect_flag->len;
	/// keep alive
	struct mqtt_buf_uint16 * p_buf_keep_alive = mqtt_buf_uint16_encode(p_attr_packet->attr_packet.connect.keep_alive);
	mqtt_buffer_array[i++] = p_buf_keep_alive;
	remaining_length += p_buf_keep_alive->len;

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
	mqtt_buffer_array[i++] = p_buf_ctl_flag;

	//< |payload|var header|remaining_length_code|fixed header byte|
	size_t total_len = remaining_length +   //!< length of [payload + var header]
			   p_buf_re_len->len +   //!< length of [remaining_length]
			   p_buf_ctl_flag->len;    //!< length of [fixed header byte]

	//struct mqtt_buf_packet * p_buf_packet = mqtt_buf_new(total_len);
	*/

	struct mqtt_attr_packet * p_attr_packet = malloc(sizeof(struct mqtt_attr_packet));

	uint8_t * buf = p_packet->buf;  //!< temp pointer to buffer
	//size_t offset = 0;  //!< offset of packet pointer

	//< fixed header
	///< control flag
	union mqtt_attr_ctl_flag attr_ctl_flag = mqtt_ctl_flag_unpack(&buf);
	///< remaining length

}


struct mqtt_buf_packet_connack * mqtt_pack_connack(
		const struct mqtt_attr_packet_connack * p_packet_connack
		);

struct mqtt_attr_packet_connack * mqtt_unpack_connack(
		const struct mqtt_buf_packet * p_packet
		);


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


