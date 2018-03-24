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
#include <stdint.h>

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

/*********** protocol name ***********/
#define MQTT_PROTOCOL_NAME "MQTT"

/*********** connect flags ***********/
#define MQTT_CONNECT_FLAG_USER_NAME_OFFSET     7
#define MQTT_CONNECT_FLAG_PWD_OFFSET           6
#define MQTT_CONNECT_FLAG_W_RETAIN_OFFSET      5
#define MQTT_CONNECT_FLAG_W_QoS_OFFSET         3  //!< two bits
#define MQTT_CONNECT_FLAG_W_FLAG_OFFSET        2
#define MQTT_CONNECT_FLAG_CLEAN_SESSION_OFFSET 1
#define MQTT_CONNECT_FLAG_RESERVED_OFFSET      0

#define MQTT_CONNECT_FLAG_USER_NAME_Msk     BIT(MQTT_CONNECT_FLAG_USER_NAME_OFFSET) 
#define MQTT_CONNECT_FLAG_PWD_Msk           BIT(MQTT_CONNECT_FLAG_PWD_OFFSET)
#define MQTT_CONNECT_FLAG_W_RETAIN_Msk      BIT(MQTT_CONNECT_FLAG_W_RETAIN_OFFSET)
#define MQTT_CONNECT_FLAG_W_QoS_Msk         (BIT(MQTT_CONNECT_FLAG_W_QoS_OFFSET) | \
		BIT(MQTT_CONNECT_FLAG_W_QoS_OFFSET+1))  //!< two bits
#define MQTT_CONNECT_FLAG_W_FLAG_Msk        BIT(MQTT_CONNECT_FLAG_W_FLAG_OFFSET)
#define MQTT_CONNECT_FLAG_CLEAN_SESSION_Msk BIT(MQTT_CONNECT_FLAG_CLEAN_SESSION_OFFSET)
#define MQTT_CONNECT_FLAG_RESERVED_Msk      BIT(MQTT_CONNECT_FLAG_RESERVED_OFFSET)

typedef struct mqtt_connect_flag {
	uint8_t flag_user_name;  //!< 0-1
	uint8_t flag_pwd;  //!< 0-1
	uint8_t flag_w_retain;  //!< 0-1
	uint8_t flag_w_QoS;  //!< 0-3
	uint8_t flag_w_flag;  //!< 0-1
	uint8_t flag_clean_session;  //!< 0-1
	uint8_t flag_reserved;  //!< 0-1
} mqtt_connect_flag_t;

#define MQTT_CONNECT_FLAG_CHECK_BOOL(byte) assert(0 == byte || 1 == byte)
#define MQTT_CONNECT_FLAG_CHECK_QoS(byte)  assert(0 <= byte || 2 >= byte)

/*  \brief evaluate mqtt connect flag value
 *  \param byte mqtt connect flag byte
 *  \param msk mask of mqtt connect flags
 *  \offset offset of mqtt connect flags
 *  \retval value of mqtt connect flags
 * */
#define MQTT_CONNECT_FLAG_EVAL(byte,msk,offset)   ((byte & msk) >> offset)

/*  \brief pack mqtt connect flags to byte
 *  \param p_mqtt_connect_flag pointer to mqtt connect flags structure
 *  \retval mqtt connect flags byte value
 * */
#define MQTT_CONNECT_FLAG_PACK(p_mqtt_connect_flag) (\
		(p_mqtt_connect_flag->flag_user_name) << MQTT_CONNECT_FLAG_USER_NAME_OFFSET | \
		(p_mqtt_connect_flag->flag_pwd) << MQTT_CONNECT_FLAG_PWD_OFFSET | \
		(p_mqtt_connect_flag->flag_w_retain) << MQTT_CONNECT_FLAG_W_RETAIN_OFFSET | \
		(p_mqtt_connect_flag->flag_w_QoS) << MQTT_CONNECT_FLAG_W_QoS_OFFSET | \
		(p_mqtt_connect_flag->flag_w_flag) << MQTT_CONNECT_FLAG_W_FLAG_OFFSET | \
		(p_mqtt_connect_flag->flag_clean_session) << MQTT_CONNECT_FLAG_CLEAN_SESSION_OFFSET | \
		(p_mqtt_connect_flag->flag_reserved) << MQTT_CONNECT_FLAG_RESERVED_OFFSET)

/*  \brief pack mqtt connect flags to byte
 *  \param p_mqtt_connect_flag pointer to mqtt connect flags structure
 *  \retval mqtt connect flags byte value
 * */
uint8_t mqtt_connect_flag_pack_s(struct mqtt_connect_flag * p_mqtt_connect_flag); 


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_VAR_HEADER_H_

