#include <stdio.h>
#include <stdint.h>


void __mqtt_log_print_buf(uint8_t * buf, size_t len){
	for(int i=0; i<len; i++){
		printf("%2x",buf[i]);
	}
	printf("\n");
}

