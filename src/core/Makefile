DIR_INC = ../../inc
OBJS = test.o mqtt_fixed_header.o 

.PHONY : all
all : $(OBJS)
	cc -o test $(OBJS)

test.o : $(DIR_INC)/mqtt_fixed_header.h
mqtt_fixed_header.o : $(DIR_INC)/mqtt_fixed_header.h

.PHONY : clean
clean : 
	rm test $(OBJS)

