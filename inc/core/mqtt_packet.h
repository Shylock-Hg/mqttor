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
			const mqtt_attr_str_t topic_name;  //!< topic_name name of topic publish to
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
typedef struct mqtt_buf_packet_publish {
	//!< fixed header
	union mqtt_attr_ctl_flag flag;  //!< p_ctl_flag flag of mqtt packet control
	//!< variable header
	const mqtt_attr_str_t topic_name;  //!< topic_name name of topic publish to
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	const mqtt_attr_str_t app_msg;  //!< app_msg application specify message
} mqtt_buf_packet_publish_t;

/*! \brief pack mqtt publish packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_publish[in] pointer to publish structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_publish(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_publish * p_packet_publish
		);

/*! \brief unpack mqtt publish packet 
 *  \param p_packet[in] pointer to mqtt packet 
 *  \param p_packet_publish[out] pointer to mqtt publish structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_publish(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_publish * p_packet_publish
		);
/// @}


///! \defgroup mqtt_buf_packet_puback
/// @{
typedef struct mqtt_buf_packet_puback {
	//!< fixed header
	
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_puback_t;

/*! \brief pack mqtt publish acknowledge packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_puback[in] pointer to mqtt puback structure
 *  \retval pointer to mqtt packet
 * */
mqtt_err_t mqtt_pack_puback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_puback * p_packet_puback
		);

/*! \brief unpack matt puback packet 
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_puback[out] pointer to mqtt puback packet
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_puback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_puback * p_packet_puback
		);
/// @}


///! \defgroup mqtt_buf_packet_pubrec
/// @{
typedef struct mqtt_buf_packet_pubrec {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrec_t;

/*! \brief pack mqtt publish received packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubrec[in] pointer to mqtt pubrec structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubrec(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);

/*! \brief unpack mqtt pubrec packet 
 *  \param p_packet[in] pointer to mqtt pubrec packet
 *  \param p_packet_pubrec[out] pointer to mqtt pubrec structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_pubrec(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrec * p_packet_pubrec
		);
/// @}


///! \defgroup mqtt_buf_packet_pubrel
/// @{
typedef struct mqtt_buf_packet_pubrel {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubrel_t;

/*! \brief pack mqtt publish release packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubrel[in] pointer to mqtt pubrel structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubrel(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);

/*! \brief unpack mqtt pubrel packet
 *  \param p_packet_pubrel[in] pointer to mqtt pubrel structure
 *  \param p_packet[out] pointer to mqtt packet
 *  \retval 
 * */
mqtt_err_t mqtt_unpack_pubrel(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubrel * p_packet_pubrel
		);
/// @}


///! \defgroup mqtt_buf_packet_pubcomp
/// @{
typedef struct mqtt_buf_packet_pubcomp {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_pubcomp_t;

/*! \brief pack mqtt publish complete packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubcomp[in] pointer to mqtt pubcomp structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubcomp(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);

/*! \brief unpack mqtt pubcomp packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_pubcomp[out] pointer to pubcomp structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpacke_pubcomp(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_pubcomp * p_packet_pubcomp
		);
/// @}


///! \defgroup mqtt_buf_packet_subscribe
/// @{
typedef struct mqtt_buf_packet_subscribe {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_subscribe_content * sub_content;  //!< sub_content content array to subscribe
	uint16_t sub_count;  //!< sub_count count of subscribe
} mqtt_buf_packet_subscribe_t;

/*! \brief pack mqtt subscribe packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_subscribe[in] pointer to mqtt subscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_subscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);

/*! \brief unpack mqtt subscribe packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_subscribe[out] pointer to mqtt subscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_subscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_subscribe * p_packet_subscribe
		);
/// @}


///! \defgroup mqtt_buf_packet_suback
/// @{
typedef struct mqtt_buf_packet_suback {
	//!< fixed header
	//!< variable header
	mqtt_attr_uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_suback_flag * suback_flag;  //!< suback_flag[] flags of suback array
	uint16_t suback_flag_count;  //!< suback_flag_count count of suback
} mqtt_buf_packet_suback_t;

/*! \brief pack mqtt suback packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_suback[in] pointer to mqtt suback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_suback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_suback * p_packet_suback
		);

/*! \brief unpack mqtt suback packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_suback[in] pointer to mqtt suback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_suback(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_suback * p_packet_suback
		);
/// @}


///! \defgroup mqtt_buf_packet_unsubscribe
/// @{
typedef struct mqtt_buf_packet_unsubscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
	const mqtt_attr_str_t * top_filter;  //!< top_filter[] array of top_filter string
	uint16_t top_filter_count;  //!< to_filter_count count of top_filter string
} mqtt_buf_packet_unsubscribe_t;

/*! \brief pack mqtt unsubscribe packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_unsubscribe[in] pointer to mqtt unsubscribe structure
 *  \retval 
 * */
mqtt_err_t mqtt_pack_unsubscribe(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);

/*! \brief unpack mqtt unsubscribe packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_unsubscribe[out] pointer to mqtt unsubscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_unsubscribe(
		const struct mqtt_buf_packet * p_packet,
		struct mqtt_buf_packet_unsubscribe * p_packet_unsubscribe
		);
/// @}


///! \defgroup mqtt_buf_packet_unsuback
/// @{
typedef struct mqtt_buf_packet_unsuback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet;  //!< id_packet identifier of packet
	//!< payload
} mqtt_buf_packet_unsuback_t;

/*! \brief pack mqtt unsuback packet
 *  \param p_packet[out] pointer to mqtt unsuback packet
 *  \param p_packet_unsuback[in] pointer to mqtt unsuback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_unsuback(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_unsuback * p_packet_unsuback
		);

/*! \brief unpack mqtt unsuback packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_unsuback[out] pointer to mqtt unsuback structure
 *  \retval mqtt_err_t
 * */
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
	void;
} mqtt_buf_packet_pingreq_t;

/*! \brief pack mqtt pingreq packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pingreq[in] pointer to mqtt pingreq structure
 *  \retval mqtt_err_t
 * */
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
	void;
} mqtt_buf_packet_pingresp_t;

/*! \brief pack mqtt pingresp packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pingresp[in] pointer to mqtt pingresp structure
 *  \retval mqtt_err_t
 * */
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
	void;
} mqtt_buf_packet_disconnect_t;

/*! \brief pack mqtt disconnect packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_disconnect[in] pointer to mqtt disconnect structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_disconnect(
		struct mqtt_buf_packet * p_packet,
		const struct mqtt_buf_packet_disconnect * p_packet_disconnect
		);
/// @}

/// @}


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

