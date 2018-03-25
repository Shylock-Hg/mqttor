/*  \brief API to upper layer 
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

/*  \brief evaluate the value of bits in uint variable
 *  \param uint the packed uint value
 *  \param msk the and operator mask
 *  \param offset the bits offset to bit 0
 * */
#define MQTT_PACKET_SEGMENT_EVAL(uint,msk,offset) ((uint&msk) >> offset)

/*  \brief check packet flag value of boolean type
 *  \param byte value of boolean
 * */
#define MQTT_PACKET_FLAG_VAL_BOOL_CHECK(byte) assert(0 == byte || 1 == byte)

/*  \brief check packet flag QoS value 
 *  \param byte value of QoS
 * */
#define MQTT_PACKET_FLAG_VAL_QoS_CHECK(byte)  assert(0 <= byte && 2 >= byte)


typedef struct mqtt_packet {
	const uint8_t * packet;  //!< pointer to all packet data
	uint32_t length;  //!< length of the whole packet
} mqtt_packet_t;

/*  \brief pack connect packet 
 *  \param p_conn_flag pointer to conn_flag structure
 *  \param keep_alive time limit to keep alive measured by seconds 
 *  \param id_client client identifier string
 *  \param w_topic will topic string
 *  \param w_msg will message string
 *  \param user_name user name string
 *  \param pwd password string
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_connect(
		//!< fixed header
		
		//!< variable header
		const struct mqtt_connect_flag * p_conn_flag, 
		uint16_t keep_alive,

		//!< payload
		const char * id_client,
		const char * w_topic,
		const char * w_msg,
		const char * user_name,
		const char * pwd
		);

/*  \brief pack mqtt connack packet
 *  \param p_connack_flag flags of connack
 *  \param ret_code return code of connect
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_connack(
		//!< fixed header
		
		//!< variable header
		const struct mqtt_connack_flag * p_connack_flag,
		enum mqtt_conn_ret_code ret_code
		//!< payload
		);

/*  \brief pack mqtt publish packet
 *  \param p_ctl_flag flag of mqtt packet control
 *  \param topic_name name of topic publish to 
 *  \param id_packet identifier of packet
 *  \param app_msg application specify message
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_publish(
		//!< fixed header
		const struct mqtt_ctl_head * p_ctl_flag,
		//!< variable header
		const char * topic_name,
		uint16_t id_packet,
		//!< payload
		const char * app_msg
		);

/*  \brief pack mqtt publish acknowledge packet
 *  \param id_packet identifier of packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_puback(
		//!< fixed header
		
		//!< variable header
		uint16_t id_packet
		//!< payload
		);

/*  \brief pack mqtt publish received packet
 *  \param id_packet identifier of packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_pubrec(
		//!< fixed header
		//!< variable header
		uint16_t id_packet
		//!< payload
		);

/*  \brief pack mqtt publish release packet
 *  \param id_packet identifier of packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_pubrel(
		//!< fixed header
		//!< variable header
		uint16_t id_packet
		//!< payload
		);

/*  \brief pack mqtt publish complete packet
 *  \param id_packet identifier of packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_pubcomp(
		//!< fixed header
		//!< variable header
		uint16_t id_packet
		//!< payload
		);

/*  \brief pack mqtt subscribe packet
 *  \param id_packet identifier of packet
 *  \param sub_content content array to subscribe 
 *  \param sub_count count of subscribe
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_subscribe(
		//!< fixed header
		//!< variable header
		uint16_t id_packet,
		//!< payload
		struct mqtt_payload_subscribe_content sub_content[],
		uint16_t sub_count
		);

/*  \brief pack mqtt suback packet
 *  \param id_packet identifier of packet
 *  \param suback_flag[] flags of suback array
 *  \param suback_flag_count count of suback
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_suback(
		//!< fixed header
		//!< variable header
		uint16_t id_packet,
		//!< payload
		struct mqtt_payload_suback_flag suback_flag[],
		uint16_t suback_flag_count
		);

/*  \brief pack mqtt unsubscribe packet
 *  \param id_packet identifier of packet
 *  \param top_filter[] array of top_filter string
 *  \param to_filter_count count of top_filter string
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_unsubscribe(
		//!< fixed header
		//!< variable header
		uint16_t id_packet,
		//!< payload
		const char * top_filter[],
		uint16_t top_filter_count
		);

/*  \brief pack mqtt unsuback packet
 *  \param id_packet identifier of packet
 * */
struct mqtt_packet * mqtt_packet_unsuback(
		//!< fixed header
		//!< variable header
		uint16_t id_packet
		//!< payload
		);

/*  \brief pack mqtt pingreq packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_pingreq(
		//!< fixed header
		//!< variable header
		//!< payload
		);

/*  \brief pack mqtt pingresp packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_pingresp(
		//!< fixed header
		//!< variable header
		//!< payload
		);

/*  \brief pack mqtt disconnect packet
 *  \retval mqtt packet
 * */
struct mqtt_packet * mqtt_packet_disconnect(
		//!< fixed header
		//!< variable header
		//!< payload
		);


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

