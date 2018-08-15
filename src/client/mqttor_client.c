/*! \brief a simple mqttor client
 *  \author Shylock Hg
 *  \date 2018-08-13
 *  \email tcath2s@gmail.com
 * */

/*! mqttor client command line usage
 *  synopsis -- mqttor_client <operation> [options]
 *  example 
 *          --  mqttor_client publish -t "topic" -m "message" 
 *          	[-q QoS] [-i host] [-p port]
 *          --  mqttor_client subscribe -t "topic" [-q will_QoS] 
 * */

#include <string.h>
#include <stdlib.h>

//#include <unistd.h>
#include <getopt.h>

#include "../../inc/client/mqttor_client_common.h"
#include "../../inc/client/mqttor_client.h"

#include "../../inc/toolkit/mqtt_log.h"

static const char * topic = "test/topic";
static const char * message = "hello world!";

static enum mqttor_QoS qos = MQTTOR_QoS_MONCE;

/* eclipse.org:1883 -- mqtt with tcp */
static const char * host = "37.187.106.16";
static int port  = 1883;

static void print_usage(void){
	printf("Mqttor client usage:\n");
	printf("\tsynopsis：mqttor_client <operation> [options]\n");
	printf("\tmqttor_client publish [-t topic] [-m message] [-q QoS] ");
	printf("[-i host] [-p port]\n");
	printf("\tmqttor_client subscribe [-t topic] [-q requested_QoS]\n");
}

/*! \brief parse mqttor client command line parameter
 *  \param mq_sess mqttor session
 *  \param argc count of arguments
 *  \param argv array of arguments
 *  \retval 0 for help 1 for publish 2 for subscribe  
 *          and negative number for error
 * */
static int mqttor_client_argparser(mqttor_session_t * mq_sess, int argc, 
		char * argv[]){
	assert(mq_sess);
	assert(argc);
	assert(argv);

	if(2 > argc)  //!< mqttor_client publish
		return -E_INVAL;

	char opt = '\0';

	if(0 == strcmp("publish", argv[1])){  //!< publish
		while(-1 != (opt = getopt(argc-1, argv+1, "t:m:q:i:p:h"))){
			//mqtt_log_printf(LOG_LEVEL_LOG, "Option is `%c`!\n", opt);
			switch(opt){
				case 't':  //!< topic
					topic = optarg;
					break;
				case 'm':  //!< message
					message = optarg;
					break;
				case 'q':  //!< QoS
					//mq_sess->config->qos = atoi(optarg);
					qos = atoi(optarg);
					break;
				case 'i':  //!< broker ip
					//mq_sess->config->broker_ip = optarg;
					host = optarg;
					break;
				case 'p':  //!< broker port
					//mq_sess->config->broker_port = atoi(optarg);
					port = atoi(optarg);
					break;
				case 'h':  //!< help
					return 0;
					break;
				default:
					mqtt_log_printf(LOG_LEVEL_WARN, 
							"Unkown option `-%c`", 
							opt);
			}
		}
		return 1;  //!< publish
	}else if(0 == strcmp("subscribe", argv[1])){  //!< subscribe
	
		return 2;  //!< subscribe
	}else{
		return -E_INVAL;
	}
}

int main(int argc, char * argv[]){

	int err = 0;
	mqttor_session_t * mq_sess = mqttor_session_new();
	assert(mq_sess);

	//mq_sess->config->id_client = MQTTOR_CLIENT_ID;
	err = mqttor_client_argparser(mq_sess, argc, argv);
	//mqtt_log_printf(LOG_LEVEL_LOG, "Mqttor parser error is `%d`!\n", err);
	switch(err){
		case 0:  //!< help
			print_usage();
			break;
		case 1:  //!< publish
#if 0
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Publish topic is `%s`\n", topic);
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Publish message is `%s`\n", message);
			mqtt_log_printf(LOG_LEVEL_LOG, "Publish qos is `%d`\n", 
					qos/*mq_sess->config->qos*/);
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Publish host is `%s`\n", 
					mq_sess->config->broker_ip);
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Publish port is `%d`\n", 
					mq_sess->config->broker_port);
#endif

			//< connect
			err = mqttor_client_connect(mq_sess, 
					host, port);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, 
						"Mqttor client connect to broker (%s,%d) fail!\n",
						host/*MQTTOR_BROKER_IP*/, 
						port/*MQTTOR_BROKER_PORT*/);
				return err;
			}

			//< publish
			mqtt_attr_payload_t * payload = mqtt_attr_payload_new(1024);
			mqtt_attr_payload_write_string(payload, message);
			err = mqttor_client_publish(mq_sess, topic, payload, 
					qos, true);
			mqtt_attr_payload_release(payload);
			if(err){
				mqtt_log_printf(LOG_LEVEL_ERR, "Mqttor client publish fail!\n"
					       );
				return err;
			}

			//< disconnect
			mqttor_client_disconnect(mq_sess);

			break;
		case 2:  //!< subscribe
			break;
		default:
			mqtt_log_printf(LOG_LEVEL_LOG, 
					"Mqttor comand line parse fail!\n");
			err = -E_INVAL;
	}
	

	mqttor_session_release(mq_sess);

	return err;
}

