#CC = gcc
LN = ln
AR = ar
INSTALL = install

CFLAGS_LOCAL = 
CFLAGS_LOCAL += $(CFLAGS)

INCLUDES = -I../../inc

prefix = /usr/local

DIR_BUILD = build

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d $(INCLUDES)

LIB_SOURCES = src/client/mqttor_client_common.c \
	  ../core/mqtt_fixed_header.c \
	  ../core/mqtt_packet.c \
	  ../core/mqtt_packet_segment.c \
	  ../core/mqtt_payload.c \
	  ../core/mqtt_var_header.c \
	  ../session/mqttor_session.c \
	  ../toolkit/mqtt_log.c

APP_SOURCE = src/client/mqttor_client.c

LIB_OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(LIB_SOURCES))))

OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(APP_SOURCE))))

TARGET = mqttor_client

LIBVERSION = 0.0.1
LIBNAME = mqttor

LIB_SO_MQTTOR = lib$(LIBNAME).so.$(LIBVERSION)
LIB_A_MQTTOR = lib$(LIBNAME).a.$(LIBVERSION)

DEPFILES = $(patsubst %.o, %.d, $(OBJECTS))

# set c sources search path
vpath %.c $(sort $(dir $(LIB_SOURCES)))

# set c headers search path
#vpath %.h $(sort $(dir $(INCLUDES)))

.PHONY : all clean install
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(OBJECTS) $(DIR_BUILD)/$(LIB_SO_MQTTOR) Makefile
	$(CC) $(CFLAGS_LOCAL) -o $@ $< -L$(shell pwd)/$(DIR_BUILD) -l$(LIBNAME)

$(DIR_BUILD)/$(LIB_SO_MQTTOR) : $(LIB_OBJECTS) Makefile
	$(CC) $(CFLAGS_LOCAL) -shared -o $@ $(LIB_OBJECTS)
	$(LN) -sf $(shell pwd)/$(DIR_BUILD)/$(LIB_SO_MQTTOR) $(DIR_BUILD)/lib$(LIBNAME).so

$(DIR_BUILD)/$(OBJECTS) : $(APP_SOURCE) Makefile
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -fPIC -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

install : all
	$(INSTALL) -d "${DESTDIR}${prefix}/lib"
	$(INSTALL) $(DIR_BUILD)/$(LIB_SO_MQTTOR) "${DESTDIR}${prefix}/lib"
	$(LN) -sf ${DESTDIR}${prefix}/lib/$(LIB_SO_MQTTOR) ${DESTDIR}${prefix}/lib/lib${LIBNAME}.so
	$(INSTALL) -d "${DESTDIR}${prefix}/bin"
	$(INSTALL) $(DIR_BUILD)/$(TARGET) "${DESTDIR}${prefix}/bin"
	
uninstall :
	rm -f "${DESTDIR}${prefix}/lib/$(LIB_SO_MQTTOR)"
	rm -f "${DESTDIR}${prefix}/lib/lib${LIBNAME}.so"
	rm -f "${DESTDIR}${prefix}/bin/$(TARGET)"

clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

