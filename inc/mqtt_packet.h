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
	E_FORMAT_CHECK,  //!< error format : flag value check fail
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
 *  \param p_packet  mqtt packet
 *  \param p_packet_connect pointer to connect packect 
 *  \
 * */
mqtt_pack_connect(
		struct mqtt_packet * p_packet,
		struct mqtt_packet_connect * p_packet_connect
		);

/*! \brief unpack connect packet
 *  \param p_packet pointer to mqtt packet received
 *  \retval pointer to mqtt connect structure
 * */
struct mqtt_packet_connect * mqtt_unpack_connect(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_connack {
	//!< fixed header
	
	//!< variable header
	const struct mqtt_connack_flag * p_connack_flag,  //!< p_connack_flag flags of connack
	enum mqtt_conn_ret_code ret_code  //!< ret_code return code of connect
	//!< payload
	
} mqtt_packet_connack_t;

/*! \brief pack mqtt connack packet
 *  \param 
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_pack_connack(
		struct mqtt_packet_connack * p_packet_connack
		);

/*! \brief unpack mqtt connack packet
 *  \param p_packet mqtt packet pointer
 *  \retval pointer to mqtt connack structure
 * */
struct mqtt_packet_connack * mqtt_unpack_connack(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_publish {
	//!< fixed header
	const struct mqtt_ctl_head * p_ctl_flag,  //!< p_ctl_flag flag of mqtt packet control
	//!< variable header
	const char * topic_name,  //!< topic_name name of topic publish to
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * app_msg  //!< app_msg application specify message
} mqtt_packet_publish_t;

/*! \brief pack mqtt publish packet
 *  \param p_packet_publish pointer to publish structure
 *  \retval pointer to mqtt packet 
 * */
struct mqtt_packet * mqtt_pack_publish(
		struct mqtt_packet_publish * p_packet_publish
		);

/*! \brief unpack mqtt publish packet 
 *  \param p_packet mqtt packet 
 *  \retval pointer to mqtt publish structure
 * */
struct mqtt_packet_publish * mqtt_unpack_publish(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_puback {
	//!< fixed header
	
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_puback_t;

/*! \brief pack mqtt publish acknowledge packet
 *  \param  pointer to mqtt puback structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_puback(
		struct mqtt_packet_puback * p_packet_puback
		);

/*! \brief unpack matt puback packet 
 *  \param p_packet pointer to mqtt packet
 *  \retval mqtt puback structure
 * */
struct mqtt_packet_puback * mqtt_unpack_puback(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_pubrec {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubrec_t;

/*! \brief pack mqtt publish received packet
 *  \param p_packet_pubrec pointer to mqtt pubrec structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_pubrec(
		struct mqtt_packet_pubrec * p_packet_pubrec
		);

/*! \brief unpack mqtt pubrec packet 
 *  \param p_packet pointer to mqtt pubrec packet
 *  \retval pointer to mqtt pubrec structure
 * */
struct mqtt_packet_pubrec * mqtt_unpack_pubrec(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_pubrel {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubrel_t;

/*! \brief pack mqtt publish release packet
 *  \param p_packet_pubrel pointer to mqtt pubrel structure
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_pack_pubrel(
		struct mqtt_packet_pubrel * p_packet_pubrel
		);

/*! \brief unpack mqtt pubrel packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to mqtt pubrel structure
 * */
struct mqtt_packet_pubrel * mqtt_unpack_pubrel(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_pubcomp {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_pubcomp_t;

/*! \brief pack mqtt publish complete packet
 *  \param p_packet_pubcomp pointer to mqtt pubcomp structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_pubcomp(
		struct mqtt_packet_pubcomp * p_packet_pubcomp
		);

/*! \brief unpack mqtt pubcomp packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to pubcomp structure
 * */
struct mqtt_packet_pubcomp * mqtt_unpacke_pubcomp(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_subscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_subscribe_content sub_content[],  //!< sub_content content array to subscribe
	uint16_t sub_count  //!< sub_count count of subscribe
} mqtt_packet_subscribe_t;

/*! \brief pack mqtt subscribe packet
 *  \param p_packet_subscribe pointer to mqtt subscribe structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_subscribe(
		struct mqtt_packet_subscribe * p_packet_subscribe
		);

/*! \brief unpack mqtt subscribe packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to mqtt subscribe structure
 * */
struct mqtt_packet_subscribe * mqtt_unpack_subscribe(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_suback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	struct mqtt_payload_suback_flag suback_flag[],  //!< suback_flag[] flags of suback array
	uint16_t suback_flag_count  //!< suback_flag_count count of suback
} mqtt_packet_suback_t;

/*! \brief pack mqtt suback packet
 *  \param p_packet_suback pointer to mqtt suback structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_suback(
		struct mqtt_packet_suback * p_packet_suback
		);

/*! \brief unpack mqtt suback packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to mqtt suback structure
 * */
struct mqtt_packet_suback * mqtt_unpack_suback(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_unsubscribe {
	//!< fixed header
	//!< variable header
	uint16_t id_packet,  //!< id_packet identifier of packet
	//!< payload
	const char * top_filter[],  //!< top_filter[] array of top_filter string
	uint16_t top_filter_count  //!< to_filter_count count of top_filter string
} mqtt_packet_unsubscribe_t;

/*! \brief pack mqtt unsubscribe packet
 *  \param p_packet_unsubscribe pointer to mqtt unsubscribe structure
 *  \retval pointer to mqtt packet
 * */
struct mqtt_packet * mqtt_pack_unsubscribe(
		struct mqtt_packet_unsubscribe * p_packet_unsubscribe
		);

/*! \brief unpack mqtt unsubscribe packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to mqtt unsubscribe structure
 * */
struct mqtt_packet_unsubscribe * mqtt_unpack_unsubscribe(
		struct mqtt_packet * p_packet
		);


struct mqtt_packet_unsuback {
	//!< fixed header
	//!< variable header
	uint16_t id_packet  //!< id_packet identifier of packet
	//!< payload
} mqtt_packet_unsuback_t;

/*! \brief pack mqtt unsuback packet
 *  \param p_packet_unsuback pointer to mqtt unsuback structure
 *  \retval pointer to mqtt unsuback packet
 * */
struct mqtt_packet * mqtt_pack_unsuback(
		struct mqtt_packet_unsuback * p_packet_unsuback
		);

/*! \brief unpack mqtt unsuback packet
 *  \param p_packet pointer to mqtt packet
 *  \retval pointer to mqtt unsuback structure
 * */
struct mqtt_packet_unsuback * mqtt_unpack_unsuback(
		struct mqtt_packet p_packet
		);


/*! \brief pack mqtt pingreq packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_pack_pingreq(
		//!< fixed header
		//!< variable header
		//!< payload
		void
		);


/*! \brief pack mqtt pingresp packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_pack_pingresp(
		//!< fixed header
		//!< variable header
		//!< payload
		void
		);


/*! \brief pack mqtt disconnect packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_pack_disconnect(
		//!< fixed header
		//!< variable header
		//!< payload
		void
		);

/// @}


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

