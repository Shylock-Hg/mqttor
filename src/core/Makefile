CC = gcc

CFLAGS = -std=c99 -Wall -g -fprofile-arcs -ftest-coverage -g

DIR_BUILD = ./build

INCLUDES = -I../../inc

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d $(INCLUDES)

SOURCES = $(wildcard *.c) ../toolkit/mqtt_log.c


OBJS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

DEPFILES = $(patsubst %.o, %.d, $(OBJS))

# core header
vpath %.h $(sort $(dir $(INCLUDES)))

.PHONY : all output clean
all : $(DIR_BUILD)/test

$(DIR_BUILD)/test : $(OBJS) output
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/%.o : %.c $(DIR_BUILD)/%.d output
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD)/mqtt_log.o : ../toolkit/mqtt_log.c
	$(CC) $(CFLAGS) -c $< -o $@

#create build directory
output : 
	mkdir -p $(DIR_BUILD)

#clean outputs
clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

