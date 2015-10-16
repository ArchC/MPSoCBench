ECHO = /bin/echo -e
CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -DLITTLE_ENDIAN -specs=archc
endif
LDFLAGS = -lm

INC_DIR := -I.

SRCS := sha.c sha_driver.c ../acPthread.c

OBJS := $(SRCS:.c=.o)

OBJS_AUX := sha.o sha_driver.o acPthread.o
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.SILENT:

#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.SUFFIXES: .o .c .h

#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
all: $(OBJS)
	$(CC) $(OBJS_AUX) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
clean:
	rm -f *.o *.x *~ $(TARGET).x 
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
distclean: clean
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
bin:
	
#"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---"---
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<

copy: 
	$(ECHO) "---copy $(TARGET).x to the appropriate rundir\n"\n"
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp *.asc ../../rundir/$(RUNDIRNAME)/
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check

check:
	$(ECHO) "\n\nTESTING RESULTS"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then $(ECHO) "\nTest Passed.\n"; else $(ECHO) "\nTest Failed.\n"; fi
