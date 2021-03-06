# ####################################################
# LU for dual processor platform
# ####################################################
SHELL := /bin/bash 
ECHO := /bin/echo -e
CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

INC_DIR := -I.

#SRCS := lu.c
SRCS := lu.c barrier.c ../acPthread.c

OBJS := $(SRCS:.c=.o)
OBJS_AUX := barrier.o lu.o acPthread.o

MACROS := c.m4.null
M4 := m4 -s -Ulen -Uindex

x = *
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
(TARGET): $(OBJS)
	$(CC) $(OBJS_AUX) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)

clean:
	rm -rf *.c *.h *.o *.x 

distclean: clean

.SILENT:

.SUFFIXES:	.o .c .C .h .H

bin:
	

.H.h:
	$(M4) $(MACROS) $*.H > $*.h

.C.c:
	$(M4) $(MACROS) $*.C > $*.c

.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $< 
	
.C.o:
	$(M4) $(MACROS) $*.C > $*.c
	$(CC) -c $(CFLAGS) $*.c

copy: 	
	$(ECHO) "---copy $(TARGET).x to the appropriate rundir"
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check

check:
	$(ECHO) "\n\nTESTING RESULTS"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then $(ECHO) "\nTest Passed.\n"; else $(ECHO) "\nTest Failed.\n"; fi
