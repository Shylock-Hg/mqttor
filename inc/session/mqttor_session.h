/*! \brief mqttor session 
 *  \author Shylock Hg
 *  \date 2018-08-10
 *  \email tcath2s@gmail.com
 * */

#ifndef _MQTTOR_SESSION_H_
#define _MQTTOR_SESSION_H_

#ifdef __cplusplus
	extern "C" {
#endif
	
#include <stdbool.h>

#include "../core/mqtt_packet.h"

enum mqttor_protocol {
	MQTTOR_V311 = 4
};

enum mqttor_QoS {
	MQTTOR_QoS_MONCE,
	MQTTOR_QoS_LONCE,
	MQTTOR_QoS_EONCE,
	MQTTOR_QoS_RESERVED
};

/*! \brief mqttor configuration of client and broker
 * */
typedef struct mqttor_config {
	/* for client */
	enum mqttor_protocol protocol; //!< connect
	char * user;  //!< connect
	char * pwd;  //!< connect
	bool will_retain;  //!< connect
	enum mqttor_QoS will_qos;  //!< connect
	bool clean_session;  //!< connect
	uint16_t keep_alive;  //!< connect
	char * id_client;  //!< connect
	char * will_topic;  //!< connect
	char * will_message;  //!< connect
	/* for client */
	bool DUP;  //!< publish
	enum mqttor_QoS qos;  //!< publish
	bool RETAIN;  //!< publish


} mqttor_config_t;

/*! \brief create a mqttor configuration object
 *  \retval pointer of created mqttor configuraton object
 * */
mqttor_config_t * mqttor_config_new(void);
/*! \brief release a mqttor configuration object
 *  \parma mq_config pointer to mqttor configuration object to release
 * */
void mqttor_config_release(mqttor_config_t * mq_config);
/*! \brief deinitialize the mqttor configuration object
 *  \parma mq_config pointer to mqttor configuration object to release
 * */
void mqttor_config_deinit(mqttor_config_t * mq_config);


/*! \brief mqttor session structure to store all state of mqtt behavior
 * */
typedef struct mqttor_session {
	//< configuration
	mqttor_config_t * config;

	int socket;

	uint16_t id_packet;  //!< record message id

	//< callback functions
	int (*on_publish)(struct mqttor_session * mq_sess, 
			mqtt_buf_packet_t * buf_packet);  //!< client and broker


} mqttor_session_t;

/*! \brief create a mqttor session object
 *  \retval the created mqttor session object
 * */
mqttor_session_t * mqttor_session_new(void);
/*! \brief release a mqttor session object
 *  \param mq_sess the mqttor session object to release
 * */
void mqttor_session_release(mqttor_session_t * mq_sess);


#ifdef __cplusplus
	}
#endif

#endif  //!< _MQTTOR_SESSION_H_

