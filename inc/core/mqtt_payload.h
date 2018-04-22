/*! \brief API to pack&unpack payload segment of mqtt packet
 *
 * */

#ifndef _MQTT_PAYLOAD_H_
#define _MQTT_PAYLOAD_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <assert.h>
#include <stdint.h>

#include "../toolkit/array.h"
#include "mqtt_packet_segment.h"

/***********if packet with payload***********/


/*! \brief check if packet type with payload
 * */
#define MQTT_PAYLOAD_CHECK(mqtt_ctl_type) assert(\
		MQTT_CTL_TYPE_CONNECT == mqtt_ctl_type || \
		MQTT_CTL_TYPE_PUBLISH == mqtt_ctl_type || \
		(MQTT_CTL_TYPE_SUBSCRIBE <= mqtt_ctl_type && \
		 MQTT_CTL_TYPE_UNSUBSCRIBE >= mqtt_ctl_type))


/***********mqtt subscribe acknowledge flag***********/

///!  \defgroup mqtt_payload_suback_flag mqtt suback payload flags byte
////  @{

/*! \brief mqtt subscribe acknowledge flags
 *
 * */
/*
typedef struct mqtt_payload_suback_flag {
	uint8_t ok;  //!< fail or success
	uint8_t QoS;  //!< quality of delivery
} mqtt_payload_suback_flag_t;
*/
typedef struct mqtt_payload_suback_flag {
	uint8_t ok:1;
	uint8_t reserved:5;
	uint8_t QoS:2;
} mqtt_payload_sub_ack_flag_t;

typedef union mqtt_attr_payload_suback_flag {
	uint8_t all;
	struct mqtt_payload_suback_flag bits;
} mqtt_attr_payload_suback_flag_t;

#define mqtt_buf_payload_suback_flag mqtt_buf
typedef mqtt_buf_t mqtt_buf_payload_suback_flag_t;

#define MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET  7
#define MQTT_PAYLOAD_SUBACK_FLAG_OK_Msk     (BIT(MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET))

#define MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET 0
#define MQTT_PAYLOAD_SUBACK_FLAG_QoS_Msk    (BIT(MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET) | \
		BIT(MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET+1))

#define MQTT_PAYLOAD_SUBACK_FLAG_PACK(flag)         ((uint8_t)(flag.all))
#define MQTT_PAYLOAD_SUBACK_FLAG_UNPACK(p_buf_flag) \
	((union mqtt_attr_payload_suback_flag)(p_buf_flag->buf[0]))

/*! \brief pack suback flags byte
 *  \param p_flag flags structure
 *  \retval suback flags byte
 * */
struct mqtt_buf_payload_suback_flag * mqtt_payload_suback_flag_pack(
		union mqtt_attr_payload_suback_flag flag);
union mqtt_attr_payload_suback_flag mqtt_payload_suback_flag_unpack(
		const struct mqtt_buf_payload_suback_flag * p_buf_flag);


///  @}


/***********mqtt packet subscribe content***********/

///! \defgroup mqtt_payload_subscribe_content 
///  @{

typedef struct mqtt_payload_subscribe_content {
	char * top_filter;  //!< topic filter
	uint8_t QoS;  //!< requested QoS
} mqtt_payload_subscribe_content_t;

#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET 0
#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_Msk    \
		(BIT(MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET) | \
		BIT(MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET))

///  @}

#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PAYLOAD_H_

