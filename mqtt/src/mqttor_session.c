/*! \brief mqttro session
 *  \author Shylock Hg
 *  \date 2018-08-10
 *  \email tcath2s@gmail.com
 * */
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <sys/socket.h>

#include "../inc/mqtt_packet.h"
#include "../inc/mqtt_log.h"

#include "../inc/mqttor_session.h"


void mqttor_config_deinit(mqttor_config_t * mq_config){

	//static const char * MQTTOR_BROKER_IP = "37.187.106.16";//"127.0.0.1"
	//static const int MQTTOR_BROKER_PORT  = 1883;

	static const char * MQTTOR_CLIENT_ID = "shylock-archlinux";

	assert(mq_config);

	mq_config->DUP = false;  //!< not resent
	mq_config->RETAIN = true;   //!< broker saving
	mq_config->qos = MQTTOR_QoS_MONCE;  //!< most once

	//mq_config->broker_ip = MQTTOR_BROKER_IP;
	//mq_config->broker_port = MQTTOR_BROKER_PORT;

	mq_config->clean_session = true;  //!< new session
	mq_config->id_client = MQTTOR_CLIENT_ID;  //!< unknow client identifier
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

	enum mqtt_ctl_type type = MQTT_BUF_SEGMENT_EVAL(buf_packet->buf[0], 
			MQTT_CTL_TYPE_Msk, MQTT_CTL_TYPE_OFFSET);

	mqtt_attr_packet_t * p_attr_publish = NULL;
	mqtt_attr_packet_t * p_attr_pubcomp = NULL;
	switch(type){
		case MQTT_CTL_TYPE_PUBLISH:
			err = mqtt_unpack_publish(buf_packet, &p_attr_publish);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, "error:%d\n", err);
				if(p_attr_publish)
					mqtt_attr_packet_release(p_attr_publish);
				return err;
			}
			
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Receive publish message from broker:\n");
			mqtt_log_print_buf(LOG_LEVEL_LOG, p_attr_publish->payload->buf, 
					p_attr_publish->payload->len);

			//char content[1024] = {0};
			//err = mqtt_attr_payload_read_string(p_attr_publish->payload, 
			//content, sizeof(content));
			//if(0 > err){
				//mqtt_log_printf(LOG_LEVEL_ERR, 
						//"Mqttor client read string from payload fail!\n");
			//}
			//mqtt_log_printf(LOG_LEVEL_LOG, "Publish -- %s\n", 
			//p_attr_publish->payload->buf);


			//< check QoS of puback or pubrec
			mqtt_attr_packet_t * p_attr_puback = NULL;
			mqtt_attr_packet_t * p_attr_pubrec = NULL;
			switch(p_attr_publish->hdr.bits.QoS){
				case MQTTOR_QoS_MONCE:
					/* no ack */
					break;
				case MQTTOR_QoS_LONCE:
					/* puback */
					p_attr_puback = mqtt_attr_packet_new(0);
					mqtt_buf_packet_t * p_buf_puback = NULL;
					p_attr_puback->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
					p_attr_puback->attr_packet.puback.id_packet = 
						mq_sess->id_packet++;
					err = mqtt_pack_puback(p_attr_puback, &p_buf_puback);
					mqtt_attr_packet_release(p_attr_puback);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client pack puback fail!\n");
					}
					err = send(mq_sess->socket, p_buf_puback->buf, 
							p_buf_puback->len, 0);
					mqtt_buf_release(p_buf_puback);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client send puback fail!\n");
					}
					break;
				case MQTTOR_QoS_EONCE:
					/* pubrec */
					p_attr_pubrec = mqtt_attr_packet_new(0);
					mqtt_buf_packet_t * p_buf_pubrec = NULL;
					p_attr_pubrec->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
					p_attr_pubrec->attr_packet.pubrec.id_packet = 
						mq_sess->id_packet++;
					err = mqtt_pack_pubrec(p_attr_pubrec, &p_buf_pubrec);
					mqtt_attr_packet_release(p_attr_pubrec);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client pack pubrec fail!\n");
					}
					err = send(mq_sess->socket, p_buf_pubrec->buf, 
							p_buf_pubrec->len, 0);
					mqtt_buf_release(p_buf_pubrec);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client send pubrec fail!\n");
					}

					/* pubrel */
					mqtt_buf_t * p_buf_pubrel = mqtt_buf_new(
							MQTT_FIXED_PACKET_LEN_PUBREL);
					mqtt_attr_packet_t * p_attr_pubrel = NULL;
					err = recv(mq_sess->socket, p_buf_pubrel->buf,
							p_buf_pubrel->len, 0);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client recv pubrel fail!\n");
					}
					err = mqtt_unpack_pubrel(p_buf_pubrel, &p_attr_pubrel);
					mqtt_buf_release(p_buf_pubrel);
					if(MQTT_CTL_TYPE_PUBREL != p_attr_pubrel->hdr.bits.type){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client recv non pubrel!\n");
					}
					mqtt_attr_packet_release(p_attr_pubrel);

					/* pubcomp */
					p_attr_pubcomp = mqtt_attr_packet_new(0);
					mqtt_buf_packet_t * p_buf_pubcomp = NULL;
					p_attr_pubcomp->hdr.bits.type = MQTT_CTL_TYPE_PUBACK;
					p_attr_pubcomp->attr_packet.pubcomp.id_packet = 
						mq_sess->id_packet++;
					err = mqtt_pack_pubcomp(p_attr_pubcomp, &p_buf_pubcomp);
					mqtt_attr_packet_release(p_attr_pubcomp);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client pack pubcomp fail!\n");
					}
					err = send(mq_sess->socket, p_buf_pubcomp->buf, 
							p_buf_pubcomp->len, 0);
					mqtt_buf_release(p_buf_pubcomp);
					if(0 > err){
						mqtt_log_printf(LOG_LEVEL_ERR, 
								"Mqttor client send pubcomp fail!\n");
					}
						break;
				default:
					mqtt_log_printf(LOG_LEVEL_WARN, "Invalid QoS `%d`!\n", 
							p_attr_publish->hdr.bits.QoS);
			}

			free(p_attr_publish->attr_packet.publish.topic_name);
			mqtt_attr_packet_release(p_attr_publish);
			p_attr_publish = NULL;

			break;
		default:
			mqtt_log_printf(LOG_LEVEL_WARN, "Invalid packet `%d`!\n", type);
	}

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

	mq_sess->is_connected = false;

	mq_sess->id_packet = 0;

	return mq_sess;
}

void mqttor_session_release(mqttor_session_t * mq_sess){
	mqttor_config_release(mq_sess->config);
	free(mq_sess);
}

