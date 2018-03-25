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

/*  \brief evaluate the value of bits in uint variable
 *  \param uint the packed uint value
 *  \param msk the and operator mask
 *  \param offset the bits offset to bit 0
 * */
#define MQTT_PACKET_SEGMENT_EVAL(uint,msk,offset) ((uint&msk) >> offset)

#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_PACKET_H_

