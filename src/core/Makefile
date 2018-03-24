CFLAGS = -std=c99

DIR_INC = ../../inc
DIR_BUILD = ./build
OBJS = $(DIR_BUILD)/test.o $(DIR_BUILD)/mqtt_fixed_header.o $(DIR_BUILD)/mqtt_var_header.o

.PHONY : all
all : $(DIR_BUILD)/test

$(DIR_BUILD)/test : $(OBJS) output
	cc $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/test.o : test.c mqtt_fixed_header.c $(DIR_INC)/mqtt_fixed_header.h \
	output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_fixed_header.o : mqtt_fixed_header.c $(DIR_INC)/mqtt_fixed_header.h \
	output
	cc $(CFLAGS) -o $@ -c $<

$(DIR_BUILD)/mqtt_var_header.o : mqtt_var_header.c $(DIR_INC)/mqtt_var_header.h\
	output
	cc $(CFLAGS) -o $@ -c $<

#create build directory
.PHONY : output
output : 
	mkdir -p ./build

#clean outputs
.PHONY : clean
clean : 
	rm $(DIR_BUILD)/test $(OBJS)

