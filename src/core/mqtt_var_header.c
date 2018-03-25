#include "../../inc/mqtt_var_header.h"

#include <stdint.h>

uint8_t mqtt_connect_flag_pack_s(struct mqtt_connect_flag * p_mqtt_connect_flag){
	//!< check parameter
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_user_name);
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_pwd);
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_w_retain);
	MQTT_CONNECT_FLAG_CHECK_QoS(p_mqtt_connect_flag->flag_w_QoS);
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_w_flag);
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_clean_session);
	MQTT_CONNECT_FLAG_CHECK_BOOL(p_mqtt_connect_flag->flag_reserved);

	//!< pack mqtt connect flags
	return MQTT_CONNECT_FLAG_PACK(p_mqtt_connect_flag);
}
