CC = $(CROSS)
TARGET = $(SOFTWARE).$(PROCESSOR)

CFLAGS = -msoft-float -DNPROC=$(NUMPROCESSORS) -specs=archc

ifeq ($(PROCESSOR),arm)
CFLAGS =  -DNPROC=$(NUMPROCESSORS) -specs=archc
endif

LDFLAGS = -lm

INC_DIR := -I.

SRCS := parallel_basicmath.c rad2deg.c isqrt.c cubic.c ../acPthread.c

OBJS := $(SRCS:.c=.o) 
OBJS_AUX := parallel_basicmath.o rad2deg.o isqrt.o cubic.o acPthread.o

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
	cp base_output.$(SOFTWARE).$(NUMPROCESSORS) ../../rundir/$(RUNDIRNAME)/output.base
	cp Makefile ../../rundir/$(RUNDIRNAME)/Makefile.check


check:
	echo "\n\nTESTING RESULTS:"
	sort output > output.sorted 
	if diff output.sorted output.base > diff.txt ; then echo "Test Passed."; else echo "Test Failed."; fi
