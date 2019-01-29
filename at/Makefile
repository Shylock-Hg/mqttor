#CC = gcc
#RM = rm
#AR = ar
LN = ln
INSTALL = install
MKDIR = mkdir

DIR_BUILD = .build
prefix = /usr/local

PPFLAGS = -MT $@ -MMD -MP -MF $(patsubst %.o, %.d, $@)

CFLAGS_LOCAL = -Wall -g -std=gnu99 -coverage
CFLAGS_LOCAL += $(CFLAGS)

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

APP_SOURCE = mqttor_client_at.c
APP_OBJECT = mqttor_client_at.o
APP = mqttor_client_at

LIB_DIR = ../mqtt ../AT-interpreter
LIB_TARGET_DIR = $(addsuffix /$(DIR_BUILD), $(LIB_DIR))
LIB_NAME = mqtt at
LIB_TARGET = ../mqtt/$(DIR_BUILD)/libmqtt.so \
	../AT-interpreter/$(DIR_BUILD)/libat.so

DEPFILES = $(patsubst %.o, %.d, $(DIR_BUILD)/$(APP_OBJECT))

.PHONY : all clean install uninstall test

all : $(DIR_BUILD) $(DIR_BUILD)/$(APP)

$(DIR_BUILD)/$(APP) : $(DIR_BUILD)/$(APP_OBJECT) $(LIB_TARGET) Makefile | $(DIR_BUILD)
	$(CC) $(CFLAGS_LOCAL) -o $@ $< ${addprefix -L, $(LIB_TARGET_DIR)} ${addprefix -l, $(LIB_NAME)}

$(addprefix $(DIR_BUILD)/, $(APP_OBJECT)) : $(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(MKDIR) -p $(@D)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(LIB_TARGET) : $(LIB_DIR)
	for dir in $(LIB_DIR); do $(MAKE) -C $${dir}; done

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	$(MKDIR) -p $@

install : all
	$(INSTALL) -d "$(prefix)/bin"
	$(INSTALL) "$(DIR_BUILD)/$(APP)" "$(prefix)/bin"

uninstall :
	$(RM) -f  "$(prefix)/bin/$(APP)"

test :
	$(VALGRIND) $(APP) -h
	$(TARGET) pub -t 'test/topic' -m 'QoS0' -q 0
	$(TARGET) pub -t 'test/topic' -m 'QoS1' -q 1
	$(TARGET) pub -t 'test/topic' -m 'QoS2' -q 2
	$(TARGET) sub -t 'test/topic' -n 1

clean :
	$(RM) -rf $(DIR_BUILD)

include $(DEPFILES)
