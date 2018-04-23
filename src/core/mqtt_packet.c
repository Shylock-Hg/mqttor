/*! \file mqtt_buf_packet.c
 *  \brief mqtt packet pack&unpack functions
 *  \author Shylock Hg
 *  \date 2018-04-10
 * */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

//#include "../../inc/core/mqtt_fixed_header.h"
//#include "../../inc/core/mqtt_var_header.h"
//#include "../../inc/core/mqtt_payload.h"

//#include "../../inc/core/mqtt_buf_packet_segment.h"
#include "../../inc/core/mqtt_packet.h"

///! \defgroup mqtt_err mqtt error definition
///  @{
/*
typedef enum mqtt_err {
	E_NONE         = 0,  //!< error : none error
	E_FORMAT_CHECK = -1,  //!< error format : flag value check fail
	E_MEM_FAIL     = -1,
} mqtt_err_t;
*/

static const char * errstr[] = {
	"E_NONE",
	"E_FORMAT_CHECK",
	"E_MEM_FAIL"
};

const char * mqtt_str_error(mqtt_err_t err){
	return errstr[-err];
}
/// @}


///! \defgroup mqtt_buf_packet mqtt pack & unpack packet
///  @{
/*
typedef struct mqtt_buf_packet {
	const uint8_t * packet;  //!< pointer to all packet data
	size_t length;  //!< length of the whole packet
} mqtt_buf_packet_t;
*/


///! \defgroup mqtt_buf_packet_connect 
/// @{
/*
typedef struct mqtt_buf_packet_connect {
	//!< fixed header
	
	//!< variable header
	struct mqtt_connect_flag p_conn_flag,  //!< p_conn_flag pointer to conn_flag structure
	uint16_t keep_alive,  //!< keep_alive time limit to keep alive measured by seconds

	//!< payload
	const char * id_client,  //!< id_client client identifier string
	const char * w_topic,  //!< w_topic will topic string
	const char * w_msg,  //!< w_msg will message string
	const char * user,  //!< user_name user name string
	const char * pwd  //!< pwd password string
} mqtt_buf_packet_connect_t;
*/

#define MQTT_PACK_CONNECT_VAR_LEN 10

mqtt_err_t mqtt_pack_connect(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_connect * p_packet_connect
		){
	uint32_t remaining_length = 0;
	/// payload
	/// password
	size_t pwd_len = strlen(p_packet->pwd);
	MQTT_PACKET_STR_LEN_CHECK(pwd_len);
	remaining_length += (pwd_len + MQTT_PACKET_STR_MAX_BYTE);
	/// user
	size_t user_len = strlen(p_packet->user);
	MQTT_PACKET_STR_LEN_CHECK(user_len);
	remaining_length += (user_len + MQTT_PACKET_STR_MAX_BYTE);
	/// w_msg
	size_t w_msg_len = strlen(p_packet->w_msg);
	MQTT_PACKET_STR_LEN_CHECK(w_msg_len);
	remaining_length += (w_msg_len + MQTT_PACKET_STR_MAX_BYTE);
	/// w_topic
	size_t w_topic_len = strlen(p_pakcet->w_topic);
	MQTT_PACKET_STR_LEN_CHECK(w_topic_len);
	remaining_length += (w_topic_len + MQTT_PACKET_STR_MAX_BYTE);
	/// id_client
	size_t id_client_len = strlen(p_pakcet->w_client);
	MQTT_PACKET_STR_LEN_CHECK(id_client_len);
	remaining_length += (id_client_len + MQTT_PACKET_STR_MAX_BYTE);

	/// variable header
	remaining_length += MQTT_PACK_CONNECT_VAR_LEN;  //!< add varibale header length

	/// fixed header
	MQTT_CTL_REMAINING_LEN_CHECK(remaining_length);
	uint8_t remaining_length_code[MQTT_CTL_REMAINING_MAX_LEN_BYTE] = {0};
	int remaining_length_code_size = mqtt_ctl_encode_remaining_len(remaining_length_code,remaining_length);
	uint8_t fixed_flags_byte = (MQTT_CTL_TYPE_CONNECT<<MQTT_CTL_TYPE_OFFSET) | \
				   0x00;  //!< |TYPE|RESERVED FLAGS|

	//< |payload|var header|remaining_length_code|fixed header byte|
	size_t total_len = remaining_length + \  //!< length of [payload + var header]
			   remaining_length_code_size + \  //!< length of [remaining_length]
			   MQTT_CTL_FLAG_MAX_LEN_BYTE;    //!< length of [fixed header byte]

	uint8_t * packet_data = malloc(total_len);

	if(NULL != packet_data){
		size_t offset = 0;
		//< fixed header
		//< fixed header first byte
		memcpy(packet_data+offset,&fixed_flags_byte,sizeof(fixed_flags_byte));
		offset += sizeof(fixed_flags_byte);
		//< remaining length code
		memcpy(packet_data+offset,remaining_length_code,remaining_length_code_size);
		offset += remaining_length_code_size;

		//< var header
		//< protocol name string
		struct mqtt_str * mq_str = mqtt_buf_packet_str_encode(MQTT_PROTOCOL_NAME);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
		//< protocol level 4
		packet_data[++offset] = 0x04;
		//< connect flags
		packet_data[++offset] = mqtt_connect_flag_pack_s(p_packet->p_conn_flag);

		//< keep alive
		uint8_t kp_alive[sizeof(p_packet->keep_alive)] = {0};
		UINT16_2_BYTES(p_packet->keep_alive,kp_alive);
		memcpy(packet_data+offset,kp_alive,sizeof(kp_alive));
		offset += sizeof(kp_alive);

		//< payload
		//< client identifier mqtt-string
		mq_str = mqtt_buf_packet_str_encode(p_packet->id_client);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
		//< will topic mqtt-string
		mq_str = mqtt_buf_packet_str_encode(p_packet->w_topic);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
		//< will message mqtt-string
		mq_str = mqtt_buf_packet_str_encode(p_packet->w_msg);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
		//< user name mqtt-string
		mq_str = mqtt_buf_packet_str_encode(p_packet->user);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
		//< password mqtt-string
		mq_str = mqtt_buf_packet_str_encode(p_packet->user);
		memcpy(packet_data+offset,mq_str->code,mq_str->len);
		offset += mq_str->len;
		free(mq_str->code);
		free(mq_str);
	}else{
		printf("[err]:malloc fail!\n");
		return E_MEM_FAIL;
	}
}

mqtt_err_t mqtt_unpack_connect(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_connect * p_packet_connect
		){
	//< fixed header
	
}
/// @}


///! \defgroup mqtt_buf_packet_connack
/// @{
struct mqtt_buf_packet_connack {
	//!< fixed header
	
	//!< variable header
	const struct mqtt_connack_flag p_connack_flag,  //!< p_connack_flag flags of connack
	enum mqtt_conn_ret_code ret_code  //!< ret_code return code of connect
	//!< payload
	
} mqtt_buf_packet_connack_t;


mqtt_err_t mqtt_pack_connack(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_connack * p_packet_connack
		);


mqtt_err_t mqtt_unpack_connack(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_connack * p_packet_connack
		);
/// @}


///! \defgroup mqtt_buf_packet_publish
/// @{
struct mqtt_buf_packet_publish {
	//!< fixed header
	const struct mqtt_ctl_head p_ctl_flag,  //!< p_ctl_flag flag of mqtt packet control
	//!< variable header
	const char * topic_name,  //!< topic_name name of topic publish to
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * app_msg  //!< app_msg application specify message
} mqtt_buf_packet_publish_t;


 mqtt_pack_publish(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_publish * p_packet_publish
		);


mqtt_err_t mqtt_unpack_publish(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_publish * p_packet_publish
		);
/// @}


///! \defgroup mqtt_buf_packet_puback
/// @{
struct mqtt_buf_packet_puback {
	//!< fixed header
	
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_puback_t;


mqtt_err_t mqtt_pack_puback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_puback * p_packet_puback
		);


mqtt_err_t mqtt_unpack_puback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_puback * p_packet_puback
		);
/// @}


///! \defgroup mqtt_buf_packet_pubrec
/// @{
struct mqtt_buf_packet_pubrec {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrec_t;


mqtt_err_t mqtt_pack_pubrec(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);


mqtt_err_t mqtt_unpack_pubrec(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);
/// @}


///! \defgroup mqtt_buf_packet_pubrel
/// @{
struct mqtt_buf_packet_pubrel {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrel_t;


mqtt_err_t mqtt_pack_pubrel(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);


mqtt_err_t mqtt_unpack_pubrel(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);
/// @}


///! \defgroup mqtt_buf_packet_pubcomp
/// @{
struct mqtt_buf_packet_pubcomp {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubcomp_t;


mqtt_err_t mqtt_pack_pubcomp(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);


mqtt_err_t mqtt_unpacke_pubcomp(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);
/// @}


///! \defgroup mqtt_buf_packet_subscribe
/// @{
struct mqtt_buf_packet_subscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_subscribe_content sub_content[],  //!< sub_content content array to subscribe
	uint16_t sub_count  //!< sub_count count of subscribe
} mqtt_buf_packet_subscribe_t;


mqtt_err_t mqtt_pack_subscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);


mqtt_err_t mqtt_unpack_subscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);
/// @}


///! \defgroup mqtt_buf_packet_suback
/// @{
struct mqtt_buf_packet_suback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_suback_flag suback_flag[],  //!< suback_flag[] flags of suback array
	uint16_t suback_flag_count  //!< suback_flag_count count of suback
} mqtt_buf_packet_suback_t;


mqtt_err_t mqtt_pack_suback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_suback * p_packet_suback
		);


mqtt_err_t mqtt_unpack_suback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_suback * p_packet_suback
		);
/// @}


///! \defgroup mqtt_buf_packet_unsubscribe
/// @{
struct mqtt_buf_packet_unsubscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * top_filter[],  //!< top_filter[] array of top_filter string
	uint16_t top_filter_count  //!< to_filter_count count of top_filter string
} mqtt_buf_packet_unsubscribe_t;


mqtt_err_t mqtt_pack_unsubscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);


mqtt_err_t mqtt_unpack_unsubscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);
/// @}


///! \defgroup mqtt_buf_packet_unsuback
/// @{
struct mqtt_buf_packet_unsuback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_unsuback_t;


mqtt_err_t mqtt_pack_unsuback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsuback * p_packet_unsuback
		);


mqtt_err_t mqtt_unpack_unsuback(
		const struct mqtt_buf_packet p_packet,
		struct mqtt_buf_packet_unsuback * p_packet_unsuback
		);
/// @}


///! \defgroup mqtt_buf_packet_pingreq
///  @{
typedef struct mqtt_buf_packet_pingreq {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_buf_packet_pingreq_t;


mqtt_err_t mqtt_pack_pingreq(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pingreq * p_packet_pingreq
		);
/// @}


///! \defgroup mqtt_buf_packet_pingresp
///  @{
typedef struct mqtt_buf_packet_pingresp {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_buf_packet_pingresp_t;


mqtt_err_t mqtt_pack_pingresp(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pingresp * p_packet_pingresp
		);
/// @}


///! \defgroup mqtt_buf_packet_disconnect
///  @{
typedef struct mqtt_buf_packet_disconnect {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_buf_packet_disconnect_t;


mqtt_err_t mqtt_pack_disconnect(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_disconnect * p_packet_disconnect
		);
/// @}

/// @}


