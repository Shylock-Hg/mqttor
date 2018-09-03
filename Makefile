
WITH_GCOV = no

PLATFORM = -DMQTTOR_LINUX #-DMQTTOR_HI2115

#export CC = gcc
export INSTALL = install
export prefix = /usr/local
export CFLAGS = -Wall -g -std=gnu99
CFLAGS += $(PLATFORM)

ifeq ($(WITH_GCOV),yes)
	CFLAGS += -fprofile-arcs -ftest-coverage
endif


DIRS = ./src/client AT-interpreter/ ./src/at

.PHONY : all install clean
all :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d}; done

install : 
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} install; done

uninstall :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} uninstall; done

clean :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} clean; done


