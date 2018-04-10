/*! \brief API to upper layer 
 *         - pack&unpack fixed header , variable header and payload
 * */

#ifndef _MQTT_PACKET_H_
#define _MQTT_PACKET_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "mqtt_fixed_header.h"
#include "mqtt_var_header.h"
#include "mqtt_payload.h"

#include <assert.h>

///! \defgroup mqtt_err mqtt error definition
///  @{
typedef enum mqtt_err {
	E_NONE         = 0,  //!< error : none error
	E_FORMAT_CHECK = -1,  //!< error format : flag value check fail
} mqtt_err_t;

/*! \brief convert err to coressponding prompt string
 *  \param err error index
 *  \retval pointer to error prompt string
 * */
const char * mqtt_str_error(mqtt_err_t err);
/// @}


///! \defgroup mqtt_packet mqtt pack & unpack packet
///  @{

typedef struct mqtt_packet {
	const uint8_t * packet;  //!< pointer to all packet data
	size_t length;  //!< length of the whole packet
} mqtt_packet_t;


///! \defgroup mqtt_packet_connect 
/// @{
typedef struct mqtt_packet_connect {
	//!< fixed header
	
	//!< variable header
	struct mqtt_connect_flag p_conn_flag,  //!< p_conn_flag pointer to conn_flag structure
	uint16_t keep_alive,  //!< keep_alive time limit to keep alive measured by seconds

	//!< payload
	const char * id_client,  //!< id_client client identifier string
	const char * w_topic,  //!< w_topic will topic string
	const char * w_msg,  //!< w_msg will message string
	const char * user_name,  //!< user_name user name string
	const char * pwd  //!< pwd password string
} mqtt_packet_connect_t;

/*! \brief pack connect packet 
 *  \param p_packet[out]  mqtt packet
 *  \param p_packet_connect[in] pointer to connect packect 
 *  \retval mqtt_err_t 
 * */
mqtt_err_t mqtt_pack_connect(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_connect * p_packet_connect
		);

/*! \brief unpack connect packet
 *  \param p_packet[in] pointer to mqtt packet received
 *  \pram p_packet_connect[out] pointer to mqtt connect packet 
 *  \retval pointer to mqtt connect structure
 * */
mqtt_err_t mqtt_unpack_connect(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_connect * p_packet_connect
		);
/// @}


///! \defgroup mqtt_packet_connack
/// @{
struct mqtt_packet_connack {
	//!< fixed header
	
	//!< variable header
	const struct mqtt_connack_flag p_connack_flag,  //!< p_connack_flag flags of connack
	enum mqtt_conn_ret_code ret_code  //!< ret_code return code of connect
	//!< payload
	
} mqtt_packet_connack_t;

/*! \brief pack mqtt connack packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_connack[in] pointer to mqtt connack structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_connack(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_connack * p_packet_connack
		);

/*! \brief unpack mqtt connack packet
 *  \param p_packet[in] mqtt packet pointer
 *  \param p_packet_connack[out] pointer to mqtt connack structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_connack(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_connack * p_packet_connack
		);
/// @}


///! \defgroup mqtt_packet_publish
/// @{
struct mqtt_packet_publish {
	//!< fixed header
	const struct mqtt_ctl_head p_ctl_flag,  //!< p_ctl_flag flag of mqtt packet control
	//!< variable header
	const char * topic_name,  //!< topic_name name of topic publish to
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * app_msg  //!< app_msg application specify message
} mqtt_packet_publish_t;

/*! \brief pack mqtt publish packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_publish[in] pointer to publish structure
 *  \retval mqtt_err_t
 * */
 mqtt_pack_publish(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_publish * p_packet_publish
		);

/*! \brief unpack mqtt publish packet 
 *  \param p_packet[in] pointer to mqtt packet 
 *  \param p_packet_publish[out] pointer to mqtt publish structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_publish(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_publish * p_packet_publish
		);
/// @}


///! \defgroup mqtt_packet_puback
/// @{
struct mqtt_packet_puback {
	//!< fixed header
	
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_puback_t;

/*! \brief pack mqtt publish acknowledge packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_puback[in] pointer to mqtt puback structure
 *  \retval pointer to mqtt packet
 * */
mqtt_err_t mqtt_pack_puback(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_puback * p_packet_puback
		);

/*! \brief unpack matt puback packet 
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_puback[out] pointer to mqtt puback packet
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_puback(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_puback * p_packet_puback
		);
/// @}


///! \defgroup mqtt_packet_pubrec
/// @{
struct mqtt_packet_pubrec {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubrec_t;

/*! \brief pack mqtt publish received packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubrec[in] pointer to mqtt pubrec structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubrec(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_pubrec * p_packet_pubrec
		);

/*! \brief unpack mqtt pubrec packet 
 *  \param p_packet[in] pointer to mqtt pubrec packet
 *  \param p_packet_pubrec[out] pointer to mqtt pubrec structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_pubrec(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_pubrec * p_packet_pubrec
		);
/// @}


///! \defgroup mqtt_packet_pubrel
/// @{
struct mqtt_packet_pubrel {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubrel_t;

/*! \brief pack mqtt publish release packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubrel[in] pointer to mqtt pubrel structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubrel(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_pubrel * p_packet_pubrel
		);

/*! \brief unpack mqtt pubrel packet
 *  \param p_packet_pubrel[in] pointer to mqtt pubrel structure
 *  \param p_packet[out] pointer to mqtt packet
 *  \retval 
 * */
mqtt_err_t mqtt_unpack_pubrel(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_pubrel * p_packet_pubrel
		);
/// @}


///! \defgroup mqtt_packet_pubcomp
/// @{
struct mqtt_packet_pubcomp {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubcomp_t;

/*! \brief pack mqtt publish complete packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pubcomp[in] pointer to mqtt pubcomp structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pubcomp(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_pubcomp * p_packet_pubcomp
		);

/*! \brief unpack mqtt pubcomp packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_pubcomp[out] pointer to pubcomp structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpacke_pubcomp(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_pubcomp * p_packet_pubcomp
		);
/// @}


///! \defgroup mqtt_packet_subscribe
/// @{
struct mqtt_packet_subscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_subscribe_content sub_content[],  //!< sub_content content array to subscribe
	uint16_t sub_count  //!< sub_count count of subscribe
} mqtt_packet_subscribe_t;

/*! \brief pack mqtt subscribe packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_subscribe[in] pointer to mqtt subscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_subscribe(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_subscribe * p_packet_subscribe
		);

/*! \brief unpack mqtt subscribe packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_subscribe[out] pointer to mqtt subscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_subscribe(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_subscribe * p_packet_subscribe
		);
/// @}


///! \defgroup mqtt_packet_suback
/// @{
struct mqtt_packet_suback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_suback_flag suback_flag[],  //!< suback_flag[] flags of suback array
	uint16_t suback_flag_count  //!< suback_flag_count count of suback
} mqtt_packet_suback_t;

/*! \brief pack mqtt suback packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_suback[in] pointer to mqtt suback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_suback(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_suback * p_packet_suback
		);

/*! \brief unpack mqtt suback packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_suback[in] pointer to mqtt suback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_suback(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_suback * p_packet_suback
		);
/// @}


///! \defgroup mqtt_packet_unsubscribe
/// @{
struct mqtt_packet_unsubscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * top_filter[],  //!< top_filter[] array of top_filter string
	uint16_t top_filter_count  //!< to_filter_count count of top_filter string
} mqtt_packet_unsubscribe_t;

/*! \brief pack mqtt unsubscribe packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_unsubscribe[in] pointer to mqtt unsubscribe structure
 *  \retval 
 * */
mqtt_err_t mqtt_pack_unsubscribe(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_unsubscribe * p_packet_unsubscribe
		);

/*! \brief unpack mqtt unsubscribe packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_unsubscribe[out] pointer to mqtt unsubscribe structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_unsubscribe(
		const struct mqtt_packet * p_packet,
		struct mqtt_packet_unsubscribe * p_packet_unsubscribe
		);
/// @}


///! \defgroup mqtt_packet_unsuback
/// @{
struct mqtt_packet_unsuback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_unsuback_t;

/*! \brief pack mqtt unsuback packet
 *  \param p_packet[out] pointer to mqtt unsuback packet
 *  \param p_packet_unsuback[in] pointer to mqtt unsuback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_unsuback(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_unsuback * p_packet_unsuback
		);

/*! \brief unpack mqtt unsuback packet
 *  \param p_packet[in] pointer to mqtt packet
 *  \param p_packet_unsuback[out] pointer to mqtt unsuback structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_unpack_unsuback(
		const struct mqtt_packet p_packet,
		struct mqtt_packet_unsuback * p_packet_unsuback
		);
/// @}


///! \defgroup mqtt_packet_pingreq
///  @{
typedef struct mqtt_packet_pingreq {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_packet_pingreq_t;

/*! \brief pack mqtt pingreq packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pingreq[in] pointer to mqtt pingreq structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pingreq(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_pingreq * p_packet_pingreq
		);
/// @}


///! \defgroup mqtt_packet_pingresp
///  @{
typedef struct mqtt_packet_pingresp {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_packet_pingresp_t;

/*! \brief pack mqtt pingresp packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_pingresp[in] pointer to mqtt pingresp structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_pingresp(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_pingresp * p_packet_pingresp
		);
/// @}


///! \defgroup mqtt_packet_disconnect
///  @{
typedef struct mqtt_packet_disconnect {
	//!< fixed header
	//!< variable header
	//!< payload
	void
} mqtt_packet_disconnect_t;

/*! \brief pack mqtt disconnect packet
 *  \param p_packet[out] pointer to mqtt packet
 *  \param p_packet_disconnect[in] pointer to mqtt disconnect structure
 *  \retval mqtt_err_t
 * */
mqtt_err_t mqtt_pack_disconnect(
		struct mqtt_packet * p_packet,
		const struct mqtt_packet_disconnect * p_packet_disconnect
		);
/// @}

/// @}


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

