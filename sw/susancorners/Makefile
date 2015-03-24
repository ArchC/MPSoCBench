CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)
CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

INC_DIR := -I.

SRCS := susan_parallel.c ../acPthread.c

OBJS_AUX := susan_parallel.o acPthread.o

OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(CC) $(OBJS_AUX) $(CFLAGS) -o $(TARGET).x $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET).x
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
bin:
	tar xzf qsort-bin.tgz
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<

copy: 	
	echo ---copy $(TARGET).x to the appropriate rundir
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp input_large2.pgm ../../rundir/$(RUNDIRNAME)/	
	cp input_small.pgm ../../rundir/$(RUNDIRNAME)/
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check

check:
	echo "\n\nTESTING RESULTS"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then echo "\nTest Passed."; else echo "\nTest Failed."; fi
