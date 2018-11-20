
WITH_GCOV = yes

WITH_DEBUG = yes

#export CC = gcc
export INSTALL = install
export prefix = /usr/local
export CFLAGS = -Wall -std=gnu99
export BUILD_ROOT = $(shell pwd)

ifeq ($(WITH_GCOV),yes)
	CFLAGS += -coverage#-fprofile-arcs -ftest-coverage
endif

ifeq ($(WITH_DEBUG),yes)
	CFLAGS += -g
endif


DIRS = ./src/client AT-interpreter/ ./src/at
DIRS_TEST = ./src/client AT-interpreter/

.PHONY : all install uninstall test clean
all :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d}; done

install : 
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} install; done

uninstall :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} uninstall; done

test :
	set -e; for d in $(DIRS_TEST); do $(MAKE) -C $${d} test; done

clean :
	set -e; for d in $(DIRS); do $(MAKE) -C $${d} clean; done
