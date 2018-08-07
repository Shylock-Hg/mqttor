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

//!< representation of mqtt payload attribute
typedef struct mqtt_attr_payload {
	uint8_t * buf;
	size_t len;
	size_t len_valid;
} mqtt_attr_payload_t;

/*! \brief create a mqtt attribute payload
 *  \param len length of paylod buffer
 *  \retval created object
 * */
mqtt_attr_payload_t * mqtt_attr_payload_new(size_t len);
/*! \brief release a mqtt attribute payload
 *  \param payload pointer of mqtt attribute paylod
 * */
void mqtt_attr_payload_release(mqtt_attr_payload_t * payload);
/*! \brief convert mqtt attribute payload to mqtt buffer
 *  \param payload pointer to mqtt attribute payload
 *  \retval pointer to mqtt buffer
 *  \note don't alloc new memory, just convert , and can release by mqtt_buf_release
 * */
mqtt_buf_t * mqtt_attr_payload_2_buf(mqtt_attr_payload_t * payload);
/*! \brief create a mqtt buffer from a payload buffer
 *  \param payload payload buffer
 *  \retval created mqtt buffer
 * */
mqtt_buf_t * mqtt_attr_payload_deep2_buf(mqtt_attr_payload_t * payload);

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
//typedef struct mqtt_payload_suback_flag  mqtt_payload_sub_ack_flag_t;

typedef union mqtt_attr_payload_suback_flag {
	uint8_t all;
	struct {
		uint8_t QoS:2;
		uint8_t reserved:5;
		uint8_t ok:1;
	} bits;
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

//typedef struct mqtt_payload_subscribe_content_QoS  mqtt_payload_subscribe_content_QoS_t;

typedef union mqtt_attr_payload_subscribe_content_QoS {
	uint8_t all;
	struct {
		uint8_t QoS:2;
		uint8_t reserved:6;
	} bits;
} mqtt_attr_payload_subscribe_content_QoS_t;

#define mqtt_buf_payload_subscribe_content_QoS mqtt_buf
typedef mqtt_buf_t mqtt_buf_payload_subscribe_content_QoS_t;

#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_QoS_PACK(flag)         ((uint8_t)(flag.all))
#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_QoS_UNPACK(p_buf_flag) \
	((union mqtt_attr_payload_subscribe_content_QoS)(p_buf_flag->buf[0]))

struct mqtt_buf_payload_subscribe_content_QoS * mqtt_payload_subscribe_content_QoS_pack(
		union mqtt_attr_payload_subscribe_content_QoS flag;
		);
union mqtt_attr_payload_subscribe_content_QoS mqtt_payload_subscribe_content_QoS_unpack(
		const struct mqtt_buf_payload_subscribe_content_QoS * p_buf_flag
		);

typedef struct mqtt_payload_subscribe_content {
	const mqtt_attr_str_t top_filter;  //!< topic filter
	union mqtt_attr_payload_subscribe_content_QoS QoS;  //!< requested QoS
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

