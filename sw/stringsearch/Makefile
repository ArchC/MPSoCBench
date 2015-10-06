CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS = -DNPROC=$(NUMPROCESSORS) $(POWER_SIM_FLAG) -specs=archc
endif
LDFLAGS = -lm

INC_DIR := -I.

SRCS := bmhasrch.c  bmhisrch.c  bmhsrch.c  stringsearch.c ../acPthread.c
OBJS_AUX := bmhasrch.c  bmhisrch.c  bmhsrch.c  stringsearch.c acPthread.o
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
	rm -f *.o *~ *.x 
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
bin:
	
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
copy:
	echo ---copy $(TARGET).x to the appropriate rundir
	cp $(TARGET).x ../../rundir/$(RUNDIRNAME)/
	cp *.txt ../../rundir/$(RUNDIRNAME)/
	cp output.base ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check
ifeq ($(PROCESSOR),sparc)
	echo "\n\nATTENTION: We're sorry. There are problems running stringsearch just in sparc processors. \nMaybe the output file will contain the wrong answer at the end of simulation.\n\n"
endif
	
check:
	echo "\n\nTESTING RESULTS"
	#sort output > output.sorted
	if diff output output.base > diff.txt ; then echo "\nTest Passed."; else echo "\nTest Failed."; fi
	
