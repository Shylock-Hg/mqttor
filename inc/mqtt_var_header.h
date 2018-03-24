/*  \brief API of pack&unpack variable header of mqtt packet
 *
 * */

#ifndef _MQTT_VAR_HEADER_H_
#define _MQTT_VAR_HEADER_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include "../src/toolkit/array.h"
#include "mqtt_fixed_header.h"

#include <assert.h>

/*********** packet identifier ***********/

/*  \brief evaluate identifier value `uint16_t`
 * */
#define MQTT_PACKET_IDENTIFIER_EVAL(identifier) BYTES_2_UINT16(identifier)

/*  \brief pack identifier value to bytes[2]
 * */
#define MQTT_PACKET_IDENTIFIER_PACK(identifier,bytes) UINT16_2_BYTES(identifier,bytes)

/*  \brief check packet type with identifier
 * */
#define MQTT_PACKET_IDENTIFIER_CHECK(mqtt_ctl_type) assert(\
		MQTT_CTL_TYPE_PUBLISH <= mqtt_ctl_type && \
		MQTT_CTL_TYPE_UNSUBACK >= mqtt_ctl_type)



#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_VAR_HEADER_H_

