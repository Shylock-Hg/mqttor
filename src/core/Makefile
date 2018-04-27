CFLAGS = -std=c99 -Wall

DIR_BUILD = ./build

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

SOURCES = $(wildcard *.c)

OBJS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

DEPFILES = $(patsubst %.o, %.d, $(OBJS))

# core header
#vpath %.h ../../inc/core
#vpath %.h ../../inc/toolkit

.PHONY : all output clean
all : $(DIR_BUILD)/test

$(DIR_BUILD)/test : $(OBJS) output
	cc $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/%.o : %.c $(DIR_BUILD)/%.d output
	cc $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

#create build directory
output : 
	mkdir -p $(DIR_BUILD)

#clean outputs
clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

