/*  \brief test mqtt_var_header.h
 * */

#include "mqtt_var_header.h"

#include "stdio.h"

int main(int argc, char * argv[]){
	uint8_t bytes[2] = {0};
	MQTT_PACKET_IDENTIFIER_PACK(55,bytes);
	printf("[info]:origin identifier=`55`,evaluate value=`%u`\n",MQTT_PACKET_IDENTIFIER_EVAL(bytes));
}

