/*! \brief common client API
 *  \author Shylock Hg
 *  \date 2018-08-13
 *  \email tcath2s@gmail.com
 * */

#ifndef _MQTTOR_CLIENT_COMMON_H_
#define _MQTTOR_CLIENT_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "./mqttor_session.h"

/*! \brief connect to mqtt broker
 *  \param mq_sess mqttor session
 *  \param host mqtt broker ip address
 *  \param prot mqtt broker tcp port
 *  \retval mqtt error
 * */
int mqttor_client_connect(mqttor_session_t* mq_sess, const char* host,
                          int port);

/*! \brief disconnect to mqtt broker
 *  \param mq_sess mqttor session
 *  \retval mqtt error
 * */
int mqttor_client_disconnect(mqttor_session_t* mq_sess);

/*! \brief publish to broker
 *  \param mq_sess mqttor session
 *  \param topic topic to publish
 *  \param payload payload to publish -- user-define content
 *  \param qos QoS of publish , invalid QoS will be replaced by default QoS in
 * config \param retain is retain the publish \retval mqtt error
 * */
int mqttor_client_publish(mqttor_session_t* mq_sess, /*const*/ char* topic,
                          mqtt_attr_payload_t* payload, enum mqttor_QoS qos,
                          bool retain);

/*! \brief subscribe to broker
 *  \param mq_sess mqttor session
 *  \param sub subscribe pattern
 *  \param qos the requested QoS of subscribe
 *  \retval mqtt error
 * */
int mqttor_client_subscribe(mqttor_session_t* mq_sess, const char* sub,
                            enum mqttor_QoS qos);

/*! \brief unsubscribe to broker
 *  \param mq_sess mqttor session
 *  \param sub unsubscribe pattern
 *  \retval mqtt error
 * */
int mqttor_client_unsubscribe(mqttor_session_t* mq_sess, const char* sub);

/*! \brief send ping request and handle ping rasponse
 *  \param mq_sess mqttor session
 *  \retval mqtt error
 * */
int mqttor_client_pingreq(mqttor_session_t* mq_sess);

#ifdef __cplusplus
}
#endif

#endif  //!< _MQTTOR_CLIENT_COMMON_H_
