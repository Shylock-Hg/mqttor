#CC = gcc
#RM = rm
#AR = ar
LN = ln
INSTALL = install
MKDIR = mkdir

DIR_BUILD = .build
prefix = /usr/local

PPFLAGS = -MT $@ -MMD -MP -MF $(patsubst %.o, %.d, $@)

CFLAGS_LOCAL = -Wall -g -std=c99 -coverage
CFLAGS_LOCAL += $(CFLAGS)

ifeq ($(WITH_VALGRIND), yes)
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
endif

APP_SOURCE = mqttor_client.c
APP_OBJECT = mqttor_client.o
APP = mqttor_client

LIB_DIR = ../mqtt
LIB_NAME = mqtt

DEPFILES = $(patsubst %.o, %.d, $(DIR_BUILD)/$(APP_OBJECT))

.PHONY : all clean install uninstall test

all : $(DIR_BUILD) $(DIR_BUILD)/$(APP)

$(DIR_BUILD)/$(APP) : $(DIR_BUILD)/$(APP_OBJECT) $(LIB_DIR)/$(DIR_BUILD)/lib$(LIB_NAME).so Makefile | $(DIR_BUILD)
	$(CC) $(CFLAGS_LOCAL) -o $@ $< -L$(LIB_DIR)/$(DIR_BUILD) -l$(LIB_NAME)

$(addprefix $(DIR_BUILD)/, $(APP_OBJECT)) : $(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(MKDIR) -p $(@D)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(LIB_DIR)/$(DIR_BUILD)/lib$(LIB_NAME).so :
	$(MAKE) -C $(LIB_DIR)

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
	$(VALGRIND) $(APP) pub -t 'test/topic' -m 'QoS0' -q 0
	$(VALGRIND) $(APP) pub -t 'test/topic' -m 'QoS1' -q 1
	$(VALGRIND) $(APP) pub -t 'test/topic' -m 'QoS2' -q 2
	$(VALGRIND) $(APP) sub -t 'test/topic' -n 1

clean :
	$(RM) -rf $(DIR_BUILD)

include $(DEPFILES)
