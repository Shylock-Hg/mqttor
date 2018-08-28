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

#include <toolkit/mqtt_log.h>
#include <core/mqtt_packet.h>

#include <session/mqttor_session.h>

int mqttor_client_connect(mqttor_session_t * mq_sess, const char * host, 
		int port){
	assert(mq_sess);
	//assert(host);
	//assert(port);
	assert((host && port > 0) || (!host && port < 0));

	int err = 0;

	/*
	mq_sess->config->broker_ip = host ? 
		host : 
		mq_sess->config->broker_ip;
	mq_sess->config->broker_port = port > 0 ? 
		port : 
		mq_sess->config->broker_port;

	mqtt_log_printf(LOG_LEVEL_LOG, "Connect to (%s:%d)!\n", 
			mq_sess->config->broker_ip, 
			mq_sess->config->broker_port);
	*/

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
	mqtt_attr_packet_t * p_attr_connect = mqtt_attr_packet_new(1024);
	p_attr_connect->hdr.bits.type = MQTT_CTL_TYPE_CONNECT;
	p_attr_connect->attr_packet.connect.flag.bits.flag_clean_session = 
		mq_sess->config->clean_session;
	p_attr_connect->attr_packet.connect.flag.bits.flag_pwd = 
		mq_sess->config->pwd ? true : false;
	p_attr_connect->attr_packet.connect.flag.bits.flag_reserved = 0;
	p_attr_connect->attr_packet.connect.flag.bits.flag_user_name = 
		mq_sess->config->user ? true : false;
	p_attr_connect->attr_packet.connect.flag.bits.flag_w_QoS = 
		mq_sess->config->will_qos;
	p_attr_connect->attr_packet.connect.flag.bits.flag_w_flag = 
		mq_sess->config->will_message && mq_sess->config->will_topic ? 
		true : 
		false;
	p_attr_connect->attr_packet.connect.flag.bits.flag_w_retain = 
		mq_sess->config->will_retain;
	p_attr_connect->attr_packet.connect.keep_alive = 
		mq_sess->config->keep_alive;
	mqtt_buf_packet_t * p_buf_packet = NULL;

	assert(mq_sess->config->id_client);
	if(mq_sess->config->id_client)
		mqtt_attr_payload_write_string(p_attr_connect->payload, 
				mq_sess->config->id_client);
	if(mq_sess->config->will_topic && mq_sess->config->will_message)
		mqtt_attr_payload_write_string(p_attr_connect->payload,
				mq_sess->config->will_topic);
	if(mq_sess->config->will_message && mq_sess->config->will_topic)
		mqtt_attr_payload_write_string(p_attr_connect->payload,
				mq_sess->config->will_message);
	if(mq_sess->config->user)
		mqtt_attr_payload_write_string(p_attr_connect->payload,
				mq_sess->config->user);
	if(mq_sess->config->pwd)
		mqtt_attr_payload_write_string(p_attr_connect->payload,
				mq_sess->config->pwd);

	err = mqtt_pack_connect(p_attr_connect, 
			&p_buf_packet);
	if(0 > err){
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
	mqtt_attr_packet_release(p_attr_connect);

	//< wait for connack
	mqtt_buf_t * p_buf_connack = mqtt_buf_new(4);
	err = recv(mq_sess->socket, p_buf_connack->buf, p_buf_connack->len, 
			MSG_WAITALL);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor recv connack fail!\n");
		return -E_NET_XFER;
	}
	mqtt_attr_packet_t * p_attr_connack = NULL;
	err = mqtt_unpack_connack(p_buf_connack, &p_attr_connack);
	mqtt_buf_release(p_buf_connack);
	if(0 > err){
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
	mqtt_attr_packet_release(p_attr_connack);
	return err;
}

int mqttor_client_disconnect(mqttor_session_t * mq_sess){
	assert(mq_sess);

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}
	
	int err = 0;


	//< mqtt disconect
	mqtt_attr_packet_t * p_attr_packet = mqtt_attr_packet_new(0);
	p_attr_packet->hdr.bits.type = MQTT_CTL_TYPE_DISCONNECT;
	mqtt_buf_packet_t * p_buf_packet = NULL; 
	err = mqtt_pack_disconnect(p_attr_packet,
			&p_buf_packet);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor packet disconnect fail!\n");
		return err;
	}
	err = send(mq_sess->socket, p_buf_packet->buf, p_buf_packet->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send disconnect packet fail!\n");
		return -E_NET_CONN;
	}
	mqtt_buf_release(p_buf_packet);
	mqtt_attr_packet_release(p_attr_packet);

	//!< close socket
	close(mq_sess->socket);
	mq_sess->socket = -1;

	mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client disconnect!\n");

	return err;
}

int mqttor_client_publish(mqttor_session_t * mq_sess, const char * topic, 
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

	//< check QoS
	qos = MQTTOR_QoS_MONCE <= qos && qos < MQTTOR_QoS_RESERVED ? 
		qos : 
		mq_sess->config->qos;

	//!< mqtt publish
	mqtt_attr_packet_t * p_attr_publish = mqtt_attr_packet_new(
			payload->len_valid);
	assert(p_attr_publish);
	p_attr_publish->hdr.bits.type = MQTT_CTL_TYPE_PUBLISH;
	p_attr_publish->hdr.bits.DUP = false;
	p_attr_publish->hdr.bits.RETAIN = retain;
	p_attr_publish->hdr.bits.QoS = qos;
	p_attr_publish->attr_packet.publish.id_packet = mq_sess->id_packet++;
	p_attr_publish->attr_packet.publish.topic_name = topic;
	//mqtt_attr_payload_t * _payload = mqtt_attr_payload_new(payload->len_valid);
	memcpy(p_attr_publish->payload->buf, payload->buf, payload->len_valid);
	p_attr_publish->payload->len_valid = payload->len_valid;
	//p_attr_publish->payload = _payload;
	mqtt_buf_packet_t * p_buf_publish = NULL;
	err = mqtt_pack_publish(p_attr_publish, &p_buf_publish);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor pack publish fail!\n");
		return err;
	}
	mqtt_attr_packet_release(p_attr_publish);
	
	err = send(mq_sess->socket, p_buf_publish->buf, p_buf_publish->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor send disconnect packet fail!\n");
		return -E_NET_CONN;
	}
	mqtt_buf_release(p_buf_publish);

	//!< handle puback
	mqtt_buf_packet_t * p_buf_puback = mqtt_buf_new(4);
	mqtt_attr_packet_t * p_attr_puback = NULL;//mqtt_attr_packet_new(0);
	switch(qos){
		case MQTTOR_QoS_MONCE:
			//< no puback
			err = E_NONE;
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Mqttor publish ok!\n");
			break;
		case MQTTOR_QoS_LONCE:
			//< puback
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv puback fail!\n");
				break;
			}
			err = mqtt_unpack_puback(p_buf_puback, &p_attr_puback);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack puback fail!\n");
				break;
			}
			if(MQTT_CTL_TYPE_PUBACK == p_attr_puback->hdr.bits.type){
				mqtt_log_printf(LOG_LEVEL_LOG, 
						"Mqttor publish ok!\n");
				err = E_NONE;
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				err = -E_NET_CONN;
				break;
			}
			break;
		case MQTTOR_QoS_EONCE:
			//< pubrec
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv pubrec fail!=n");
				break;
			}
			err = mqtt_unpack_pubrec(p_buf_puback, &p_attr_puback);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack pubrec fail!\n");
				break;
			}
			if(MQTT_CTL_TYPE_PUBREC == p_attr_puback->hdr.bits.type){
				//mqtt_log_printf(LOG_LEVEL_LOG, 
						//"Mqttor publish receive ok!\n");
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				err = -E_NET_CONN;
				break;
			}
			//< pubrel
			p_attr_puback->hdr.bits.type = MQTT_CTL_TYPE_PUBREL;
			mqtt_buf_release(p_buf_puback);
			err = mqtt_pack_pubrel(p_attr_puback, &p_buf_puback);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor pack pubrel fail!\nn");
				break;
			}
			err = send(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor send pubrel fail!\n");
				err = -E_NET_XFER;
				break;
			}
			//< pubcomp
			err = recv(mq_sess->socket, p_buf_puback->buf, 
					p_buf_puback->len, 0);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor recv pubcomp fail!=n");
				err =  -E_NET_XFER;
				break;
			}
			mqtt_attr_packet_release(p_attr_puback);
			p_attr_puback = NULL;
			err = mqtt_unpack_pubcomp(p_buf_puback, &p_attr_puback);
			if(0 > err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor unpack pubcomp fail!\n");
				break;
			}
			if(MQTT_CTL_TYPE_PUBCOMP == p_attr_puback->hdr.bits.type){
				mqtt_log_printf(LOG_LEVEL_LOG, 
						"Mqttor publish ok!\n");
				err = E_NONE;
			}else{
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor publish fail!\n");
				err = -E_SESS_ACK;
				break;
			}
			
			break;
		default:
			mqtt_log_printf(LOG_LEVEL_ERR, 
					"Mqttor unkown qos `%d`!\n", qos);
			err = -E_SESS_ACK;

	}
	
	
	if(p_attr_puback){
		mqtt_attr_packet_release(p_attr_puback);
		p_attr_puback = NULL;
	}
	if(p_buf_puback){
		mqtt_buf_release(p_buf_puback);
		p_buf_puback = NULL;
	}

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
	if(0 > err){
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
	//mqtt_attr_packet_t * p_attr_suback = mqtt_attr_packet_new(
			//sizeof(uint8_t));
			//
	mqtt_attr_packet_t * p_attr_suback = NULL;
	err = mqtt_unpack_suback(p_buf_suback, &p_attr_suback);
	mqtt_buf_release(p_buf_suback);
	if(0 > err){
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
	if(0 > err){
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
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor recv unsuback fail!\n");
		return -E_NET_XFER;
	}
	mqtt_attr_packet_t *  p_attr_unsuback = NULL;
	err = mqtt_unpack_unsuback(p_buf_unsuback, &p_attr_unsuback);
	if(0 > err){
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

int mqttor_client_pingreq(mqttor_session_t * mq_sess){
	assert(mq_sess);

	if(0 > mq_sess->socket){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor invalid socket!\n");
		return -E_NET_SOCK;
	}

	int err = 0;

	//< send pingreq
	mqtt_attr_packet_t * p_attr_pingreq = mqtt_attr_packet_new(0);
	mqtt_buf_packet_t * p_buf_pingreq = NULL;
	assert(p_attr_pingreq);
	p_attr_pingreq->hdr.bits.type = MQTT_CTL_TYPE_PINGREQ;
	err = mqtt_pack_pingreq(p_attr_pingreq, &p_buf_pingreq);
	mqtt_attr_packet_release(p_attr_pingreq);
	p_attr_pingreq = NULL;
	assert(p_buf_pingreq);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client pack pingreq fail!\n");
		mqtt_attr_packet_release(p_attr_pingreq);
		if(p_buf_pingreq){
			mqtt_buf_release(p_buf_pingreq);
		}
		return err;
	}
	err = send(mq_sess->socket, p_buf_pingreq->buf, p_buf_pingreq->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client send pingreq fail!\n");
		mqtt_attr_packet_release(p_attr_pingreq);
		if(p_buf_pingreq){
			mqtt_buf_release(p_buf_pingreq);
		}
		return -E_NET_XFER;
	}
	mqtt_buf_release(p_buf_pingreq);
	p_buf_pingreq = NULL;

	//< wait pingresp
	mqtt_buf_packet_t * p_buf_pingresp = mqtt_buf_new(2);
	mqtt_attr_packet_t * p_attr_pingresp = NULL;
	assert(p_buf_pingresp);
	err = recv(mq_sess->socket, p_buf_pingresp->buf, p_buf_pingresp->len, 0);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client recv pingresp fail!\n");
		mqtt_buf_release(p_buf_pingresp);
		return -E_NET_XFER;
	}
	err = mqtt_unpack_pingresp(p_buf_pingresp, &p_attr_pingresp);
	mqtt_buf_release(p_buf_pingresp);
	p_buf_pingresp = NULL;
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client unpack pingresp fail!\n");
		mqtt_buf_release(p_buf_pingresp);
		if(p_attr_pingresp){
			mqtt_attr_packet_release(p_attr_pingresp);
		}
		return err;
	}
	if(MQTT_CTL_TYPE_PINGRESP == p_attr_pingresp->hdr.bits.type){
		err = E_NONE;
	}else{
		err = -E_SESS_ACK;
	}
	mqtt_attr_packet_release(p_attr_pingresp);
	p_attr_pingresp = NULL;
	return err;
}

