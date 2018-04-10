CFLAGS = -std=c99 -Wall

DIR_BUILD = ./build

OBJS = $(DIR_BUILD)/test.o $(DIR_BUILD)/mqtt_fixed_header.o $(DIR_BUILD)/mqtt_var_header.o \
       $(DIR_BUILD)/mqtt_payload.o $(DIR_BUILD)/mqtt_packet_segment.o

# core header
vpath %.h ../../inc/core
vpath %.h ../../inc/toolkit

.PHONY : all output clean
all : $(DIR_BUILD)/test

$(DIR_BUILD)/test : $(OBJS) output
	cc $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/test.o : test.c mqtt_fixed_header.h mqtt_var_header.h mqtt_payload.h output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_packet_segment.o : mqtt_packet_segment.c mqtt_packet_segment.h array.h output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_fixed_header.o : mqtt_fixed_header.c mqtt_fixed_header.h output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_var_header.o : mqtt_var_header.c mqtt_var_header.h output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_payload.o : mqtt_payload.c mqtt_payload.h mqtt_packet_segment.h output
	cc $(CFLAGS) -o $@ -c $<

#create build directory
output : 
	mkdir -p ./build

#clean outputs
clean : 
	rm $(DIR_BUILD)/test $(OBJS)

