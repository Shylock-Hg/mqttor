/*! \brief API of pack&unpack variable header of mqtt packet
 *
 * */

#ifndef _MQTT_VAR_HEADER_H_
#define _MQTT_VAR_HEADER_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <assert.h>
#include <stdint.h>

#include "./array.h"
#include "./mqtt_packet_segment.h"

/*********** packet identifier ***********/

/*! \brief evaluate identifier value `uint16_t`
 * */
//#define MQTT_PACKET_IDENTIFIER_EVAL(identifier) BYTES_2_UINT16(identifier)

/*! \brief pack identifier value to bytes[2]
 * */
//#define MQTT_PACKET_IDENTIFIER_PACK(identifier,bytes) UINT16_2_BYTES(identifier,bytes)

/*! \brief check packet type with identifier
 * */
#define MQTT_PACKET_IDENTIFIER_CHECK(mqtt_ctl_type) assert(\
		MQTT_CTL_TYPE_PUBLISH <= mqtt_ctl_type && \
		MQTT_CTL_TYPE_UNSUBACK >= mqtt_ctl_type)

/*********** protocol name ***********/
#define MQTT_PROTOCOL_NAME "MQTT"

/*********** connect flags ***********/

///! \defgroup mqtt_connect_flag
///  @{

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

/*
typedef struct mqtt_connect_flag {
	uint8_t flag_user_name;  //!< 0-1
	uint8_t flag_pwd;  //!< 0-1
	uint8_t flag_w_retain;  //!< 0-1
	uint8_t flag_w_QoS;  //!< 0-3
	uint8_t flag_w_flag;  //!< 0-1
	uint8_t flag_clean_session;  //!< 0-1
	//uint8_t flag_reserved;  //!< 0-1
} mqtt_connect_flag_t;
*/
//typedef struct mqtt_connect_flag  mqtt_connect_flag;

typedef union mqtt_attr_connect_flag {
	uint8_t all;
	struct {
		uint8_t flag_reserved     :1;  //!< 0-1
		uint8_t flag_clean_session:1;  //!< 0-1
		uint8_t flag_w_flag       :1; //!< 0-1
		uint8_t flag_w_QoS        :2;  //!< 0-3
		uint8_t flag_w_retain     :1;  //!< 0-1
		uint8_t flag_pwd          :1;  //!< 0-1
		uint8_t flag_user_name    :1;  //!< 0-1
	} bits;
} mqtt_attr_connect_flag_t;

typedef mqtt_buf_t mqtt_buf_connect_flag_t;
#define mqtt_buf_connect_flag mqtt_buf


/*! \brief evaluate mqtt connect flag value
 *  \param byte mqtt connect flag byte
 *  \param msk mask of mqtt connect flags
 *  \offset offset of mqtt connect flags
 *  \retval value of mqtt connect flags
 * */
//#define MQTT_CONNECT_FLAG_EVAL(byte,msk,offset)   ((byte & msk) >> offset)

/*! \brief pack mqtt connect flags to byte
 *  \param p_mqtt_connect_flag pointer to mqtt connect flags structure
 *  \retval mqtt connect flags byte value
 * */
#define MQTT_CONNECT_FLAG_PACK(flag)         ((uint8_t)flag.all)
#define MQTT_CONNECT_FLAG_UNPACK(p_buf_flag) ((union mqtt_attr_connect_flag)(p_buf_flag->buf[0]))

/*! \brief pack mqtt connect flags to byte
 *  \param flag mqtt connect flags bits field
 *  \retval mqtt connect flag buffer
 * */
struct mqtt_buf_connect_flag * mqtt_connect_flag_pack(union mqtt_attr_connect_flag flag); 

/* \brief unpack mqtt connect flags buffer
 * \param p_buf_flag pointer to mqtt connect flags buffer
 * \retval mqtt connect flags bits field
 * */
union mqtt_attr_connect_flag mqtt_connect_flag_unpack(const struct mqtt_buf_connect_flag * p_buf_flag); 

///  @}

/***********connect acknowledge flags***********/

///! \defgroup mqtt_connack_flag
///  @{

//typedef struct mqtt_connack_flag  mqtt_connack_flag_t;

typedef union mqtt_attr_connack_flag {
	uint8_t all;
	struct {
		uint8_t SP:1;  //!< 0-1
		uint8_t reserved:7;
	} bits;
} mqtt_attr_connack_flag_t;

#define mqtt_buf_connack_flag mqtt_buf
typedef mqtt_buf_t mqtt_buf_connack_flag_t;

#define MQTT_CONNACK_FLAG_SP_OFFSET          0
#define MQTT_CONNACK_FLAG_SP_Msk             BIT(MQTT_CONNACK_FLAG_SP_OFFSET)
#define MQTT_CONNACK_FLAG_PACK(flag)         ((uint8_t)flag.all)
#define MQTT_CONNACK_FLAG_UNPACK(p_buf_flag) ((union mqtt_attr_connack_flag)(p_buf_flag->buf[0]))

struct mqtt_buf_connack_flag * mqtt_connack_flag_pack(union mqtt_attr_connack_flag flag);
union mqtt_attr_connack_flag mqtt_connack_flag_unpack(const struct mqtt_buf_connack_flag * p_buf_flag);

///  @}

//!< evaluate bit value by universal API

/***********connect return code***********/

///! \defgroup mqtt_connect_ret_code
///  @{

typedef enum mqtt_attr_connack_ret_code {
	CONNECT_RET_CODE_ACCEPTED,  //!< accepte the connect
	CONNECT_RET_CODE_REF_VER,  //!< unavailable protocol version suported by server
	CONNECT_RET_CODE_REF_ID,  //!< identifier is format-legal but not allowed by server
	CONNECT_RET_CODE_REF_SERVER,  //!< server unavailable
	CONNECT_RET_CODE_REF_VALI,  //!< bad user name or password
	CONNECT_RET_CODE_REF_AUTH,  //!< not authoried 
	CONNECT_RET_CODE_RESERVED  //!< more value reserved
} mqtt_attr_connack_ret_code_t;

#define MQTT_CONNECT_RET_CODE_CHECK(conn_ret_code) assert(CONNECT_RET_CODE_RESERVED > conn_ret_code)

///  @}

#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTT_VAR_HEADER_H_

