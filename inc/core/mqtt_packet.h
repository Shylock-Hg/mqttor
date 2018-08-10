/*! \brief API to upper layer 
 *         - pack&unpack fixed header , variable header and payload
 * */


#ifndef _MQTT_PACKET_H_
#define _MQTT_PACKET_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <assert.h>

#include "mqtt_fixed_header.h"
#include "mqtt_var_header.h"
#include "mqtt_payload.h"



///! \defgroup mqtt_buf_packet mqtt pack & unpack packet
///  @{
/*
typedef struct mqtt_buf_packet {
	const uint8_t * packet;  //!< pointer to all packet data
	size_t length;  //!< length of the whole packet
} mqtt_buf_packet_t;
*/
#define mqtt_buf_packet mqtt_buf
typedef mqtt_buf_t mqtt_buf_packet_t;

typedef struct mqtt_attr_packet {
	//enum mqtt_ctl_type type;  //!< packet type
	//!< control header
	mqtt_attr_ctl_flag_t hdr;
	mqtt_attr_re_len_t remaining_length;

	union {
		//< connect packet
		struct {
			//!< fixed header
			
			//!< variable header
			union mqtt_attr_connect_flag flag;  //!< p_conn_flag pointer to conn_flag structure
			mqtt_attr_uint16_t keep_alive;  //!< keep_alive time limit to keep alive measured by seconds

			//!< payload
			//const mqtt_attr_str_t id_client;  //!< id_client client identifier string
			//const mqtt_attr_str_t w_topic;  //!< w_topic will topic string
			//const mqtt_attr_str_t w_msg;  //!< w_msg will message string
			//const mqtt_attr_str_t user;  //!< user_name user name string
			//const mqtt_attr_str_t pwd;  //!< pwd password string
		} connect;

		//< connack packet
		struct {
			//!< fixed header
			
			//!< variable header
			union mqtt_attr_connack_flag flag;  //!< p_connack_flag flags of connack
			enum mqtt_attr_connack_ret_code ret_code;  //!< ret_code return code of connect
			//!< payload
		
		} connack;

		//< publish packet
		struct {
			//!< fixed header
			//union mqtt_attr_ctl_flag flag;  //!< p_ctl_flag flag of mqtt packet control
			//!< variable header
			mqtt_attr_str_t topic_name;  //!< topic_name name of topic publish to
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
			//const mqtt_attr_str_t app_msg;  //!< app_msg application specify message
		} publish;

		//< puback packet
		struct {
			//!< fixed header
			
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
		} puback;

		//< pubrec
		struct {
			//!< fixed header
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
		} pubrec;

		//< pubrel
		struct {
			//!< fixed header
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
		} pubrel;

		//< pubcomp
		struct {
			//!< fixed header
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
		} pubcomp;

		//< subscribe
		struct {
			//!< fixed header
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
			//struct mqtt_payload_subscribe_content * sub_content;  //!< sub_content content array to subscribe
			//uint16_t sub_count;  //!< sub_count count of subscribe
		} subscribe;

		//< packet suback
		struct {
			//!< fixed header
			//!< variable header
			mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
			//struct mqtt_payload_suback_flag * suback_flag;  //!< suback_flag[] flags of suback array
			//uint16_t suback_flag_count;  //!< suback_flag_count count of suback
		} suback;

		//< packet unsubscribe
		struct {
			//!< fixed header
			//!< variable header
			uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
			//const mqtt_attr_str_t * top_filter;  //!< top_filter[] array of top_filter string
			//uint16_t top_filter_count;  //!< to_filter_count count of top_filter string
		} unsubscribe;

		//< packet unsuback
		struct {
			//!< fixed header
			//!< variable header
			uint16_t id_packet;  //!< id_packet identifier of packet
			//!< payload
		} unsuback;

	} attr_packet;

	//!< payload
	mqtt_attr_payload_t * payload;
} mqtt_attr_packet_t;

/*! \brief create a mqtt packet attribute
 *  \param len_payload lenght of payload buffer
 *  \retval pointer to mqtt attribute packet
 * */
mqtt_attr_packet_t * mqtt_attr_packet_new(size_t len_payload);
/*! \brief release a mqtt attribute packet
 *  \param pointer to mqtt attribute packet
 * */
void mqtt_attr_packet_release(mqtt_attr_packet_t * packet);


///! \defgroup mqtt_buf_packet_connect 
/// @{
/*
typedef struct mqtt_attr_packet_connect {
	//!< fixed header
	
	//!< variable header
	union mqtt_attr_connect_flag flag;  //!< p_conn_flag pointer to conn_flag structure
	mqtt_attr_uint16_t keep_alive;  //!< keep_alive time limit to keep alive measured by seconds

	//!< payload
	const mqtt_attr_str_t id_client;  //!< id_client client identifier string
	const mqtt_attr_str_t w_topic;  //!< w_topic will topic string
	const mqtt_attr_str_t w_msg;  //!< w_msg will message string
	const mqtt_attr_str_t user;  //!< user_name user name string
	const mqtt_attr_str_t pwd;  //!< pwd password string
} mqtt_attr_packet_connect_t;
*/

/*! \brief pack connect packet 
 *  \param p_attr_packet[in]  mqtt packet
 *  \param p_buf_packet[out] pointer to connect packect 
 *  \retval mqtt error
 * */
int mqtt_pack_connect(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack connect packet
 *  \param p_buf_packet[in] pointer to mqtt packet received
 *  \param p_attr_packet[out] pointer to mqtt connect packet 
 *  \retval mqtt error
 * */
int mqtt_unpack_connect(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);
/// @}


///! \defgroup mqtt_buf_packet_connack
/// @{
/*
typedef struct mqtt_attr_packet_connack {
	//!< fixed header
	
	//!< variable header
	union mqtt_attr_connack_flag flag;  //!< p_connack_flag flags of connack
	enum mqtt_connect_ret_code ret_code;  //!< ret_code return code of connect
	//!< payload
	
} mqtt_attr_packet_connack_t;
*/

/*! \brief pack mqtt connack packet
 *  \param p_attr_packet[in] pointer to mqtt packet
 *  \param pp_buf_packet[out] pointer to mqtt connack structure
 *  \retval mqtt error
 * */
int mqtt_pack_connack(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_connack
		);

/*! \brief unpack mqtt connack packet
 *  \param p_packet[in] mqtt packet pointer
 *  \param p_packet_connack[out] pointer to mqtt connack structure
 *  \retval mqtt_err_t
 * */
int mqtt_unpack_connack(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);
/// @}


///! \defgroup mqtt_buf_packet_publish
/// @{
/*
typedef struct mqtt_buf_packet_publish {
	//!< fixed header
	union mqtt_attr_ctl_flag flag;  //!< p_ctl_flag flag of mqtt packet control
	//!< variable header
	const mqtt_attr_str_t topic_name;  //!< topic_name name of topic publish to
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	const mqtt_attr_str_t app_msg;  //!< app_msg application specify message
} mqtt_buf_packet_publish_t;
*/

/*! \brief pack mqtt publish packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_publish(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt publish packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt publish attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_publish(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);
/// @}


///! \defgroup mqtt_buf_packet_puback
/// @{
/*
typedef struct mqtt_buf_packet_puback {
	//!< fixed header
	
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_puback_t;
*/

/*! \brief pack mqtt puback packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_puback(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt puback packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt puback attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_puback(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_pubrec
/// @{
/*
typedef struct mqtt_buf_packet_pubrec {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrec_t;
*/

/*! \brief pack mqtt pubrec packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_pubrec(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt pubrec packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt pubrec attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_pubrec(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_pubrel
/// @{
/*
typedef struct mqtt_buf_packet_pubrel {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrel_t;
*/

/*! \brief pack mqtt pubrel packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_pubrel(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt pubrel packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt pubrel attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_pubrel(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_pubcomp
/// @{
/*
typedef struct mqtt_buf_packet_pubcomp {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubcomp_t;
*/

/*! \brief pack mqtt pubcomp packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_pubcomp(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt pubcomp packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt pubcomp attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_pubcomp(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_subscribe
/// @{
/*
typedef struct mqtt_buf_packet_subscribe {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_subscribe_content * sub_content;  //!< sub_content content array to subscribe
	uint16_t sub_count;  //!< sub_count count of subscribe
} mqtt_buf_packet_subscribe_t;
*/

/*! \brief pack mqtt subscribe packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_subscribe(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt subscribe packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt subscribe attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_subscribe(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_suback
/// @{
/*
typedef struct mqtt_buf_packet_suback {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_suback_flag * suback_flag;  //!< suback_flag[] flags of suback array
	uint16_t suback_flag_count;  //!< suback_flag_count count of suback
} mqtt_buf_packet_suback_t;
*/

/*! \brief pack mqtt suback packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_suback(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt suback packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt suback attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_suback(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_unsubscribe
/// @{
/*
typedef struct mqtt_buf_packet_unsubscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	const mqtt_attr_str_t * top_filter;  //!< top_filter[] array of top_filter string
	uint16_t top_filter_count;  //!< to_filter_count count of top_filter string
} mqtt_buf_packet_unsubscribe_t;
*/

/*! \brief pack mqtt unsubscribe packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_unsubscribe(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt unsubscribe packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt unsubscribe attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_unsubscribe(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_unsuback
/// @{
/*
typedef struct mqtt_buf_packet_unsuback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_unsuback_t;
*/

/*! \brief pack mqtt unsuback packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_unsuback(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt unsuback packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt unsuback attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_unsuback(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);
/// @}


///! \defgroup mqtt_buf_packet_pingreq
///  @{
/*
typedef struct mqtt_buf_packet_pingreq {
	//!< fixed header
	//!< variable header
	//!< payload
	void;
} mqtt_buf_packet_pingreq_t;
*/

/*! \brief pack mqtt pingreq packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_pingreq(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt pingreq packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt pingreq attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_pingreq(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}


///! \defgroup mqtt_buf_packet_pingresp
///  @{
/*
typedef struct mqtt_buf_packet_pingresp {
	//!< fixed header
	//!< variable header
	//!< payload
	void;
} mqtt_buf_packet_pingresp_t;
*/

/*! \brief pack mqtt pingresp packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_pingresp(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt pingresp packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt pingresp attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_pingresp(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);
/// @}


///! \defgroup mqtt_buf_packet_disconnect
///  @{
/*
typedef struct mqtt_buf_packet_disconnect {
	//!< fixed header
	//!< variable header
	//!< payload
	void;
} mqtt_buf_packet_disconnect_t;
*/

/*! \brief pack mqtt disconnect packet
 *  \param p_attr_packet[in] pointer to mqtt packet attributes
 *  \param pp_buf_packet[out] pointer to mqtt packet buffer
 *  \retval mqtt error
 * */
int mqtt_pack_disconnect(
		const mqtt_attr_packet_t * p_attr_packet,
		mqtt_buf_packet_t ** pp_buf_packet
		);

/*! \brief unpack mqtt disconnect packet 
 *  \param p_buf_packet[in] pointer to mqtt packet buffer
 *  \param p_attr_packet[out] pointer to mqtt disconnect attributes
 *  \retval mqtt error
 * */
int mqtt_unpack_disconnect(
		const mqtt_buf_packet_t * p_buf_packet,
		mqtt_attr_packet_t ** pp_attr_packet
		);

/// @}

/// @}


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

