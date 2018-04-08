#ifndef _MQTT_PACKET_SEGMENT_H_
#define _MQTT_PACKET_SEGMENT_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <assert.h>

///! \defgroup mqtt_packet_segment mqtt segment bits api(check,eval)
///  @{

/*! \brief evaluate the value of bits in uint variable
 *  \param uint the packed uint value
 *  \param msk the and operator mask
 *  \param offset the bits offset to bit 0
 * */
#define MQTT_PACKET_SEGMENT_EVAL(uint,msk,offset) ((uint&msk) >> offset)

/*! \brief check packet flag value of boolean type
 *  \param byte value of boolean
 * */
#define MQTT_PACKET_FLAG_VAL_BOOL_CHECK(byte) assert(0 == byte || 1 == byte)

/*! \brief check packet flag QoS value 
 *  \param byte value of QoS
 * */
#define MQTT_PACKET_FLAG_VAL_QoS_CHECK(byte)  assert(0 <= byte && 2 >= byte)

///  @}

#ifdef __cplusplus
	}
#endif

#endif //!< _MQTT_PACKET_SEGMENT_H_

