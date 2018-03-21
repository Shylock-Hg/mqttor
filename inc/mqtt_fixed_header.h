#ifndef _MQTT_FIXED_HEADER_H_
#define _MQTT_FIXED_HEADER_H_

#ifdef __cplusplus
        extern "C" {
#endif

#include <assert.h>
#include <stdint.h>

/*********** macro of MQTT protocol datagram  ***********/

/*  \brief overview of MQTT protocol datagram
 *         |segment|description|
 *         |:--|:--|
 *         |Fixed header|All MQTT control packets|
 *         |Variable header|Some MQTT control packets|
 *         |Payload|Some MQTT control packets|
 * */

/*  \brief Fixed header
 *         [B0]|control packet type[7:4]|control packet flag specific to each type[3:0]|
 *         [B0-B3]|Remaining length|
 *
 * */

/*********** MQTT control packet type ***********/

/*  \brief MQTT contorl packet type macro
 *
 * */
typedef enum mqtt_ctl_type{
        MQTT_CTL_TYPE_LOWER_BORDER, 

        MQTT_CTL_TYPE_CONNECT,  
        MQTT_CTL_TYPE_CONNACK,  

        MQTT_CTL_TYPE_PUBLISH,  
        MQTT_CTL_TYPE_PUBACK,   
        MQTT_CTL_TYPE_PUBREC,   
        MQTT_CTL_TYPE_PUBREL,
        MQTT_CTL_TYPE_PUBCOMP,

        MQTT_CTL_TYPE_SUBSCRIBE,
        MQTT_CTL_TYPE_SUBACK,
        MQTT_CTL_TYPE_UNSUBSCRIBE,
        MQTT_CTL_TYPE_UNSUBACK,

        MQTT_CTL_TYPE_PINGREQ,
        MQTT_CTL_TYPE_PINGRESP,

        MQTT_CTL_TYPE_DISCONNECT,

        MQTT_CTL_TYPE_UPPER_BORDER

} mqtt_ctl_type_t; 

/*  \brief check mqtt control type 
 * */
#define MQTT_CTL_TYPE_CHECK(type) assert(MQTT_CTL_TYPE_LOWER_BORDER < type && \
                type < MQTT_CTL_TYPE_UPPER_BORDER)

/*  \brief macro to get mqtt_ctl_type value from control type segment 
 *
 * */
#define MQTT_CTL_TYPE_OFFSET         4
#define MQTT_CTL_TYPE_EVAL(byte)     (byte >> MQTT_CTL_TYPE_OFFSET)

/*********** control packet flag ***********/
/*  \brief |DUP[3]|QoS[2:1]|RETAIN[0]|
 * */

/*  \brief evaluate value of flag DUP
 * */
#define MQTT_CTL_FLAG_DUP_CHECK(DUP) assert(0 == DUP || 1 == DUP)
#define MQTT_CTL_FLAG_DUP_Msk        0x08
#define MQTT_CTL_FLAG_DUP_OFFSET     3
#define MQTT_CTL_FLAG_DUP_EVAL(byte) (byte & MQTT_CTL_FLAG_DUP_Msk >> MQTT_CTL_FLAG_DUP_OFFSET)

/*  \brief evaluate value of flag QoS
 * */
#define MQTT_CTL_FLAG_QoS_CHECK(QoS) assert(0 <= QoS && QoS <=2)
#define MQTT_CTL_FLAG_QoS_Msk        0x06
#define MQTT_CTL_FLAG_QoS_OFFSET     1
#define MQTT_CTL_FLAG_QoS_EVAL(byte) (byte & MQTT_CTL_FLAG_QoS_Msk >> MQTT_CTL_FLAG_QoS_OFFSET)

/*  \brief evaluate value of flag RETAIN
 * */
#define MQTT_CTL_FLAG_RETAIN_CHECK(RETAIN) assert(0 == REATAIN || 1 == RETAIN)
#define MQTT_CTL_FLAG_RETAIN_Msk           0x01
#define MQTT_CTL_FLAG_RETAIN_OFFSET        0
#define MQTT_CTL_FLAG_RETAIN_EVAL(byte)    (byte & MQTT_CTL_FLAG_RETAIN_Msk >> MQTT_CTL_FLAG_RETAIN_OFFSET)


/*********** pack mqtt control packet head ***********/
typedef struct mqtt_ctl_head{
        mqtt_ctl_type_t type;
        uint8_t DUP;  //!< [0-1]
        uint8_t QoS;  //!< [0-2]
        uint8_t RETAIN;  //!< [0-1]
} mqtt_ctl_head_t;

/*  \brief pack MQTT control head without check
 *  \param p_header[in] the header struct
 * */
#define MQTT_CTL_HEAD_PACK(p_mqtt_ctl_head) (p_mqtt_ctl_head_t->type<<MQTT_CTL_TYPE_OFFSET | \
                (p_mqtt_ctl_head->DUP & MQTT_CTL_FLAG_DUP_Msk)<<MQTT_CTL_FLAG_DUP_OFFSET | \
                (p_mqtt_ctl_head->QoS & MQTT_CTL_FLAG_QoS_Msk)<<MQTT_CTL_FLAG_QoS_OFFSET)

/*  \brief pack mqtt control packet head with check
 *  \param p_header[in] the header struct
 *  \retval the byte packed
 * */
uint8_t mqtt_ctl_head_pack_s(struct mqtt_ctl_head * p_header);

/*********** encoding & decoding remaining length ***********/
/*  \brief decode remaining length
 *  \param code[in] the encoded code 
 *  \retval the value of remaining length
 *          -1 for err
 * */
int mqtt_ctl_decode_remaining_len(const uint8_t * code);

/*  \brief encode the remaining length
 *  \param code[out] the code bytes encoded from length
 *  \param length the value of remaining length to encode
 *  \retval the length of the encoded bytes
 * */
int mqtt_ctl_encode_remaining_len(uint8_t * code, uint32_t length);

#ifdef __cplusplus
        }  //!< extern "C"
#endif

#endif  //!< _MQTT_FIXED_HEADER_H_

