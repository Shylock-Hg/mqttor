/*! \brief a simple mqttor client
 *  \author Shylock Hg
 *  \date 2018-08-13
 *  \email tcath2s@gmail.com
 * */

/*! mqttor client command line usage
 *  synopsis -- mqttor_client <operation> [options]
 *  example --  mqttor_client connect [-i ip] [-p port]
 *          --  mqttor_client disconnect
 *          --  mqttor_client publish -t "topic" -m "message"
 *          --  mqttor_client subscribe -t "topic"
 *          --  mqttor_client unsubscribe -t "topic"
 * */

#include "../../inc/client/mqttor_client_common.h"
#include "../../inc/client/mqttor_client.h"

#include "../../inc/toolkit/mqtt_log.h"

int main(int argc, char * argv[]){

	mqttor_session_t * mq_sess = mqttor_session_new();
	assert(mq_sess);

	mq_sess->config->id_client = MQTTOR_CLIENT_ID;

	int err = 0;
	//< connect
	err = mqttor_client_connect(mq_sess, MQTTOR_BROKER_IP, MQTTOR_BROKER_PORT);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor client connect to broker (%s,%d) fail!\n",
				MQTTOR_BROKER_IP, MQTTOR_BROKER_PORT);
		return err;
	}

	//< publish
	mqtt_attr_payload_t * payload = mqtt_attr_payload_new(1024);
	mqtt_attr_payload_write_string(payload, "hello world!");
	err = mqttor_client_publish(mq_sess, "test/topic", payload, 
			MQTTOR_QoS_RESERVED, true);
	mqtt_attr_payload_release(payload);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client publish fail!\n"
			       );
		return err;
	}

	//< disconnect
	mqttor_client_disconnect(mq_sess);

	mqttor_session_release(mq_sess);

	return 0;
}

