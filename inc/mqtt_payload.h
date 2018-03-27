/*! \brief API to pack&unpack payload segment of mqtt packet
 *
 * */

#ifndef _MQTT_PAYLOAD_H_
#define _MQTT_PAYLOAD_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "../src/toolkit/array.h"

#include <assert.h>
#include <stdint.h>

/***********if packet with payload***********/
/*! \brief check if packet type with payload
 * */
#define MQTT_PAYLOAD_CHECK(mqtt_ctl_type) assert(\
		MQTT_CTL_TYPE_CONNECT == mqtt_ctl_type || \
		MQTT_CTL_TYPE_PUBLISH == mqtt_ctl_type || \
		(MQTT_CTL_TYPE_SUBSCRIBE <= mqtt_ctl_type && \
		 MQTT_CTL_TYPE_UNSUBSCRIBE >= mqtt_ctl_type))


/***********mqtt subscribe acknowledge flag***********/
/*! \brief mqtt subscribe acknowledge flags
 *
 * */
typedef struct mqtt_payload_suback_flag {
	uint8_t ok;  //!< fail or success
	uint8_t QoS;  //!< quality of delivery
} mqtt_payload_suback_flag_t;

#define MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET  7
#define MQTT_PAYLOAD_SUBACK_FLAG_OK_Msk     (BIT(MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET))

#define MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET 0
#define MQTT_PAYLOAD_SUBACK_FLAG_QoS_Msk    (BIT(MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET) | \
		BIT(MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET+1))

#define MQTT_PAYLOAD_SUBACK_FLAG_PACK(p_mqtt_payload_suback_flag) (\
		(p_mqtt_payload_suback_flag->ok << MQTT_PAYLOAD_SUBACK_FLAG_OK_OFFSET) | \
		(p_mqtt_payload_suback_flag->QoS << MQTT_PAYLOAD_SUBACK_FLAG_QoS_OFFSET))

/*! \brief pack suback flags byte
 *  \param p_flag flags structure
 *  \retval suback flags byte
 * */
uint8_t mqtt_payload_suback_flag_pack_s(struct mqtt_payload_suback_flag * p_flag);


/***********mqtt packet subscribe content***********/
typedef struct mqtt_payload_subscribe_content {
	char * top_filter;  //!< topic filter
	uint8_t QoS;  //!< requested QoS
} mqtt_payload_subscribe_content_t;

#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET 0
#define MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_Msk    (BIT(MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET) | \
		BIT(MQTT_PAYLOAD_SUBSCRIBE_CONTENT_FLAG_QoS_OFFSET))


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PAYLOAD_H_

