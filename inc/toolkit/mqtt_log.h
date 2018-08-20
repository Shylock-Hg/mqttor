
#ifndef _MQTT_LOG_H_
#define _MQTT_LOG_H_

//#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>

#include "port/config.h"

#define LOG_LEVEL_ERR  "[err]"
#define LOG_LEVEL_WARN "[warn]"
#define LOG_LEVEL_LOG  "[log]"

#define LOG_TYPE_CHAR  "char"
#define LOG_TYPE_BIN   "bin"

void __mqtt_log_print_buf(uint8_t * buf, size_t len);

//#if configDBG
#define mqtt_log_printf(level, format, ...) printf(level " : " __FILE__ "-" "%u" " : " LOG_TYPE_CHAR " : " format , __LINE__,##__VA_ARGS__)

#define mqtt_log_print_buf(level, buf, len) do{\
	printf(level " : " __FILE__ "-" "%u" " : " LOG_TYPE_BIN " : ",__LINE__);\
	__mqtt_log_print_buf((uint8_t*)buf,len);\
}while(0);
//#else   //!< configLOG
	//#define mqtt_log_printf(level, format, ...)
	//#define mqtt_log_print_buf(level, buf, len)
//#endif  //!< configLOG

#ifdef __cplusplus
	}  //!< extern "C"
#endif

#endif //!< _MQTT_LOG_H_

