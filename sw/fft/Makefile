# ####################################################
# FFT for dual processor platform
# ####################################################

CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) -specs=archc
endif

LDFLAGS = -lm


INC_DIR := -I.

SRCS := fft.c
OBJS := $(SRCS:.c=.o)
MACROS := c.m4.null
M4 := m4 -s -Ulen -Uindex

x = *
#------------------------------------------------------
(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)

clean:
	rm -rf *.c *.h *.o *.x 

distclean: clean

.SUFFIXES:
.SUFFIXES:	.o .c .C .h .H

.H.h:
	$(M4) $(MACROS) $*.H > $*.h

.C.c:
	$(M4) $(MACROS) $*.C > $*.c

.c.o:
	$(CC) -c $(CFLAGS) $*.c

.C.o:
	$(M4) $(MACROS) $*.C > $*.c
	$(CC) -c $(CFLAGS) $(INC_DIR) $*.c

copy: 	
	echo ---copy $(TARGET).x to the appropriate rundir
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check

check:
	echo "\n\nTESTING RESULTS"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then echo "\nTest Passed."; else echo "\nTest Failed."; fi


