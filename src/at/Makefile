INSTALL = install

CFLAGS_LOCAL =
CFLAGS_LOCAL += $(CFLAGS)

DIR_BUILD = build

INCLUDES = -I../../inc -I../../AT-interpreter/inc

prefix = /usr/local

APP_SOURCE = ./mqttor_client_at.c

TARGET = mqttor_client_at

#PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d $(INCLUDES)
PPFLAGS = -MT $@ -MMD -MP -MF $*.d $(INCLUDES)

#OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))
APP_OBJECT = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(APP_SOURCE))))
#APP_OBJECT = mqttor_client_at.o

DEPFILES = $(patsubst %.o, %.d, $(APP_OBJECT))

# set c sources search path
#vpath %.c $(sort $(dir $(SOURCES)))

.PHONY : all clean install uninstall
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(APP_OBJECT) Makefile $(DIR_BUILD)
	$(CC) $(CFLAGS_LOCAL) -o $@ $< -L$(BUILD_ROOT)/src/client/build -L$(BUILD_ROOT)/AT-interpreter/.build -lat -lmqttor

$(APP_OBJECT) : $(APP_SOURCE) Makefile $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

#$(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	#$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

#$(OBJECTS): $(DIR_BUILD)/%.o: %.c
	#mkdir -p $(@D)
	#$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

install : all
	$(INSTALL) -d "${DESTDIR}${prefix}/bin"
	$(INSTALL) $(DIR_BUILD)/$(TARGET) "${DESTDIR}${prefix}/bin"

test: ;

uninstall :
	rm -f "${DESTDIR}${prefix}/bin/$(TARGET)"

clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

