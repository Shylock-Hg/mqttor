CC = gcc

CFLAGS = -std=c99 -Wall -fprofile-arcs -ftest-coverage -g

DIR_BUILD = ./build

INCLUDES = -I../../inc

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d $(INCLUDES)

SOURCES = src/client/mqttor_client.c \
	  src/client/mqttor_client_common.c \
	  ../core/mqtt_fixed_header.c \
	  ../core/mqtt_packet.c \
	  ../core/mqtt_packet_segment.c \
	  ../core/mqtt_payload.c \
	  ../core/mqtt_var_header.c \
	  ../session/mqttor_session.c \
	  ../toolkit/mqtt_log.c


OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

TARGET = test

DEPFILES = $(patsubst %.o, %.d, $(OBJECTS))

# set c sources search path
vpath %.c $(sort $(dir $(SOURCES)))

# set c headers search path
#vpath %.h $(sort $(dir $(INCLUDES)))

.PHONY : all clean
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

#$(OBJECTS): $(DIR_BUILD)/%.o: %.c
	#mkdir -p $(@D)
	#$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

