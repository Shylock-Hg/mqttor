
#ifndef _MQTT_LOG_H_
#define _MQTT_LOG_H_

//#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

void mqtt_log_print_buf(uint8_t * buf, size_t len);

#ifdef __cplusplus
	}  //!< extern "C"
#endif

#endif //!< _MQTT_LOG_H_

