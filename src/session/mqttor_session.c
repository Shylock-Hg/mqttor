/*! \brief mqttro session
 *  \author Shylock Hg
 *  \date 2018-08-10
 *  \email tcath2s@gmail.com
 * */
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "../../inc/core/mqtt_packet.h"
#include "../../inc/toolkit/mqtt_log.h"

#include "../../inc/session/mqttor_session.h"

void mqttor_config_deinit(mqttor_config_t * mq_config){
	assert(mq_config);

	mq_config->DUP = false;  //!< not resent
	mq_config->RETAIN = true;   //!< broker saving
	mq_config->qos = MQTTOR_QoS_MONCE;  //!< most once

	mq_config->clean_session = true;  //!< new session
	mq_config->id_client = NULL;  //!< unknow client identifier
	mq_config->keep_alive = 32;  //!< 32 seconds keep alive
	mq_config->protocol = MQTTOR_V311;  //!< mqtt version 3.1.1
	mq_config->pwd = NULL;  //!< unknown password
	mq_config->user = NULL;  //!< unkown user name
	mq_config->will_message = NULL;  //!< unkown will message
	mq_config->will_qos = MQTTOR_QoS_MONCE;  //!< will most once
	mq_config->will_retain = false;//true;  //!< will broker saving
	mq_config->will_topic = NULL;  //!< will topic
}

mqttor_config_t * mqttor_config_new(void){
	mqttor_config_t * mq_config = malloc(sizeof(mqttor_config_t));
	
	mqttor_config_deinit(mq_config);

	return mq_config;
}

void mqttor_config_release(mqttor_config_t * mq_config){
	free(mq_config);
}


/*! \brief default on publish callback function
 *  \param mq_sess mqttor session 
 *  \retval error 
 * */
int mqttor_session_on_publish(mqttor_session_t * mq_sess, 
		mqtt_buf_packet_t * buf_packet){
	int err = 0;
	mqtt_attr_packet_t * attr_packet = NULL;
	err = mqtt_unpack_publish(buf_packet, &attr_packet);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "error:%d\n", err);
		if(attr_packet)
			mqtt_attr_packet_release(attr_packet);
		return err;
	}
	
	mqtt_log_printf(LOG_LEVEL_LOG, "Receive publish message from broker:\n");
	mqtt_log_print_buf(LOG_LEVEL_LOG, attr_packet->payload->buf, 
			attr_packet->payload->len);

	mqtt_attr_packet_release(attr_packet);

	return E_NONE;
}


void mqttor_session_deinit(mqttor_session_t * mq_sess){
	assert(mq_sess);

	mqttor_config_deinit(mq_sess->config);

	mq_sess->on_publish = mqttor_session_on_publish;

	mq_sess->socket = -1;

	mq_sess->id_packet = 0;
}

mqttor_session_t * mqttor_session_new(void){
	mqttor_session_t * mq_sess = malloc(sizeof(mqttor_session_t));

	mq_sess->config = mqttor_config_new();
	mq_sess->on_publish = mqttor_session_on_publish;

	mq_sess->socket = -1;

	mq_sess->id_packet = 0;

	return mq_sess;
}

void mqttor_session_release(mqttor_session_t * mq_sess){
	mqttor_config_release(mq_sess->config);
	free(mq_sess);
}

