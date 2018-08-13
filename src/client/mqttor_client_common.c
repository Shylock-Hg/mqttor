/*! \brief common client API
 *  \author Shylock Hg
 *  \date 2018-08-13
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../../inc/toolkit/mqtt_log.h"
#include "../../inc/core/mqtt_packet.h"

#include "../../inc/session/mqttor_session.h"

int mqttor_client_connect(mqttor_session_t * mq_sess, const char * host, 
		int port){
	assert(mq_sess);
	assert(host);

	int err = 0;

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);

	//< initialize socket
	if(0 > mq_sess->socket){  //!< invalid socket
		if(0 > (mq_sess->socket = socket(addr.sin_family,
						SOCK_STREAM,IPPROTO_TCP))){
			mqtt_log_printf(LOG_LEVEL_ERR, 
					"Mqttor create socket fail!\n");
			return -E_NET_SOCK;
		}
	}

	//< tcp connect
	if(0 != connect(mq_sess->socket, (struct sockaddr*)&addr, sizeof(addr))){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor tcp connect fail!\n");
		return -E_NET_CONN;
	}
	
	//< mqtt connect
	mqtt_attr_packet_t * p_attr_packet = mqtt_attr_packet_new(1024);
	p_attr_packet->hdr.bits.type = MQTT_CTL_TYPE_CONNECT;
	p_attr_packet->attr_packet.connect.flag.bits.flag_clean_session = 
		mq_sess->config->clean_session;
	p_attr_packet->attr_packet.connect.flag.bits.flag_pwd = 
		mq_sess->config->pwd ? true : false;
	p_attr_packet->attr_packet.connect.flag.bits.flag_reserved = 0;
	p_attr_packet->attr_packet.connect.flag.bits.flag_user_name = 
		mq_sess->config->user ? true : false;
	p_attr_packet->attr_packet.connect.flag.bits.flag_w_QoS = 
		mq_sess->config->will_qos;
	p_attr_packet->attr_packet.connect.flag.bits.flag_w_flag = 
		mq_sess->config->will_message && mq_sess->config->will_topic ? 
		true : 
		false;
	p_attr_packet->attr_packet.connect.flag.bits.flag_w_retain = 
		mq_sess->config->will_retain;
	p_attr_packet->attr_packet.connect.keep_alive = 
		mq_sess->config->keep_alive;
	mqtt_buf_packet_t * p_buf_packet = NULL;

	assert(mq_sess->config->id_client);
	if(mq_sess->config->id_client)
		mqtt_attr_payload_write_string(p_attr_packet->payload, 
				mq_sess->config->id_client);
	if(mq_sess->config->will_topic && mq_sess->config->will_message)
		mqtt_attr_payload_write_string(p_attr_packet->payload,
				mq_sess->config->will_topic);
	if(mq_sess->config->will_message && mq_sess->config->will_topic)
		mqtt_attr_payload_write_string(p_attr_packet->payload,
				mq_sess->config->will_message);
	if(mq_sess->config->user)
		mqtt_attr_payload_write_string(p_attr_packet->payload,
				mq_sess->config->user);
	if(mq_sess->config->pwd)
		mqtt_attr_payload_write_string(p_attr_packet->payload,
				mq_sess->config->pwd);

	err = mqtt_pack_connect(p_attr_packet, 
			&p_buf_packet);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor packet connect fail!\n");
		return err;
	}
	err = send(mq_sess->socket, p_buf_packet->buf, p_buf_packet->len, 0);
	mqtt_buf_release(p_buf_packet);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send connect packet fail!\n");
		return -E_NET_CONN;
	}
	//mqtt_buf_release(p_buf_packet);
	mqtt_attr_packet_release(p_attr_packet);

	//< wait for connack
	mqtt_buf_t * mq_rece = mqtt_buf_new(4);
	err = recv(mq_sess->socket, mq_rece->buf, mq_rece->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor recv connack fail!\n");
		return -E_NET_XFER;
	}
	mqtt_attr_packet_t * p_attr_connack = NULL;
	err = mqtt_unpack_connack(mq_rece, &p_attr_connack);
	mqtt_buf_release(mq_rece);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor unpack connack fail!\n");
		return err;
	}
	if(MQTT_CTL_TYPE_CONNACK == p_attr_connack->hdr.bits.type){
		mqtt_log_printf(LOG_LEVEL_LOG, 
				"Mqttor connect to broker ok!\n");
		err = E_NONE;
	}else{
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor connect to broker fail!\n");
		err = E_NET_CONN;
	}
	mqtt_attr_packet_release(p_attr_packet);
	return err;
}

int mqttor_client_disconnect(mqttor_session_t * mq_sess){
	assert(mq_sess);

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}
	
	int err = 0;

	/*
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	*/

	//< mqtt disconect
	mqtt_attr_packet_t * p_attr_packet = mqtt_attr_packet_new(0);
	p_attr_packet->hdr.bits.type = MQTT_CTL_TYPE_DISCONNECT;
	mqtt_buf_packet_t * p_buf_packet = NULL; 
	err = mqtt_pack_disconnect(p_attr_packet,
			&p_buf_packet);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor packet disconnect fail!\n");
		return err;
	}
	err = send(mq_sess->socket, p_buf_packet->buf, p_buf_packet->len, 0);
	if(err != p_buf_packet->len){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send disconnect packet fail!\n");
		return -E_NET_CONN;
	}
	mqtt_buf_release(p_buf_packet);
	mqtt_attr_packet_release(p_attr_packet);

	//!< close socket
	close(mq_sess->socket);
	mq_sess->socket = -1;

	return E_NONE;
}

int mqttor_client_publish(mqttor_session_t * mq_sess, /*const*/ char * topic, 
		mqtt_attr_payload_t * payload,
		enum mqttor_QoS qos,
		bool retain){
	assert(mq_sess);
	assert(topic);
	assert(payload);

	int err = 0;

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}

	//!< mqtt publish
	mqtt_attr_packet_t * p_attr_publish = mqtt_attr_packet_new(
			payload->len);
	assert(p_attr_publish);
	p_attr_publish->hdr.bits.type = MQTT_CTL_TYPE_PUBLISH;
	p_attr_publish->hdr.bits.DUP = false;
	p_attr_publish->hdr.bits.RETAIN = retain;
	p_attr_publish->hdr.bits.QoS = 
		MQTTOR_QoS_MONCE <= qos && qos < MQTTOR_QoS_RESERVED ? 
		qos : 
		mq_sess->config->qos;
	p_attr_publish->attr_packet.publish.id_packet = mq_sess->id_packet++;
	p_attr_publish->attr_packet.publish.topic_name = topic;
	mqtt_attr_payload_t * _payload = mqtt_attr_payload_new(payload->len);
	memcpy(_payload->buf, payload->buf, payload->len_valid);
	_payload->len = payload->len;
	_payload->len_valid = payload->len_valid;
	p_attr_publish->payload = _payload;
	mqtt_buf_packet_t * p_buf_publish = NULL;
	err = mqtt_pack_publish(p_attr_publish, &p_buf_publish);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor pack publish fail!\n");
		return err;
	}
	mqtt_attr_packet_release(p_attr_publish);
	
	err = send(mq_sess->socket, p_buf_publish->buf, p_buf_publish->len, 0);
	if(err != p_buf_publish->len){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send disconnect packet fail!\n");
		return -E_NET_CONN;
	}
	mqtt_buf_release(p_buf_publish);

	//!< handle puback
	mqtt_buf_packet_t * p_buf_puback = mqtt_buf_new(4);
	mqtt_attr_packet_t * p_attr_puback = mqtt_attr_packet_new(0);
	switch(qos){
		case MQTTOR_QoS_MONCE:
			//< no puback
			err = E_NONE;
			break;
		case MQTTOR_QoS_LONCE:
			//< puback
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv puback fail!=n");
				return err;
			}
			err = mqtt_unpack_puback(p_buf_puback, &p_attr_puback);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack puback fail!\n");
				return err;
			}
			if(MQTT_CTL_TYPE_PUBACK == p_attr_puback->hdr.bits.type){
				mqtt_log_printf(LOG_LEVEL_LOG, 
						"Mqttor publish ok!\n");
				return E_NONE;
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				return E_NET_CONN;
			}
			break;
		case MQTTOR_QoS_EONCE:
			//< pubrec
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv pubrec fail!=n");
				return err;
			}
			err = mqtt_unpack_pubrec(p_buf_puback, &p_attr_puback);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack puback fail!\n");
				return err;
			}
			if(MQTT_CTL_TYPE_PUBREC == p_attr_puback->hdr.bits.type){
				mqtt_log_printf(LOG_LEVEL_LOG, 
						"Mqttor publish ok!\n");
				return E_NONE;
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				return E_NET_CONN;
			}
			//< pubrel
			p_attr_puback->hdr.bits.type = MQTT_CTL_TYPE_PUBREL;
			err = mqtt_pack_pubrel(p_attr_puback, &p_buf_puback);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor pack pubrel fail!\nn");
				return err;
			}
			err = send(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor send pubrel fail!\n");
				return err;
			}
			//< pubcomp
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv pubcomp fail!=n");
				return err;
			}
			err = mqtt_unpack_pubcomp(p_buf_puback, &p_attr_puback);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack pubcomp fail!\n");
				return err;
			}
			if(MQTT_CTL_TYPE_PUBCOMP == p_attr_puback->hdr.bits.type){
				mqtt_log_printf(LOG_LEVEL_LOG, 
						"Mqttor publish ok!\n");
				return E_NONE;
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				return E_NET_CONN;
			}
			
			break;
		default:
			mqtt_log_printf(LOG_LEVEL_ERR, 
					"Mqttor unkown qos `%d`!\n", qos);

	}
	mqtt_attr_packet_release(p_attr_puback);
	mqtt_buf_release(p_buf_puback);
	
	return err;
}

int mqttor_client_subscribe(mqttor_session_t * mq_sess, const char * sub, 
		enum mqttor_QoS qos){
	assert(mq_sess);

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}

	//< subscribe
	int err = 0;
	size_t len_sub = strlen(sub);
	mqtt_attr_packet_t * p_attr_subscribe = mqtt_attr_packet_new(
			len_sub+MQTT_BUF_STR_MAX_BYTE+sizeof(uint8_t));
	p_attr_subscribe->hdr.bits.type = MQTT_CTL_TYPE_SUBSCRIBE;
	p_attr_subscribe->attr_packet.subscribe.id_packet = 
		mq_sess->id_packet++;
	mqtt_attr_payload_write_string(p_attr_subscribe->payload, sub);
	mqtt_attr_payload_write_byte(p_attr_subscribe->payload,
			MQTTOR_QoS_MONCE <= qos && qos < MQTTOR_QoS_RESERVED ? 
			qos : 
			mq_sess->config->qos);
	mqtt_buf_packet_t * p_buf_subscribe = NULL;
	err = mqtt_pack_subscribe(p_attr_subscribe, &p_buf_subscribe);
	mqtt_attr_packet_release(p_attr_subscribe);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor pack subscribe fail!\n");
		return err;
	}

	err = send(mq_sess->socket, p_buf_subscribe->buf, p_buf_subscribe->len,
			0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send subscribe fail!=\n");
		return err;
	}

	mqtt_buf_release(p_buf_subscribe);

	//!< handle suback
	mqtt_buf_packet_t * p_buf_suback = mqtt_buf_new(5);
	err = recv(mq_sess->socket, p_buf_suback->buf, p_buf_suback->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor recv suback fail!\n");
		return err;
	}
	mqtt_attr_packet_t * p_attr_suback = mqtt_attr_packet_new(
			sizeof(uint8_t));
	err = mqtt_unpack_suback(p_buf_suback, &p_attr_suback);
	mqtt_buf_release(p_buf_suback);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor unpack suback fail!\n");
		return err;
	}
	if(MQTT_CTL_TYPE_SUBACK == p_attr_suback->hdr.bits.type){
		err = E_NONE;
	}else{
		err = -E_SESS_ACK;
	}
	mqtt_attr_packet_release(p_attr_suback);
	
	return err;
}

int mqttor_client_unsubscribe(mqttor_session_t * mq_sess, const char * sub){
	assert(mq_sess);
	assert(sub);

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}
	
	//< unsubscribe
	int err = 0;
	size_t len_sub = strlen(sub);
	mqtt_attr_packet_t * p_attr_unsubscribe = mqtt_attr_packet_new(
			len_sub+MQTT_BUF_STR_MAX_BYTE);
	p_attr_unsubscribe->hdr.bits.type = MQTT_CTL_TYPE_UNSUBSCRIBE;
	p_attr_unsubscribe->attr_packet.unsubscribe.id_packet = 
		mq_sess->id_packet;
	mqtt_attr_payload_write_string(p_attr_unsubscribe->payload, sub);
	mqtt_buf_packet_t * p_buf_unsubscribe = NULL;
	err = mqtt_pack_unsubscribe(p_attr_unsubscribe, &p_buf_unsubscribe);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor pack unsubscribe fail!\n");
		return err;
	}
	mqtt_attr_packet_release(p_attr_unsubscribe);
	err = send(mq_sess->socket, p_buf_unsubscribe->buf, 
			p_buf_unsubscribe->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send unsubscribe fail!\n");
		return -E_NET_XFER;
	}
	mqtt_buf_release(p_buf_unsubscribe);

	//< unsuback
	mqtt_buf_t * p_buf_unsuback =  mqtt_buf_new(4);
	err = recv(mq_sess->socket, p_buf_unsuback->buf, p_buf_unsuback->len, 
			0);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor recv unsuback fail!\n");
		return -E_NET_XFER;
	}
	mqtt_attr_packet_t *  p_attr_unsuback = NULL;
	err = mqtt_unpack_unsuback(p_buf_unsuback, &p_attr_unsuback);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor unpack unsuback fail!\n");
		return err;
	}
	mqtt_buf_release(p_buf_unsuback);
	if(MQTT_CTL_TYPE_UNSUBACK == p_attr_unsuback->hdr.bits.type){
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor unsubscribe ok!\n");
		err = E_NONE;
	}else{
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor unsubscribe fail!\n");
		err = -E_SESS_ACK;
	}
	return err;
}

