#ifndef _MQTT_PACKET_SEGMENT_H_
#define _MQTT_PACKET_SEGMENT_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdint.h>
#include <assert.h>
#include <stddef.h>

///! \defgroup mqtt_err mqtt error definition
///  @{
typedef enum mqtt_err {
	E_NONE         = 0,  //!< error : none error
	E_FORMAT_CHECK = 1,  //!< error format : flag value check fail
	E_MEM_FAIL     = 2,
	E_MEM_OUT      = 3,
	E_NET_SOCK     = 4,  //!< socket create fail
	E_NET_CONN     = 5,  //!< socket connect fail
	E_NET_XFER     = 6,  //!< socket transfer fail
	E_SESS_ACK     = 7,  //!< mqtt ack fail
} mqtt_err_t;

/*! \brief convert err to coressponding prompt string
 *  \param err error index
 *  \retval pointer to error prompt string
 * */
const char * mqtt_str_error(mqtt_err_t err);
/// @}

///! \defgroup mqtt_packet_segment mqtt segment bits api(check,eval)
///  @{

/*! \brief evaluate the value of bits in uint variable
 *  \param uint the packed uint value
 *  \param msk the and operator mask
 *  \param offset the bits offset to bit 0
 * */
#define MQTT_BUF_SEGMENT_EVAL(uint,msk,offset) ((uint&msk) >> offset)

/*  \brief check reserved value attributes
 *  \param byte value to check
 * */
#define MQTT_ATTR_FLAG_VAL_RESERVED_CHECK(byte) assert(0 == byte)

/*! \brief check packet flag value of boolean type
 *  \param byte value of boolean
 * */
#define MQTT_ATTR_FLAG_VAL_BOOL_CHECK(byte) assert(0 == byte || 1 == byte)

/*! \brief check packet flag QoS value 
 *  \param byte value of QoS
 * */
#define MQTT_ATTR_FLAG_VAL_QoS_CHECK(byte)  assert(0 <= byte && 2 >= byte)


//! \defgroup mqtt_buf 
/// @{
typedef struct mqtt_buf {
	uint8_t * buf;  //!< binary data
	size_t    len;  //!< length of buf
} mqtt_buf_t;
	
/*! \brief create empty mqtt_buf with length by alloc memory
 *  \param len length of mqtt_buf->buf
 *  \retval pointer to mqtt_buf created
 * */
struct mqtt_buf * mqtt_buf_new(size_t len);

/*! \brief create a new mqtt buffer from simple buffer
 *  \param buf pointer to buffer
 *  \param len length of buffer
 *  \retval pointer to created mqtt buffer
 * */
mqtt_buf_t * mqtt_buf_new_4_buf(uint8_t * buf, size_t len);

/*! \brief release mqtt_buf by free memory
 *  \param buf mqtt_buf object pointer to release
 * */
void mqtt_buf_release(struct mqtt_buf * mq_buf);
/// @} group mqtt_buf

//! \defgroup mqtt_buf_flag 
/// @{
//typedef struct mqtt_buf struct mqtt_buf_flag;
#define mqtt_buf_flag   mqtt_buf
typedef mqtt_buf_t      mqtt_buf_flag_t;

typedef uint8_t         mqtt_attr_flag_t;
/// @}

//! \defgroup mqtt_buf_re_len remaining length buf
/// @{
//typedef struct mqtt_buf struct mqtt_buf_re_len;
#define mqtt_buf_re_len mqtt_buf
typedef mqtt_buf_t      mqtt_buf_re_len_t;

typedef uint32_t        mqtt_attr_re_len_t;
/// @}


//! \defgroup mqtt_buf_str
/// @{
//typedef struct mqtt_buf struct mqtt_buf_str;
#define mqtt_buf_str    mqtt_buf
typedef mqtt_buf_t      mqtt_buf_str_t;

typedef char* mqtt_attr_str_t;
/*! \brief encode c-string to mqtt string 
 *  \param str c-string
 *  \retval pointer to mqtt string
 * */
struct mqtt_buf_str * mqtt_buf_str_encode(const mqtt_attr_str_t str);
/*! \brief decode mqtt string to c-string
 *  \parma code mqtt string .buf|LEN_MSB|LEN_LSB|...| .len
 *  \retval c-string
 * */
mqtt_attr_str_t mqtt_buf_str_decode(const struct mqtt_buf_str * mq_str);
/*! \brief get string from mqtt string buffer
 *  \param buf buffer of mqtt string buffer|LEN_MSB|LEN_LSB|...|
 *  \retval c-string
 * */
mqtt_attr_str_t mqtt_buf_str_4_buf(const uint8_t * buf);
/// @}

//! \defgroup mqtt_buf_uint16 uint16_t bytes buf
/// @{
//typedef struct mqtt_buf struct mqtt_buf_uint16;
#define MQTT_BUF_STR_MAX_BYTE 2
#define MQTT_BUF_STR_MAX_LEN  0xFFFF
#define mqtt_buf_uint16 mqtt_buf
typedef mqtt_buf_t      mqtt_buf_uint16_t;

typedef uint16_t mqtt_attr_uint16_t;
/*! \brief encode uint16_t to mqtt_buf_uint16
 *  \param num mqtt_attr_uint16_t number
 *  \retval mqtt_buf_uint16 pointer
 * */
struct mqtt_buf_uint16 * mqtt_buf_uint16_encode(mqtt_attr_uint16_t num);
/*! \brief decode mqtt_buf_uint16 to uint16
 *  \param mq_uint16 pointer to mqtt_buf_uint16
 *  \retval mqtt_attr_uint16_t value
 * */
mqtt_attr_uint16_t mqtt_buf_uint16_decode(
		const struct mqtt_buf_uint16 * mq_uint16);
/// @}

///  @}

#ifdef __cplusplus
	}
#endif

#endif //!< _MQTT_PACKET_SEGMENT_H_

