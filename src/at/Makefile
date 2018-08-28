CC = gcc

CFLAGS = -Wall -fprofile-arcs -ftest-coverage -g

DIR_BUILD = ./build

INCLUDES = -I../../inc -I../../AT-interpreter/inc

SOURCES =\
      ../toolkit/mqtt_log.c \
	  ../core/mqtt_fixed_header.c \
	  ../core/mqtt_packet.c \
	  ../core/mqtt_packet_segment.c \
	  ../core/mqtt_payload.c \
	  ../core/mqtt_var_header.c \
	  ../session/mqttor_session.c \
	  ../client/mqttor_client_common.c \
	  ../../AT-interpreter/src/at_command.c \
      ../../AT-interpreter/src/at_fsm.c \
	  ../../AT-interpreter/src/at_xrecord.c \
	  ../../AT-interpreter/src/at_param.c \
	  ../../AT-interpreter/src/at_table.c \
	  ../../AT-interpreter/src/hash.c \
	  ../../AT-interpreter/src/queue.c \
	  ../../AT-interpreter/src/stdlog.c \
	  mqttor_client_at.c


TARGET = test

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d $(INCLUDES)

OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

DEPFILES = $(patsubst %.o, %.d, $(OBJECTS))

# set c sources search path
vpath %.c $(sort $(dir $(SOURCES)))

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

