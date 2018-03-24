/*  \brief API to pack&unpack payload segment of mqtt packet
 *
 * */

#ifndef _MQTT_PAYLOAD_H_
#define _MQTT_PAYLOAD_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <assert.h>

/*  \brief check if packet type with payload
 * */
#define MQTT_PAYLOAD_CHECK(mqtt_ctl_type) assert(\
		MQTT_CTL_TYPE_CONNECT == mqtt_ctl_type || \
		MQTT_CTL_TYPE_PUBLISH == mqtt_ctl_type || \
		(MQTT_CTL_TYPE_SUBSCRIBE <= mqtt_ctl_type && \
		 MQTT_CTL_TYPE_UNSUBSCRIBE >= mqtt_ctl_type))



#ifdef __cplusplus
	}
#endif

#endif

