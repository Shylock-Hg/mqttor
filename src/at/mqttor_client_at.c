/*! \brief at command implement of mqttor client
 *  \author Shylock Hg
 *  \date 2018-08-20
 *  \email tcath2s@gmail.com
 * */

#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <at_table.h>
#include <at_param.h>
#include <at_command.h>
#include <at_xrecord.h>
#include <at_fsm.h>
#include <client/mqttor_client_common.h>
#include <toolkit/mqtt_log.h>


#define REPL

/********** mqttor client options **********/
static /*const*/ char * topic = "test/topic";
static const char * message = "hello world!";

static enum mqttor_QoS qos = MQTTOR_QoS_MONCE;

/* eclipse.org:1883 -- mqtt with tcp */
static const char * host = "127.0.0.1";//"37.187.106.16";
static int port  = 1883;

static bool is_connected = false;  //!< mqttor session connection
static mqttor_session_t * mq_sess = NULL;  //!< mqttor session instance

static int at_cmd_MQCONF_set_handler(char * parameter){
	assert(parameter);

	if(NULL == mq_sess){
		return -1;
	}

	//< parse parameter -- AT+MQCONF=KEY,VALUE
	at_cmd_params_t * p_at_params = at_cmd_params_new(parameter, 2, 2);
	assert(p_at_params);
	if(0 == strcmp("username", p_at_params->params[0]->param)){
		mq_sess->config->user = p_at_params->params[1]->param;
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client user is `%s`!\n", 
				mq_sess->config->user);
		return 0;  //!< not release string argument value
	}else if(0 == strcmp("password", p_at_params->params[0]->param)){
		mq_sess->config->pwd = p_at_params->params[1]->param;
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client pwd is `%s`!\n", 
				mq_sess->config->pwd);
		return 0;  //!< not release string argument value
	}else if(0 == strcmp("will_retain", p_at_params->params[0]->param)){
		mq_sess->config->will_retain = !!atoi(p_at_params->params[1]->param);
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client will_retain is `%s`!\n", 
				mq_sess->config->will_retain ? "true" : "false");
	}else if(0 == strcmp("will_qos", p_at_params->params[0]->param)){
		enum mqttor_QoS qos = atoi(p_at_params->params[1]->param);
		mq_sess->config->will_qos = 
			(qos >= MQTTOR_QoS_MONCE) && (qos < MQTTOR_QoS_RESERVED) ? 
			qos : mq_sess->config->will_qos;

		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client will_qos is `%d`!\n", 
				mq_sess->config->will_qos);
	}else if(0 == strcmp("clean_session", p_at_params->params[0]->param)){
		mq_sess->config->clean_session = !!atoi(p_at_params->params[1]->param);
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client clean_session is `%s`!\n",
				mq_sess->config->clean_session ? "true" : "false");
	}else if(0 == strcmp("keepalive", p_at_params->params[0]->param)){
		mq_sess->config->keep_alive = atoi(p_at_params->params[1]->param);
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client keepalive is `%d`!\n",
				mq_sess->config->keep_alive);
	}else if(0 == strcmp("id_client", p_at_params->params[0]->param)){
		mq_sess->config->id_client = p_at_params->params[1]->param;
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client id_client is `%s`!\n", 
				mq_sess->config->id_client);
		return 0;  //!< not release string argument value
	}else if(0 == strcmp("will_topic", p_at_params->params[0]->param)){
		mq_sess->config->will_topic = p_at_params->params[1]->param;
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client will_topic is `%s`!\n",
				mq_sess->config->will_topic);
		return 0;  //!< not release string argument value
	}else if(0 == strcmp("will_message", p_at_params->params[0]->param)){
		mq_sess->config->will_message = p_at_params->params[1]->param;
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client will_message is `%s`!\n", 
				mq_sess->config->will_message);
		return 0;  //!< not release string argument value
	}
	
	else if(0 == strcmp("RETAIN", p_at_params->params[0]->param)){
		mq_sess->config->RETAIN = !!atoi(p_at_params->params[1]->param);
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client RETAIN is `%s`!\n",
				mq_sess->config->RETAIN ? "true" : "false");
	}else if(0 == strcmp("DUP", p_at_params->params[0]->param)){
		mq_sess->config->DUP = !!atoi(p_at_params->params[1]->param);
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client DUP is `%s`!\n",
				mq_sess->config->DUP ? "true" : "false");
	}else if(0 == strcmp("qos", p_at_params->params[0]->param)){
		enum mqttor_QoS qos = MQTTOR_QoS_RESERVED;
		qos = atoi(p_at_params->params[1]->param);
		mq_sess->config->qos = 
			(qos >= MQTTOR_QoS_MONCE)&&(qos < MQTTOR_QoS_RESERVED) ?
			qos : mq_sess->config->qos;	
		mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor client qos is `%d`!\n",
				mq_sess->config->qos);
	}
	
	else{
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client MQCONF key `%s` error!\n", 
				p_at_params->params[0]->param);
		return -1;
	}
	

	//!< don't release string parameter for used later
	at_cmd_params_release(p_at_params);  
	

	return 0;
}

static int at_cmd_MQCONF_read_handler(void){
	if(NULL == mq_sess){
		return -1;
	}

	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:id_client,%s\n",
			mq_sess->config->id_client);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:username,%s\n", 
			mq_sess->config->user);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:password,%s\n",
			mq_sess->config->pwd);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:will_retain,%s\n",
			mq_sess->config->will_retain ? "true" : "false");
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:will_qos,%d\n",
			mq_sess->config->will_qos);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:clean_session,%s\n",
			mq_sess->config->clean_session ? "true" : "false");
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:keepalive,%d\n",
			mq_sess->config->keep_alive);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:will_topic,%s\n",
			mq_sess->config->will_topic);
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:will_message,%s\n",
			mq_sess->config->will_message);

	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:DUP,%s\n",
			mq_sess->config->DUP ? "true" : "false");
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:RETAIN,%s\n",
			mq_sess->config->RETAIN ? "true" : "false");
	mqtt_log_printf(LOG_LEVEL_LOG, "+MQCONF:qos,%d\n",
			mq_sess->config->qos);
	
	return 0;
}


static int at_cmd_MQCON_set_handler(char * parameter){
	assert(parameter);
	assert(mq_sess);

	int err = 0;

	if(is_connected){
		return 0;
	}
	if(NULL == mq_sess){
		return -1;
	}

	//< parse parameter -- AT+MQCON=host,port
	at_cmd_params_t * p_at_params = at_cmd_params_new(parameter, 2, 2);
	assert(p_at_params);

	host = p_at_params->params[0]->param;
	port = atoi(p_at_params->params[1]->param);


	//< connect
	err = mqttor_client_connect(mq_sess, 
			host, port);
	if(0 > err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor client connect to broker (%s,%d) fail!\n",
				host/*MQTTOR_BROKER_IP*/, 
				port/*MQTTOR_BROKER_PORT*/);
	}

	//< release params 
	at_cmd_params_release(p_at_params);

	is_connected = (0 == err ? true : is_connected);

	return err;
}


static int at_cmd_MQDSC_exec_handler(void){
	//mqtt_log_printf(LOG_LEVEL_LOG, "MQDSC");
	//< 
	assert(mq_sess);
	
	if(!is_connected){
		return 0;
	}
	if(NULL == mq_sess){
		return -1;
	}

	int err = mqttor_client_disconnect(mq_sess);
	is_connected = false;

	return err;
}

static bool runcond = true;  //!< subscribe run loop flag
/*  \brief SIGINT handler
 *  \param signum signal number
 * */
void sighandler(int signum){
	if(SIGINT == signum || SIGPIPE == signum){
		runcond = false;
	}
}

static int at_cmd_MQSUB_set_handler(char * parameter){
	int err = 0;

	assert(parameter);

	if(!is_connected){  //!< not connected
		return -1;
	}
	if(NULL == mq_sess){
		return -1;
	}

	//< parse parameter -- AT+MQSUB=topic,requested_qos
	at_cmd_params_t * p_at_params = at_cmd_params_new(parameter, 1, 2);
	if(1 == p_at_params->count){  //!< AT+MQSUB=topic
		topic = p_at_params->params[0]->param;
	}else if(2 == p_at_params->count){  //!< AT+MQSUB=topic,requested_qos
		topic = p_at_params->params[0]->param;
		qos   = atoi(p_at_params->params[1]->param);
	}else{  //!< error
		at_cmd_params_release(p_at_params);
		return -1;
	}
	
	//< subscribe
	err = mqttor_client_subscribe(mq_sess, topic, qos);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, 
				"Mqttor client subscribe `%s` fail!\n", topic);
	}


	at_cmd_params_release(p_at_params);

	return err;
}


static int at_cmd_MQPUB_set_handler(char * parameter){
	int err = 0;

	assert(parameter);

	if(!is_connected){
		return -1;
	}
	if(NULL == mq_sess){
		return -1;
	}

	//< parse parameter -- AT+MQPUB=topic,message,qos
	at_cmd_params_t * p_at_params = at_cmd_params_new(parameter, 2, 3);
	assert(p_at_params);
	
	if(2 == p_at_params->count){  //!< AT+MQPUB=topic,message
		topic   = p_at_params->params[0]->param;
		message = p_at_params->params[1]->param;
	}else if(3 == p_at_params->count){  //!< AT+MQPUB=topic,message,qos
		topic   = p_at_params->params[0]->param;
		message = p_at_params->params[1]->param;
		qos     = atoi(p_at_params->params[2]->param);
	}else{  //!< error
		at_cmd_params_release(p_at_params);
		return -1;
	}

	//< publish
	mqtt_attr_payload_t * payload = mqtt_attr_payload_new(1024);
	mqtt_attr_payload_write_string(payload, message);
	err = mqttor_client_publish(mq_sess, topic, payload, 
			qos, true);
	mqtt_attr_payload_release(payload);
	if(err){
		mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client publish fail!\n");
	}

	at_cmd_params_release(p_at_params);

	return err;
}

static at_cmd_cb_t mqttor_client_at[] = {
	{AT_FLAG_VISIABLE, "MQCONF", at_cmd_MQCONF_set_handler, 
		at_cmd_MQCONF_read_handler, NULL, NULL},
	{AT_FLAG_VISIABLE, "MQCON",  at_cmd_MQCON_set_handler, NULL, NULL, NULL},
	{AT_FLAG_VISIABLE, "MQDSC",  NULL, NULL, NULL, at_cmd_MQDSC_exec_handler},
	{AT_FLAG_VISIABLE, "MQSUB",  at_cmd_MQSUB_set_handler, NULL, NULL, NULL},
	{AT_FLAG_VISIABLE, "MQPUB",  at_cmd_MQPUB_set_handler, NULL, NULL, NULL}
};

#define TEST_AT_CMD_MAX_LEN       20
#define TEST_AT_CMD_MAX_PARAM_LEN 512
#define TEST_AT_CMD_DELIMITER     '\n'
#define TEST_HASH_TAB_SIZE        64


int main(int argc, char * argv[]){
	//< initialize mqttor client session
	int err = 0;
	mq_sess = mqttor_session_new();
	assert(mq_sess);

	//< register AT command table
	at_cmd_context_t * context = at_cmd_class_new(TEST_HASH_TAB_SIZE,
			TEST_AT_CMD_MAX_LEN, TEST_AT_CMD_MAX_PARAM_LEN, 
			TEST_AT_CMD_DELIMITER);
	at_table_register(context, mqttor_client_at, 
			sizeof(mqttor_client_at)/sizeof(mqttor_client_at[0]));

#ifdef REPL  //!< REPL mode

	signal(SIGINT, sighandler);
	signal(SIGPIPE, sighandler);

	char buffer[1024] = {0};
	mqtt_buf_t * buf = NULL;
	uint16_t seconds = 0;

	int fd = fileno(stdin);  
	int flags = fcntl(fd, F_GETFL, 0); 
	flags |= O_NONBLOCK; 
	fcntl(fd, F_SETFL, flags); 

	//< handle publish from boker
	buf = mqtt_buf_new(MQTT_FIXED_PACKET_LEN_MAX);
	mqtt_buf_t * buf_publish = NULL;
	assert(buf);
	while(runcond){
		//< handle input AT command
		if(NULL != fgets(buffer, sizeof(buffer), stdin)){
			at_cmd_execute_script_string(context, buffer);
		}

		if(is_connected){  //!< connected to broker and listen to publish
			//< receive publish packet
			err = recv(mq_sess->socket, buf->buf, buf->len,
					MSG_DONTWAIT);
			if(0 < err){  //!< receive packet
				mqtt_log_printf(LOG_LEVEL_LOG,
						"Mqttor client recv %d bytes!\n", err);
				seconds = 0;  //!< reset timeout
				buf_publish = mqtt_buf_new(err);
				memcpy(buf_publish->buf, buf->buf, buf_publish->len);
				err = mq_sess->on_publish(mq_sess, buf_publish);
				if(0 > err){
					mqtt_log_printf(LOG_LEVEL_ERR, 
							"Mqttor on publish error `%d`!\n", err);
				}
				mqtt_buf_release(buf_publish);
			}

			//< sleep one seconds
			sleep(1);
			seconds++;
			//< send pingreq
			if(seconds > mq_sess->config->keep_alive/2){
				seconds = 0;
				err = mqttor_client_pingreq(mq_sess);
				if(0 > err){
					mqtt_log_printf(LOG_LEVEL_WARN, 
							"Mqttor client pingreq fail!\n");
				}
			
			}

			//err = (n==count_publish ? 0 : err);
		}  //!< is_connected
	}  //!< runcond

	mqtt_buf_release(buf);

#else  //!< script mode

	at_cmd_execute_script(context, "test.at");

#endif

	at_cmd_class_release(context);

	return err;
}

