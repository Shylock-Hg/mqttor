CFLAGS = -std=c99 -Wall

DIR_INC = ../../inc
DIR_BUILD = ./build
OBJS = $(DIR_BUILD)/test.o $(DIR_BUILD)/mqtt_fixed_header.o $(DIR_BUILD)/mqtt_var_header.o \
       $(DIR_BUILD)/mqtt_payload.o

.PHONY : all output clean
all : $(DIR_BUILD)/test

$(DIR_BUILD)/test : $(OBJS) output
	cc $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/test.o : test.c output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_fixed_header.o : mqtt_fixed_header.c output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_var_header.o : mqtt_var_header.c output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_payload.o : mqtt_payload.c output
	cc $(CFLAGS) -o $@ -c $<

#create build directory
output : 
	mkdir -p ./build

#clean outputs
clean : 
	rm $(DIR_BUILD)/test $(OBJS)

